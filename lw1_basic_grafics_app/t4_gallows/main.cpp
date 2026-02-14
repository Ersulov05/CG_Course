#include "./src/view/GallowsView.h"
#include "./src/model/Gallows.h"
#include <iostream>

int main()
{
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