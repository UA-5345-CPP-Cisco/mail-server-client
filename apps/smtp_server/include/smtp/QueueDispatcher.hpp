#pragma once

#include "logger/Logger.h"
#include "smtp/ServerConfig.hpp"
#include "storage/MailMessageRepository.h"
#include "storage/MessageRecipientRepository.h"
#include "storage/UserRepository.h" 
#include "thread_pool/ThreadPool.h"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <mutex>

namespace smtp
{

	class QueueDispatcher
	{
	public:
		QueueDispatcher(
			DeliveryConfig config,
			ThreadPool& threadPool,
			Storage::UserRepository &users,
			Storage::MailMessageRepository &mailMessages,
			Storage::MessageRecipientRepository &messageRecipients,
			std::mutex &storageMutex);

		void Poll();

	private:
		static void Deliver(
			Storage::MessageRecipientRecord recipient,
			Storage::UserRepository &users,
			Storage::MailMessageRepository &mailMessages,
			Storage::MessageRecipientRepository &messageRecipients,
			std::mutex &storageMutex) noexcept;

		DeliveryConfig config_;
		ThreadPool& threadPool_;
		Storage::UserRepository &users_;
		Storage::MailMessageRepository &mailMessages_;
		Storage::MessageRecipientRepository &messageRecipients_;
		std::mutex &storageMutex_;
		std::chrono::steady_clock::time_point nextPoll_{};
	};

}
