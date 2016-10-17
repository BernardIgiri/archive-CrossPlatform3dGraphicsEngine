#include "stdafx.h"
#include "log.h"

namespace lg
{
    std::string ModeLabel(LogMode m)
    {
        switch (m)
        {
            case ERROR:
            return std::string("Error");
            case MESSAGE:
            return std::string("Message");
            case WARNING:
            return std::string("Warning");
            default:
            break;
        };
        return std::string();
    }
}

using namespace lg;

void Log::FlushEntry()
{
    // flush string stream
    os.flush();
    // if buffer is not empty then log entry
    if (!buffer.empty())
        ofs<<"<Entry caller=\""<<ownerLbl<<"\" level=\""<<ModeLabel(mode)<<"\">"<<buffer<<"</Entry>\n";
    // flush file stream
    ofs.flush();
    // clear buffer for next entry
    buffer.clear();
}

std::string Log::Message() const
{
    return ownerLbl+std::string(" ")+ModeLabel(mode)+std::string(": ")+buffer;
}
