#pragma once

#include "core/common.h"
#include "core/utility/utility.h"

namespace Ant {
namespace core {
namespace memory {

struct Block {
    void* ptr; // pointer to memory block
    size_t size; // size of usable memory in block
    byte adjustment; // adjustment made when allocating the block

	bool operator == (const Block& other) const {
		return (this->ptr == other.ptr &&
				this->size == other.size &&
				this->adjustment == other.adjustment);
	}

	bool operator != (const Block& other) const {
		return !(*this == other);
	}

	void swap(Block& other) {
		Block tmp = move(*this);
		*this = move(other);
		other = move(tmp);
	}

};

constexpr const Block nullblock = {nullptr, 0, 0};

}
}
}
