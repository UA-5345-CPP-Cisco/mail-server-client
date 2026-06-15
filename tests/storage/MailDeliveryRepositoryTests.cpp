#include <gtest/gtest.h>

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "storage/Database.h"
#include "storage/MailMessageRecord.h"
#include "storage/MailMessageRepository.h"
#include "storage/MessageRecipientRecord.h"
#include "storage/MessageRecipientRepository.h"
#include "storage/MigrationRunner.h"
#include "storage/UserRepository.h"

namespace {

class MailDeliveryRepositoryTest : public testing::Test
{
protected:
	void SetUp() override
	{
		const auto timestamp =
			std::chrono::steady_clock::now().time_since_epoch().count();

		m_database_path =
			std::filesystem::temp_directory_path() /
			("mail_delivery_test_" + std::to_string(timestamp) + ".sqlite3");

		m_database = std::make_unique<Storage::Database>(m_database_path);

		Storage::MigrationRunner migration_runner(
			*m_database,
			STORAGE_TEST_MIGRATIONS_DIR
		);
		migration_runner.Run();

		m_user_repository =
			std::make_unique<Storage::UserRepository>(*m_database);
		m_message_repository =
			std::make_unique<Storage::MailMessageRepository>(*m_database);
		m_recipient_repository =
			std::make_unique<Storage::MessageRecipientRepository>(*m_database);
	}

	void TearDown() override
	{
		m_recipient_repository.reset();
		m_message_repository.reset();
		m_user_repository.reset();
		m_database.reset();

		std::error_code error_code;
		std::filesystem::remove(m_database_path, error_code);
		std::filesystem::remove(m_database_path.string() + "-shm", error_code);
		std::filesystem::remove(m_database_path.string() + "-wal", error_code);
	}

	std::int64_t CreateUser()
	{
		return m_user_repository->CreateUser(
			"sender",
			"sender@example.com",
			"password-hash"
		);
	}

	std::int64_t CreateMessage(
		Storage::MailMessageStatus status = Storage::MailMessageStatus::Queued
	)
	{
		return m_message_repository->CreateMessage(
			std::nullopt,
			"sender@example.com",
			std::string("Subject"),
			"Body",
			std::nullopt,
			status
		);
	}

