#include "./src/model/AlhimyModel.h"
#include "./src/view/AlhimyView.h"
#include <iostream>
#include <cwctype>

int main()
{
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
    std::wcout.imbue(std::locale("ru_RU.UTF-8"));
    try
    {
        AlhimyModel alhimyModel;
        AlhimyView alhimyView(alhimyModel);
        alhimyView.Run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}