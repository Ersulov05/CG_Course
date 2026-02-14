#pragma once
#include "./Word.h"

class WordGenerator
{
public:
    static Word GenerateWord()
    {
        return Word(L"слово", L"подсказка");
    }
};