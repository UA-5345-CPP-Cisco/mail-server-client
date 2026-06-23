#include "smtp/QueueDispatcher.hpp"

#include <exception>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace smtp
{

	QueueDispatcher::QueueDispatcher(
		DeliveryConfig config,
		ThreadPool &threadPool,
		Storage::UserRepository &users,
		Storage::MailMessageRepository &mailMessages,
		Storage::MessageRecipientRepository &messageRecipients,
		std::mutex &storageMutex)
		: config_(config),
		  threadPool_(threadPool),
		  users_(users),
		  mailMessages_(mailMessages),
		  messageRecipients_(messageRecipients),
		  storageMutex_(storageMutex)
	{
		if (config_.batchSize == 0)
		{
			throw std::invalid_argument(
				"QueueDispatcher batch size must be greater than zero");
		}
	}

	void QueueDispatcher::Poll()
	{
		const auto now = std::chrono::steady_clock::now();
		if (now < nextPoll_)
		{
			return;
		}
		nextPoll_ = now + std::chrono::milliseconds(
							  config_.pollIntervalMilliseconds);

		const std::size_t maximumInt =
			static_cast<std::size_t>(std::numeric_limits<int>::max());
		const int limit = static_cast<int>(
			config_.batchSize > maximumInt ? maximumInt : config_.batchSize);

		std::vector<Storage::MessageRecipientRecord> recipients;
		{
			std::lock_guard<std::mutex> lock(storageMutex_);
			recipients = messageRecipients_.ClaimReadyRecipients(limit);

			for (const Storage::MessageRecipientRecord &recipient : recipients)
			{
				mailMessages_.UpdateStatus(
					recipient.message_id,
					Storage::MailMessageStatus::Queued,
					Storage::MailMessageStatus::Sending);
			}
		}
		for (const Storage::MessageRecipientRecord &recipient : recipients)
		{
			try
			{
				threadPool_.Enqueue(
					[recipient,
					 &users = users_,
					 &mailMessages = mailMessages_,
					 &messageRecipients = messageRecipients_,
					 &storageMutex = storageMutex_]
					{
						Deliver(
							recipient,
							users,
							mailMessages,
							messageRecipients,
							storageMutex);
					});
			}
			catch (const std::exception &error)
			{
				std::lock_guard<std::mutex> lock(storageMutex_);
				messageRecipients_.MarkFailed(recipient.id, error.what());
				mailMessages_.FinalizeDelivery(recipient.message_id);
			}
		}
	}

	void QueueDispatcher::Deliver(
		Storage::MessageRecipientRecord recipient,
		Storage::UserRepository &users,
		Storage::MailMessageRepository &mailMessages,
		Storage::MessageRecipientRepository &messageRecipients,
		std::mutex &storageMutex) noexcept
	{
		try
		{
			std::lock_guard<std::mutex> lock(storageMutex);
			const std::optional<Storage::UserRecord> user =
				users.FindByEmail(recipient.recipient_email);

			if (user && user->status == Storage::UserStatus::Active)
			{
				messageRecipients.MarkDelivered(recipient.id);
			}
			else
			{
				messageRecipients.MarkFailed(
					recipient.id,
					"Recipient is not an active local user");
			}

			mailMessages.FinalizeDelivery(recipient.message_id);
		}
		catch (const std::exception &error)
		{
			Logger::GetInstance().LogProd("QueueDispatcher::Deliver", error.what());
			try
			{
				std::lock_guard<std::mutex> lock(storageMutex);
				messageRecipients.MarkFailed(recipient.id, error.what());
				mailMessages.FinalizeDelivery(recipient.message_id);
			}
			catch (...)
			{
			}
		}
		catch (...)
		{
			Logger::GetInstance().LogProd("QueueDispatcher::Deliver", "Unhandled local delivery error");
			try
			{
				std::lock_guard<std::mutex> lock(storageMutex);
				messageRecipients.MarkFailed(
					recipient.id,
					"Unhandled local delivery error");
				mailMessages.FinalizeDelivery(recipient.message_id);
			}
			catch (...)
			{
			}
		}
	}

}