#pragma once
#include <cwctype>
#include <string>

class Element
{
public:
    const std::string IMAGE_DIR_PATH = "./images";

    Element(const std::wstring &name, const std::string &filename = "")
        : m_name(name),
          m_imagePath(filename)
    {
    }

    std::wstring GetName() const
    {
        return m_name;
    }

    std::string GetImagePath() const
    {
        return IMAGE_DIR_PATH + "/" + m_imagePath;
    }

    auto operator<=>(const Element &other) const
    {
        return m_name <=> other.m_name;
    }

private:
    std::string m_imagePath;
    std::wstring m_name;
};