#include "fallbackallocator.h"

namespace Ant {
namespace core {
namespace memory {

template<typename P, typename F>
bool FallbackAllocator<P, F>::owns (const Block& blk) {
	return P::owns(blk) || F::owns(blk);
}

template<typename P, typename F>
Block FallbackAllocator<P, F>::allocate (size_t size, size_t alignment) {
	Block blk{nullptr, 0, 0};

	if (blk = P::allocate(size, alignment); blk.ptr == nullptr)
		blk = F::allocate(size, alignment);

	return blk;
}

template<typename P, typename F>
void FallbackAllocator<P, F>::deallocate (Block&& blk) {
	ASSERT(blk != nullblock_t, "Cannot simply deallocate an uninitialized memory block");
	if (P::owns(blk))
		P::deallocate(forward<Block>(blk));

	else
		F::deallocate(forward<Block>(blk));
}

}
}
}
