#pragma once
#include <iomanip>
#include <iostream>

using Color = u_int32_t;

static std::string ColorToString(const Color color)
{
    std::ostringstream oss;
    oss << "#" << std::hex << std::setw(6) << std::setfill('0') << color;

    return oss.str();
}

static Color StringToColor(const std::string &color)
{
    return std::stoul("0x" + color, nullptr, 16);
}
