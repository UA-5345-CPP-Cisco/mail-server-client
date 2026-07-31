#pragma once

#include <mail_server/Common.h>

namespace ISXMailServer {

Response LoginHandler(Request const& request);
Response RegisterHandler(Request const& request);

} // namespace ISXMailServer
