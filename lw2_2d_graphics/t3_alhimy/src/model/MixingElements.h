#pragma once
#include <optional>
#include <vector>
#include "./Element.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <exception>
#include <codecvt>

const std::string ELEMENTS_FILENAME = "elements.json";

struct MixElement
{
    Element element;
    std::wstring firstElementName;
    std::wstring secondElementName;

    MixElement(const Element &element, const std::wstring &firstElementName, const std::wstring &secondElementName)
        : element(element), firstElementName(firstElementName), secondElementName(secondElementName) {}
};

class MixingElements
{
public:
    MixingElements()
    {
        LoadElementsFromJson(ELEMENTS_FILENAME);
    }

    std::vector<Element> MixElements(const std::wstring &firstElementName, const std::wstring &secondElementName) const
    {
        std::vector<Element> createdElements;
        for (const auto &mixElement : m_mixElements)
        {
            if (
                (mixElement.firstElementName == firstElementName || mixElement.firstElementName == secondElementName) &&
                (mixElement.secondElementName == firstElementName || mixElement.secondElementName == secondElementName))
            {
                createdElements.push_back(mixElement.element);
            }
        }

        return createdElements;
    }

    std::vector<Element> GetDefaultElements() const
    {
        return m_defaultElements;
    }

    unsigned GetElementsCount() const
    {
        return m_elementsCount;
    }

private:
    std::vector<Element> m_defaultElements;
    std::vector<MixElement> m_mixElements;
    unsigned m_elementsCount = 0;

    static std::wstring Utf8ToWstring(const std::string &str)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(str);
    }

    void LoadElementsFromJson(const std::string &filename)
    {
        std::set<std::string> allElements;
        std::ifstream file(filename);
        if (!file.is_open())
        {
            return;
        }

        try
        {
            nlohmann::json jsonData;
            file >> jsonData;

            for (const auto &item : jsonData["defaultElements"])
            {
                std::string elementNameStr = item["name"];
                std::string filename = item["filename"];
                std::wstring elementName = Utf8ToWstring(elementNameStr);

                m_defaultElements.push_back(Element(elementName, filename));
                allElements.insert(elementNameStr);
            }

            for (const auto &item : jsonData["createdElements"])
            {
                std::string elementNameStr = item["name"];
                std::string firstElementNameStr = item["firstElementName"];
                std::string secondElementNameStr = item["secondElementName"];
                std::string filename = item["filename"];
                std::wstring elementName = Utf8ToWstring(elementNameStr);
                std::wstring firstElementName = Utf8ToWstring(firstElementNameStr);
                std::wstring secondElementName = Utf8ToWstring(secondElementNameStr);

                m_mixElements.push_back(MixElement(Element(elementName, filename), firstElementName, secondElementName));
                allElements.insert(elementNameStr);
            }
        }
        catch (const std::exception &e)
        {
        }

        m_elementsCount = allElements.size();
    }
};