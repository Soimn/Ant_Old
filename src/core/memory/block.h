#pragma once
#include "core/common.h"

namespace Ant {
namespace core {
namespace memory {

struct Block {
    void* ptr; // pointer to memory block
    size_t size; // size of usable memory in block
    byte adjustment; // adjustment made when allocating the block

	operator bool() const { return (ptr == nullptr ? false : true); }
	bool operator == (const Block& other) const {
		return (this->ptr == other.ptr &&
				this->size == other.size &&
				this->adjustment == other.adjustment);
	}

	bool operator != (const Block& other) const {
		return !(*this == other);
	}
};

constexpr const Block nullblock_t = {nullptr, 0, 0};

}
}
}
