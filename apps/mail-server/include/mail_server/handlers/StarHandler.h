#pragma once

#include <mail_server/Common.h>

namespace ISXMailServer {

Response StarHandler(Request const& request);
Response UnstarHandler(Request const& request);

} // namespace ISXMailServer
