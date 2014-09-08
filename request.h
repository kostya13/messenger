#pragma once

#include <string>

namespace Client
{
    std::string SendRequest(const std::string& host, int port, int proto, const std::string& message);
    std::string DescribeReply(const std::string& reply);
}
