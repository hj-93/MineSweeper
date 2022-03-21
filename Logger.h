#pragma once
#include <algorithm>
#include <iostream>
namespace MineSweeper { 
    class ILogger {
    public:
        virtual void LogInfo(std::string const& row) noexcept = 0;
        virtual ~ILogger() noexcept = default;
    };

    class ConsoleLogger final : public ILogger {
    public:
        void LogInfo(std::string const& row) noexcept override {
            std::cout << row << std::endl;
        }
        ~ConsoleLogger() noexcept final = default;
    };
}
