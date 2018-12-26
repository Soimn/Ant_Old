#pragma once
#include "core/common.h"

namespace Ant {
namespace core {
namespace memory {

struct Block {
    void* ptr; // pointer to memory block
    size_t size; // size of usable memory in block
    byte adjustment; // adjustment made when allocating the block

	operator bool() { return (ptr == nullptr ? false : true); }
	bool operator == (const Block& other) {
		return (this->ptr == other.ptr &&
				this->size == other.size &&
				this->adjustment == other.adjustment);
	}
};

constexpr const Block nullblock_t = {nullptr, 0, 0};

struct ArrayBlock {
	Block blk;
	size_t cell_size;
	size_t count;

	operator bool() { return (blk.ptr == nullptr ? false : true); }
	bool operator == (const ArrayBlock& other) {
		return (this->blk.ptr == other.blk.ptr &&
				this->blk.size == other.blk.size &&
				this->blk.adjustment == other.blk.adjustment &&
				this->cell_size == other.cell_size &&
				this->count == other.count);
	}
};

}
}
}
