#include <iostream>
#include "app.h"

int main()
{
    std::cout << "Launcher started..." << std::endl;
    
    try 
    {
        // Qui c'è il tuo codice originale, ad esempio:
        SpaceEngine::App app;
        app.Run();
    }
    catch (const std::exception& e) 
    {
        // Questo catturerà errori standard (es. std::vector out of range, filesystem error, ecc.)
        std::cout << "CRITICAL ERROR: " << e.what() << std::endl;
        std::cin.get(); // Aspetta un tasto prima di chiudere
        return -1;
    }
    catch (...)
    {
        std::cout << "UNKNOWN CRITICAL ERROR OCCURRED" << std::endl;
        std::cin.get();
        return -1;
    }

    return 0;
}