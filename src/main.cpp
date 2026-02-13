#ifdef _WIN32
#include <Windows.h>
#endif

#include <iostream>
#include <filesystem>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#elif defined(__linux__)
#include <unistd.h>
#endif

static std::filesystem::path getExecutablePath()
{
#ifdef _WIN32
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    return std::filesystem::path(buffer);
#elif defined(__APPLE__)
    uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size);
    std::string buffer(size, '\0');
    _NSGetExecutablePath(buffer.data(), &size);
    return std::filesystem::canonical(buffer);
#elif defined(__linux__)
    char buffer[512];
    ssize_t len = readlink("/proc/self/exe", buffer, 511);
    buffer[len] = '\0';
    return std::filesystem::path(buffer);
#else
#error "unsupported platform"
#endif
}

#include "app.h"

#ifdef _WIN32
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#else
int main()
#endif
{
    std::filesystem::current_path(getExecutablePath().parent_path());
#if defined(_WIN32) && defined(SE_DEBUG)
    AllocConsole();
#endif

    // setta current working directory alla directory che contiene l'eseguibile

    std::cout << "Launcher started..." << std::endl;

    try
    {
        SpaceEngine::App app;
        app.Run();
    }
    catch (const std::exception &e)
    {
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