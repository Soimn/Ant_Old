#include "mallocator.h"

namespace Ant {
namespace core {
namespace memory {

Block Mallocator::allocate (size_t size, byte alignment) {
	Block blk{nullptr, 0, 0};
	void* memory = malloc(size + alignment - 1);

	if (memory != nullptr) {
		blk.ptr = align(memory, alignment);
		blk.size = size;
	}

	return blk;
}

void Mallocator::deallocate (Block& blk) {
	ASSERT(blk != nullblock_t, "Cannot simply deallocate an uninitialized memory block");
	free(blk.ptr);
	blk = {nullptr, 0, 0};
}

}
}
}
