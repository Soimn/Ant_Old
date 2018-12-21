#pragma once
#include "core/common.h"

namespace Ant {
namespace core {
namespace memory {

struct Block {
    void* ptr; // pointer to memory block
    size_t size; // size of usable memory in block
    byte adjustment; // adjustment made when allocating the block
};

}
}
}
