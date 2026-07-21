#include "smtp/Session.hpp"

#include <exception>
#include <utility>

namespace smtp {

SmtpSession::SmtpSession(ConnectionId connectionId, SmtpSessionContext context, SmtpSessionHandler& handler) :
  connectionId_(connectionId),
  context_(context),
  handler_(handler),
  state_(connectionId)
{
}

ConnectionId SmtpSession::Connection() const
{
  return connectionId_;
}

void SmtpSession::PushEvent(SmtpEvent event)
{
  events_.push(std::move(event));
}

bool SmtpSession::TryExtractNextTask(std::function<void()>& task)
{
  if (isProcessing_.load())
  {
    return false;
  }

  if (events_.empty())
  {
    return false;
  }

  SmtpEvent event = std::move(events_.front());
  events_.pop();
  isProcessing_.store(true);

  std::shared_ptr<SmtpSession> self = shared_from_this();
  task = [self, event = std::move(event)] { self->ProcessEvent(event); };

  return true;
}

bool SmtpSession::IsReadyForRemoval() const
{
  return isClosed_.load() && !isProcessing_.load() && events_.empty();
}

void SmtpSession::ProcessEvent(const SmtpEvent& event) noexcept
{
  try
  {
    handler_.HandleEvent(event, state_, context_);
  }
  catch (const std::exception& exception)
  {
    context_.logger.Log(Logging::LogLevel::Error, exception.what());
    context_.socketsManager.Close(connectionId_);
  }
  catch (...)
  {
    context_.logger.Log(Logging::LogLevel::Error, "Unhandled SMTP session error");
    context_.socketsManager.Close(connectionId_);
  }

  if (event.type == SmtpEventType::Disconnected)
  {
    isClosed_.store(true);
  }

  if (state_.phase == SmtpSessionPhase::Closed)
  {
    isClosed_.store(true);
  }

  if (event.type != SmtpEventType::Disconnected && state_.phase != SmtpSessionPhase::Closing &&
      state_.phase != SmtpSessionPhase::Closed)
  {
    context_.socketsManager.Continue(connectionId_);
  }

  isProcessing_.store(false);
}

} // namespace smtp
