#include "segregator.h"

namespace Ant {
namespace core {
namespace memory {

template<size_t t, typename S, typename L>
bool Segregator<t, S, L>::owns (const Block& blk) {
	return blk.size > t ? L::owns(blk) : S::owns(blk);
}

template<size_t t, typename S, typename L>
Block Segregator<t, S, L>::allocate (size_t size, size_t alignment) {
	return size > t ? L::allocate(size, alignment) : S::allocate(size, alignment);
}

template<size_t t, typename S, typename L>
void Segregator<t, S, L>::deallocate (Block&& blk) {
	if (blk.size > t)
		L::deallocate(forward<Block>(blk));
	
	else
		S::deallocate(forward<Block>(blk));
}

}
}
}
