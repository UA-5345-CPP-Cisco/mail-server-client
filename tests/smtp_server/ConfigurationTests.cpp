#include "smtp/Configuration.hpp"

#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <string>

namespace {

class ConfigurationTest : public testing::Test {
protected:
	void TearDown() override
	{
		std::remove(path_.c_str());
	}

	void Write(const std::string& json) const
	{
		std::ofstream file(path_);
		file << json;
	}

	const std::string path_{"smtp-configuration-test.json"};
};

TEST_F(ConfigurationTest, LoadsAllConfigurationSections)
{
	Write(R"({
		"database": {
			"storage_path": "test-mail.db",
			"migrations_path": "test-migrations"
		},
		"server": {
			"host": "127.0.0.1",
			"port": 2525,
			"server_name": "mail.test",
			"max_connections": 64,
			"max_message_size_bytes": 4096,
			"require_authentication": true,
			"allow_plain_authentication_without_tls": true
		},
		"delivery": {
			"batch_size": 16,
			"poll_interval_ms": 250
		},
		"tls": {
			"enabled": true,
			"certificate_path": "server.crt",
			"private_key_path": "server.key"
		}
	})");

	const smtp::Configuration configuration(path_);

	EXPECT_EQ(configuration.Database().storagePath, "test-mail.db");
	EXPECT_EQ(configuration.Database().migrationsPath, "test-migrations");
	EXPECT_EQ(configuration.Server().host, "127.0.0.1");
	EXPECT_EQ(configuration.Server().port, 2525);
	EXPECT_EQ(configuration.Server().serverName, "mail.test");
	EXPECT_EQ(configuration.Server().maxConnections, 64);
	EXPECT_EQ(configuration.Server().maxMessageSizeBytes, 4096);
	EXPECT_TRUE(configuration.Server().requireAuthentication);
	EXPECT_TRUE(configuration.Server().allowPlainAuthenticationWithoutTls);
	EXPECT_EQ(configuration.Server().delivery.batchSize, 16);
	EXPECT_EQ(
		configuration.Server().delivery.pollIntervalMilliseconds,
		250);
	EXPECT_TRUE(configuration.Tls().enabled);
	EXPECT_EQ(configuration.Tls().certificatePath, "server.crt");
	EXPECT_EQ(configuration.Tls().privateKeyPath, "server.key");
}

TEST_F(ConfigurationTest, UsesDefaultsForMissingValues)
{
	Write("{}");

	const smtp::Configuration configuration(path_);

	EXPECT_EQ(configuration.Database().storagePath, "mail-storage");
	EXPECT_EQ(
		configuration.Database().migrationsPath,
		"libs/storage/migrations");
	EXPECT_EQ(configuration.Server().host, "0.0.0.0");
	EXPECT_EQ(configuration.Server().port, 25);
	EXPECT_EQ(configuration.Server().delivery.batchSize, 32);
	EXPECT_EQ(
		configuration.Server().delivery.pollIntervalMilliseconds,
		100);
	EXPECT_FALSE(configuration.Tls().enabled);
}

TEST_F(ConfigurationTest, RejectsPortOutsideValidRange)
{
	Write(R"({
		"server": {
			"port": 70000
		}
	})");

	EXPECT_THROW((void)smtp::Configuration{path_}, std::runtime_error);
}

TEST_F(ConfigurationTest, RequiresTlsFilesWhenTlsIsEnabled)
{
	Write(R"({
		"tls": {
			"enabled": true
		}
	})");

	EXPECT_THROW((void)smtp::Configuration{path_}, std::runtime_error);
}

TEST_F(ConfigurationTest, ReportsMalformedJson)
{
	Write("{");

	EXPECT_THROW((void)smtp::Configuration{path_}, std::runtime_error);
}

}