	std::filesystem::path m_database_path;
	std::unique_ptr<Storage::Database> m_database;
	std::unique_ptr<Storage::UserRepository> m_user_repository;
	std::unique_ptr<Storage::MailMessageRepository> m_message_repository;
	std::unique_ptr<Storage::MessageRecipientRepository>
		m_recipient_repository;
};

TEST_F(MailDeliveryRepositoryTest, CreatesAndFindsMessages)
{
	const std::int64_t user_id = CreateUser();
	const std::int64_t parent_message_id =
		m_message_repository->CreateMessage(
			user_id,
			"sender@example.com",
			std::nullopt,
			"Parent body",
			std::nullopt,
			Storage::MailMessageStatus::Draft
		);
	const std::int64_t reply_message_id =
		m_message_repository->CreateMessage(
			user_id,
			"sender@example.com",
			std::string("Reply"),
			"Reply body",
			parent_message_id
		);

	const std::optional<Storage::MailMessageRecord> parent =
		m_message_repository->FindById(parent_message_id);
	const std::optional<Storage::MailMessageRecord> reply =
		m_message_repository->FindById(reply_message_id);

	ASSERT_TRUE(parent.has_value());
	ASSERT_TRUE(reply.has_value());

	EXPECT_EQ(parent->sender_user_id, user_id);
	EXPECT_EQ(parent->sender_email, "sender@example.com");
	EXPECT_FALSE(parent->subject.has_value());
	EXPECT_EQ(parent->body, "Parent body");
	EXPECT_FALSE(parent->reply_to_message_id.has_value());
	EXPECT_FALSE(parent->created_at.empty());
	EXPECT_EQ(parent->status, Storage::MailMessageStatus::Draft);

	EXPECT_EQ(reply->subject, "Reply");
	EXPECT_EQ(reply->reply_to_message_id, parent_message_id);
	EXPECT_EQ(reply->status, Storage::MailMessageStatus::Queued);
	EXPECT_FALSE(m_message_repository->FindById(reply_message_id + 1).has_value());
}

TEST_F(MailDeliveryRepositoryTest, FindsAndTransitionsMessagesByStatus)
{
	const std::int64_t first_message_id = CreateMessage();
	CreateMessage(Storage::MailMessageStatus::Draft);
	const std::int64_t second_message_id = CreateMessage();

	const std::vector<Storage::MailMessageRecord> queued_messages =
		m_message_repository->FindByStatus(
			Storage::MailMessageStatus::Queued,
			1
		);

	ASSERT_EQ(queued_messages.size(), 1);
	EXPECT_EQ(queued_messages[0].id, first_message_id);

	EXPECT_TRUE(
		m_message_repository->UpdateStatus(
			first_message_id,
			Storage::MailMessageStatus::Queued,
			Storage::MailMessageStatus::Sending
		)
	);
	EXPECT_FALSE(
		m_message_repository->UpdateStatus(
			first_message_id,
			Storage::MailMessageStatus::Queued,
			Storage::MailMessageStatus::Sent
		)
	);
	EXPECT_TRUE(
		m_message_repository->UpdateStatus(
			first_message_id,
			Storage::MailMessageStatus::Sending,
			Storage::MailMessageStatus::Sent
		)
	);

	const std::vector<Storage::MailMessageRecord> remaining_messages =
		m_message_repository->FindByStatus(
			Storage::MailMessageStatus::Queued,
			10
		);

	ASSERT_EQ(remaining_messages.size(), 1);
	EXPECT_EQ(remaining_messages[0].id, second_message_id);
}

TEST_F(MailDeliveryRepositoryTest, EnforcesMessageRelationships)
{
	const std::int64_t user_id = CreateUser();
	const std::int64_t message_id =
		m_message_repository->CreateMessage(
			user_id,
			"sender@example.com",
			std::nullopt,
			"Body",
			std::nullopt
		);

	EXPECT_THROW(
		m_message_repository->CreateMessage(
			user_id + 1,
			"sender@example.com",
			std::nullopt,
			"Body",
			std::nullopt
		),
		std::runtime_error
	);
	EXPECT_THROW(
		m_message_repository->CreateMessage(
			user_id,
			"sender@example.com",
			std::nullopt,
			"Body",
			message_id + 1
		),
		std::runtime_error
	);

	m_database->Execute("DELETE FROM users WHERE id = 1;");

	const std::optional<Storage::MailMessageRecord> message =
		m_message_repository->FindById(message_id);

	ASSERT_TRUE(message.has_value());
	EXPECT_FALSE(message->sender_user_id.has_value());
	EXPECT_EQ(message->sender_email, "sender@example.com");
}

TEST_F(MailDeliveryRepositoryTest, CreatesAndFindsRecipients)
{
	const std::int64_t message_id = CreateMessage();
	const std::int64_t to_recipient_id =
		m_recipient_repository->CreateRecipient(
			message_id,
			"to@example.com",
			Storage::RecipientType::To
		);
	const std::int64_t cc_recipient_id =
		m_recipient_repository->CreateRecipient(
			message_id,
			"cc@example.com",
			Storage::RecipientType::Cc,
			Storage::DeliveryStatus::Queued
		);
	m_recipient_repository->CreateRecipient(
		message_id,
		"bcc@example.com",
		Storage::RecipientType::Bcc
	);

	const std::optional<Storage::MessageRecipientRecord> to_recipient =
		m_recipient_repository->FindById(to_recipient_id);
	const std::vector<Storage::MessageRecipientRecord> recipients =
		m_recipient_repository->FindByMessageId(message_id);

	ASSERT_TRUE(to_recipient.has_value());
	EXPECT_EQ(to_recipient->message_id, message_id);
	EXPECT_EQ(to_recipient->recipient_email, "to@example.com");
	EXPECT_EQ(to_recipient->recipient_type, Storage::RecipientType::To);
	EXPECT_EQ(
		to_recipient->delivery_status,
		Storage::DeliveryStatus::Pending
	);
	EXPECT_EQ(to_recipient->attempt_count, 0);
	EXPECT_FALSE(to_recipient->next_attempt_at.has_value());
	EXPECT_FALSE(to_recipient->last_error.has_value());
	EXPECT_FALSE(to_recipient->delivered_at.has_value());

	ASSERT_EQ(recipients.size(), 3);
	EXPECT_EQ(recipients[1].id, cc_recipient_id);
	EXPECT_EQ(recipients[1].recipient_type, Storage::RecipientType::Cc);
	EXPECT_EQ(
		recipients[1].delivery_status,
		Storage::DeliveryStatus::Queued
	);
}

TEST_F(MailDeliveryRepositoryTest, EnforcesRecipientForeignKeyAndCascade)
{
	const std::int64_t message_id = CreateMessage();
	const std::int64_t recipient_id =
		m_recipient_repository->CreateRecipient(
			message_id,
			"recipient@example.com",
			Storage::RecipientType::To
		);

	EXPECT_THROW(
		m_recipient_repository->CreateRecipient(
			message_id + 1,
			"invalid@example.com",
			Storage::RecipientType::To
		),
		std::runtime_error
	);

	m_database->Execute(
		"DELETE FROM mail_messages WHERE id = " +
		std::to_string(message_id) +
		";"
	);

	EXPECT_FALSE(m_recipient_repository->FindById(recipient_id).has_value());
}

TEST_F(MailDeliveryRepositoryTest, ClaimsQueuedRecipientsOnlyOnce)
{
	const std::int64_t message_id = CreateMessage();
	const std::int64_t first_recipient_id =
		m_recipient_repository->CreateRecipient(
			message_id,
			"first@example.com",
			Storage::RecipientType::To,
			Storage::DeliveryStatus::Queued
		);
	const std::int64_t second_recipient_id =
		m_recipient_repository->CreateRecipient(
			message_id,
			"second@example.com",
			Storage::RecipientType::To,
			Storage::DeliveryStatus::Queued
		);
	m_recipient_repository->CreateRecipient(
		message_id,
		"pending@example.com",
		Storage::RecipientType::To
	);

	const std::vector<Storage::MessageRecipientRecord> first_claim =
		m_recipient_repository->ClaimReadyRecipients(1);

	ASSERT_EQ(first_claim.size(), 1);
	EXPECT_EQ(first_claim[0].id, first_recipient_id);
	EXPECT_EQ(
		first_claim[0].delivery_status,
		Storage::DeliveryStatus::Delivering
	);
	EXPECT_EQ(first_claim[0].attempt_count, 1);

	Storage::Database second_database(m_database_path);
	Storage::MessageRecipientRepository second_repository(second_database);

	const std::vector<Storage::MessageRecipientRecord> second_claim =
		second_repository.ClaimReadyRecipients(10);

	ASSERT_EQ(second_claim.size(), 1);
	EXPECT_EQ(second_claim[0].id, second_recipient_id);
	EXPECT_TRUE(second_repository.ClaimReadyRecipients(10).empty());
}

TEST_F(MailDeliveryRepositoryTest, RetriesTemporaryFailuresWhenReady)
{
	const std::int64_t message_id = CreateMessage();
	const std::int64_t recipient_id =
		m_recipient_repository->CreateRecipient(
			message_id,
			"recipient@example.com",
			Storage::RecipientType::To,
			Storage::DeliveryStatus::Queued
		);

	std::vector<Storage::MessageRecipientRecord> claimed_recipients =
		m_recipient_repository->ClaimReadyRecipients(1);
	ASSERT_EQ(claimed_recipients.size(), 1);

	EXPECT_TRUE(
		m_recipient_repository->MarkTemporaryFailed(
			recipient_id,
			"2999-01-01 00:00:00",
			"temporary failure"
		)
	);
	EXPECT_TRUE(m_recipient_repository->ClaimReadyRecipients(1).empty());
	EXPECT_TRUE(m_recipient_repository->QueueRecipient(recipient_id));

	claimed_recipients =
		m_recipient_repository->ClaimReadyRecipients(1);

	ASSERT_EQ(claimed_recipients.size(), 1);
	EXPECT_EQ(claimed_recipients[0].id, recipient_id);
	EXPECT_EQ(claimed_recipients[0].attempt_count, 2);
	EXPECT_FALSE(claimed_recipients[0].next_attempt_at.has_value());
	EXPECT_FALSE(claimed_recipients[0].last_error.has_value());

	EXPECT_TRUE(m_recipient_repository->MarkDelivered(recipient_id));
	EXPECT_FALSE(m_recipient_repository->MarkDelivered(recipient_id));

	const std::optional<Storage::MessageRecipientRecord> recipient =
		m_recipient_repository->FindById(recipient_id);

	ASSERT_TRUE(recipient.has_value());
	EXPECT_EQ(
		recipient->delivery_status,
		Storage::DeliveryStatus::Delivered
	);
	EXPECT_TRUE(recipient->delivered_at.has_value());
	EXPECT_FALSE(recipient->last_error.has_value());
}

TEST_F(MailDeliveryRepositoryTest, MarksRecipientsBouncedAndFailed)
{
	const std::int64_t message_id = CreateMessage();
	const std::int64_t bounced_recipient_id =
		m_recipient_repository->CreateRecipient(
			message_id,
			"bounced@example.com",
			Storage::RecipientType::To,
			Storage::DeliveryStatus::Queued
		);
	const std::int64_t failed_recipient_id =
		m_recipient_repository->CreateRecipient(
			message_id,
			"failed@example.com",
			Storage::RecipientType::To,
			Storage::DeliveryStatus::Queued
		);

	const std::vector<Storage::MessageRecipientRecord> claimed_recipients =
		m_recipient_repository->ClaimReadyRecipients(10);
	ASSERT_EQ(claimed_recipients.size(), 2);

	EXPECT_TRUE(
		m_recipient_repository->MarkBounced(
			bounced_recipient_id,
			"mailbox unavailable"
		)
	);
	EXPECT_TRUE(
		m_recipient_repository->MarkFailed(
			failed_recipient_id,
			"internal delivery failure"
		)
	);

	const std::optional<Storage::MessageRecipientRecord> bounced_recipient =
		m_recipient_repository->FindById(bounced_recipient_id);
	const std::optional<Storage::MessageRecipientRecord> failed_recipient =
		m_recipient_repository->FindById(failed_recipient_id);

	ASSERT_TRUE(bounced_recipient.has_value());
	ASSERT_TRUE(failed_recipient.has_value());
	EXPECT_EQ(
		bounced_recipient->delivery_status,
		Storage::DeliveryStatus::Bounced
	);
	EXPECT_EQ(bounced_recipient->last_error, "mailbox unavailable");
	EXPECT_EQ(
		failed_recipient->delivery_status,
		Storage::DeliveryStatus::Failed
	);
	EXPECT_EQ(failed_recipient->last_error, "internal delivery failure");
}

}
