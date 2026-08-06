#pragma once

#include <mail_server/Common.h>

namespace ISXMailServer {

Response ReadHandler(Request const& request);

Response UnreadHandler(Request const& request);

} // namespace ISXMailServer
