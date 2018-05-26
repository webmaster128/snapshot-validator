#include "log.h"

namespace {

class ThousandSeparator : public std::numpunct<char>
{
protected:
    virtual char do_thousands_sep() const { return ','; }
    virtual std::string do_grouping() const { return "\03"; }
};

}

std::numpunct<char>* NumberLog::thousandSeparator_ = new ThousandSeparator;
