#pragma once

#include <mail_server/Common.h>

namespace ISXMailServer {

Response ArchiveHandler(Request const& request);
Response UnarchiveHandler(Request const& request);

} // namespace ISXMailServer
