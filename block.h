#pragma once

#include <cstdint>
#include <string>
#include <ostream>

class Block {

public:
    Block(std::uint64_t id);

    friend std::ostream& operator<<(std::ostream& os, const Block& block);

private:
    std::uint64_t id_;
};
