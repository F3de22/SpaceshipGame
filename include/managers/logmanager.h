#pragma once

namespace SpaceEngine
{
    class LogManager
    {
        public:
            LogManager() = default;
            ~LogManager() = default;

            void Initialize();
            void Shutdown();
    };
}