#pragma once

#include <chrono>
#include <iostream>
#include <string>

class ScopedBenchmark {
public:
    ScopedBenchmark(std::string title)
        : start_(std::chrono::steady_clock::now())
        , title_(title)
    {}

    ~ScopedBenchmark() {
        auto diff = std::chrono::steady_clock::now() - start_;
        std::cout << title_ << " finished in " << std::chrono::duration<double, std::milli>(diff).count() << " ms" << std::endl;
    }

private:
    std::chrono::steady_clock::time_point start_;
    std::string title_;
};
