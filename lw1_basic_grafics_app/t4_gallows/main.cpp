#include "./src/view/GallowsView.h"
#include "./src/model/Gallows.h"
#include <iostream>
#include <cwctype>

int main()
{
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
    std::wcout.imbue(std::locale("ru_RU.UTF-8"));
    try
    {
        Gallows gallowsModel;
        GallowsView gallowsView(gallowsModel);
        gallowsView.Run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}