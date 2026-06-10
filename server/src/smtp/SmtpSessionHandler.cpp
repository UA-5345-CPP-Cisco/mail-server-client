#include "smtp/Session.hpp"

namespace smtp {

void SmtpSessionHandler::HandleEvent(const SmtpEvent&,
                                     SmtpSessionState&,
                                     SmtpSessionContext&)
{
    // Switch on the event type.
    // Connected should prepare the initial session and send the SMTP greeting.
    // MessageReceived should parse the payload as the next SMTP command or message data.
    // Disconnected should mark the session state closed and release transaction buffers.
    // Any protocol error should send the correct SMTP status code through socketsManager.
}

void SmtpSessionHandler::HandleConnected(const SmtpEvent&,
                                         SmtpSessionState&,
                                         SmtpSessionContext&)
{
    // Initialize the per-session protocol state.
    // Send the server greeting, for example "220 <server-name> Service ready".
    // Keep SMTP-specific decisions here, not in SmtpServer or BoostSocketsManager.
}

void SmtpSessionHandler::HandleMessageReceived(const SmtpEvent&,
                                               SmtpSessionState&,
                                               SmtpSessionContext&)
{
    // Inspect the received payload.
    // If the session is receiving DATA content, append message content until the terminator.
    // Otherwise parse the payload as an SMTP command.
    //
    // Future command handling should cover at least:
    // - HELO/EHLO: record client identity and advertise supported features;
    // - MAIL FROM: start/reset a mail transaction;
    // - RCPT TO: validate and collect recipients;
    // - DATA: switch into message-content mode when transaction state is valid;
    // - RSET: reset the current mail transaction;
    // - NOOP: send a successful no-op reply;
    // - QUIT: send closing reply and close the socket;
    // - STARTTLS: send readiness reply and ask socketsManager to upgrade TLS;
    // - AUTH: delegate authentication to authService.
    //
    // Accepted messages should go through storage, cache if needed, and delivery service.
    // External lookup checks should go through lookupService, not direct API calls.
}

void SmtpSessionHandler::HandleDisconnected(const SmtpEvent&,
                                            SmtpSessionState&,
                                            SmtpSessionContext&)
{
    // Mark the session closed.
    // Drop incomplete mail transaction state.
    // Avoid sending replies because the socket is already gone.
}

void SmtpSessionHandler::ResetMailTransaction(SmtpSessionState&)
{
    // Clear sender, recipients, and buffered message content.
    // Move the session back to the phase that can accept a new MAIL FROM command.
}

void SmtpSessionHandler::SendReply(SmtpSessionContext&,
                                   ConnectionId,
                                   int,
                                   const std::string&)
{
    // Format the numeric SMTP status code and message as one protocol line.
    // Send it through context.socketsManager.Send(connectionId, formattedReply).
    // Keep line-ending details here so callers only provide code and text.
}

}
