#include "smtp/Services.hpp"

#include <gtest/gtest.h>

#include <optional>
#include <vector>

namespace smtp::test {

TEST(ServicesTests, AuthServiceAcceptsKnownUserAndRejectsInvalidCredentials)
{
    AuthService service;
    service.AddUser("Alice@Example.com", "secret");

    const AuthResult accepted = service.Authenticate(AuthRequest{"PLAIN", "alice@example.com", "secret"});
    EXPECT_TRUE(accepted.accepted);
    EXPECT_FALSE(accepted.registered);
    EXPECT_EQ(accepted.identity, "alice@example.com");

    const AuthResult wrongPassword = service.Authenticate(AuthRequest{"PLAIN", "alice@example.com", "wrong"});
    EXPECT_FALSE(wrongPassword.accepted);
    EXPECT_TRUE(wrongPassword.identity.empty());

    const AuthResult unknownUser = service.Authenticate(AuthRequest{"PLAIN", "bob", "secret"});
    EXPECT_TRUE(unknownUser.accepted);
    EXPECT_TRUE(unknownUser.registered);
    EXPECT_EQ(unknownUser.identity, "bob");
}

TEST(ServicesTests, MailStorageSavesMessagesWithStableIncrementingIds)
{
    dbSQLite storage(dbConfig{});
    MailMessage first{"alice@example.com", {"bob@example.com"}, "first"};
    MailMessage second{"carol@example.com", {"dave@example.com"}, "second"};

    const std::string firstId = storage.Save(first);
    const std::string secondId = storage.Save(second);

    EXPECT_EQ(firstId, "1");
    EXPECT_EQ(secondId, "2");

    const std::optional<MailMessage> loadedFirst = storage.Retrieve(firstId);
    ASSERT_TRUE(loadedFirst.has_value());
    EXPECT_EQ(loadedFirst->sender, first.sender);
    EXPECT_EQ(loadedFirst->recipients, first.recipients);
    EXPECT_EQ(loadedFirst->rawContent, first.rawContent);

    EXPECT_FALSE(storage.Retrieve("missing").has_value());
}

TEST(ServicesTests, MailStorageIndexesMessagesByRecipientMailbox)
{
    dbSQLite storage(dbConfig{});
    storage.Save(MailMessage{"alice@example.com", {"bob@example.com"}, "first"});
    storage.Save(MailMessage{"carol@example.com", {"BOB@example.com", "dan@example.com"}, "second"});

    EXPECT_EQ(storage.CountForRecipient("bob@example.com"), 2U);
    EXPECT_EQ(storage.CountForRecipient("dan@example.com"), 1U);
    EXPECT_EQ(storage.CountForRecipient("missing@example.com"), 0U);

    const std::vector<StoredMailMessage> bobMessages =
        storage.ListForRecipient("Bob@Example.com", 1, 2);
    ASSERT_EQ(bobMessages.size(), 2U);
    EXPECT_EQ(bobMessages[0].index, 1U);
    EXPECT_EQ(bobMessages[0].messageId, "1");
    EXPECT_EQ(bobMessages[0].message.rawContent, "first");
    EXPECT_EQ(bobMessages[1].index, 2U);
    EXPECT_EQ(bobMessages[1].messageId, "2");
    EXPECT_EQ(bobMessages[1].message.rawContent, "second");
}

TEST(ServicesTests, CacheServiceStoresOverwritesAndReturnsMissingValues)
{
    CacheService cache;

    EXPECT_FALSE(cache.Get("message:1").has_value());

    cache.Put("message:1", "first");
    std::optional<std::string> first = cache.Get("message:1");
    ASSERT_TRUE(first.has_value());
    EXPECT_EQ(*first, "first");

    cache.Put("message:1", "updated");
    std::optional<std::string> updated = cache.Get("message:1");
    ASSERT_TRUE(updated.has_value());
    EXPECT_EQ(*updated, "updated");
}

TEST(ServicesTests, LookupServiceAllowsByDefaultAndCanAllowOrBlockSpecificKeys)
{
    LookupService lookup;

    LookupResult defaultAllowed = lookup.Lookup(LookupRequest{"EXAMPLE.COM"});
    EXPECT_TRUE(defaultAllowed.found);
    EXPECT_EQ(defaultAllowed.value, "EXAMPLE.COM");

    lookup.Allow("ALLOWED.COM");
    EXPECT_TRUE(lookup.Lookup(LookupRequest{"ALLOWED.COM"}).found);
    EXPECT_FALSE(lookup.Lookup(LookupRequest{"OTHER.COM"}).found);

    lookup.Block("ALLOWED.COM");
    EXPECT_FALSE(lookup.Lookup(LookupRequest{"ALLOWED.COM"}).found);
    EXPECT_FALSE(lookup.Lookup(LookupRequest{""}).found);
}

TEST(ServicesTests, DeliveryServiceAcceptsMessagesForQueueing)
{
    DeliveryService delivery;
    MailMessage message{"alice@example.com", {"bob@example.com"}, "body"};

    EXPECT_NO_THROW(delivery.QueueForDelivery(message));
}

}
