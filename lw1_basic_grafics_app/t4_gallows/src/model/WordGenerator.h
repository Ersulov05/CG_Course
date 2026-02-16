#pragma once
#include "./Word.h"
#include <random>
#include <ctime>
#include <nlohmann/json.hpp>
#include <fstream>
#include <codecvt>

const std::string WORDS_FILENAME = "words.json";

class WordGenerator
{
public:
    static Word GenerateWord()
    {
        static const std::vector<std::pair<std::wstring, std::wstring>> wordDatabase = LoadWordsFromJson(WORDS_FILENAME);

        static std::mt19937 generator(static_cast<unsigned>(std::time(nullptr)));
        static std::uniform_int_distribution<> distribution(0, wordDatabase.size() - 1);

        int index = distribution(generator);
        const auto &[word, hint] = wordDatabase[index];

        return Word(word, hint);
    }

private:
    static std::vector<std::pair<std::wstring, std::wstring>> LoadWordsFromJson(const std::string &filename)
    {
        std::vector<std::pair<std::wstring, std::wstring>> words;

        std::ifstream file(filename);
        if (!file.is_open())
        {
            return GetDefaultWords();
        }

        try
        {
            nlohmann::json jsonData;
            file >> jsonData;

            for (const auto &item : jsonData["words"])
            {
                std::string wordStr = item["word"];
                std::string hintStr = item["hint"];

                std::wstring word = Utf8ToWstring(wordStr);
                std::wstring hint = Utf8ToWstring(hintStr);

                words.emplace_back(word, hint);
            }
        }
        catch (const std::exception &e)
        {
            return GetDefaultWords();
        }

        return words;
    }

    static std::wstring Utf8ToWstring(const std::string &str)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(str);
    }

    static std::vector<std::pair<std::wstring, std::wstring>> GetDefaultWords()
    {
        std::cout << "default\n";
        return {
            {L"ПРОГРАММА", L"Набор инструкций для компьютера"},
            {L"КОМПЬЮТЕР", L"Электронное устройство для обработки данных"},
            {L"ВИСЕЛИЦА", L"Игра в угадывание слов"},
            {L"АЛФАВИТ", L"Набор букв в определенном порядке"},
            {L"РАЗРАБОТКА", L"Процесс создания программного обеспечения"},
            {L"БИБЛИОТЕКА", L"Место, где хранятся книги"},
            {L"УНИВЕРСИТЕТ", L"Высшее учебное заведение"},
            {L"ПРИРОДА", L"Окружающий нас мир"},
            {L"ПУТЕШЕСТВИЕ", L"Поездка в другие места"},
            {L"МАТЕМАТИКА", L"Наука о числах и формулах"},
            {L"ФУТБОЛ", L"Популярная командная игра с мячом"},
            {L"МУЗЫКА", L"Искусство звуков"},
            {L"ХУДОЖНИК", L"Человек, создающий произведения искусства"},
            {L"РЕСТОРАН", L"Место, где подают еду"},
            {L"БОЛЬНИЦА", L"Медицинское учреждение"},
            {L"САМОЛЕТ", L"Воздушное транспортное средство"},
            {L"ПОЕЗД", L"Железнодорожный транспорт"},
            {L"ТЕЛЕФОН", L"Устройство для связи"},
            {L"ФОТОГРАФИЯ", L"Изображение, полученное с помощью камеры"},
            {L"ИНТЕРНЕТ", L"Всемирная компьютерная сеть"}};
    }
};