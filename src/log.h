#pragma once

#include <iostream>

class NumberLog {
public:
    NumberLog()
        : oldState_(nullptr)
    {
        oldState_.copyfmt(std::cout);
        std::cout.imbue(std::locale(std::locale::classic(), NumberLog::thousandSeparator_));
    }

    ~NumberLog() {
        std::cout.copyfmt(oldState_);
    }

    std::ostream &out() {
        return std::cout;
    }

private:
    std::ios oldState_;
    static std::numpunct<char>* thousandSeparator_;
};
