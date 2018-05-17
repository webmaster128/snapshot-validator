#include "block.h"

Block::Block(uint64_t id)
    : id_(id)
{

}

std::ostream& operator<<(std::ostream& os, const Block& block)
{
    os << block.id_;
    return os;
}
