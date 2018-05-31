#pragma once

#include <chrono>
#include <iostream>
#include <string>

#include "log.h"

class ScopedBenchmark {
public:
    ScopedBenchmark(std::string title)
        : start_(std::chrono::steady_clock::now())
        , title_(title)
    {}

    ~ScopedBenchmark() {
        auto diffMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_);
        NumberLog().out() << title_ << " finished in "
                          << diffMs.count() << " ms"
                          << std::endl;
    }

private:
    std::chrono::steady_clock::time_point start_;
    std::string title_;
};
