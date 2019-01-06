#include "segregator.h"

namespace Ant {
namespace core {
namespace memory {

template<size_t t, typename S, typename L>
bool Segregator<t, S, L>::owns (const Block& blk) {
	return blk.size > t ? L::owns(blk) : S::owns(blk);
}

template<size_t t, typename S, typename L>
Block Segregator<t, S, L>::allocate (size_t size, byte alignment) {
	Block blk;

	if (size <= t)
		blk = S::allocate(size, alignment);

	if (blk == nullblock)
		blk = L::allocate(size, alignment);

// 	if (blk == nullblock)
// 		LOGF("Failed to allocate memory");	

	return blk;
}

template<size_t t, typename S, typename L>
void Segregator<t, S, L>::deallocate (Block& blk) {
	ASSERT(blk != nullblock, "Cannot simply deallocate an uninitialized memory block");

	if (blk.size <= t && S::owns(blk))
		S::deallocate(blk);
	
	else
		L::deallocate(blk);
}

}
}
}
