#pragma once
#include "core/common.h"
#include "block.h"
#include "stackallocator.h"
#include "poolallocator.h"
#include "segregator.h"
#include "fallbackallocator.h"
#include "mallocator.h"

namespace Ant {
namespace core {
namespace memory {

namespace {

Segregator<17, FallbackAllocator<PoolAllocator<1024, 16>, Mallocator>, FallbackAllocator<StackAllocator<1024>, Mallocator>> global_allocator;

}

template<typename T>
Block allocate () {
	return global_allocator.allocate(sizeof(T), alignof(T));
}

void deallocate (Block& blk) {
	// assert(blk != nullblock_t);
	global_allocator.deallocate(forward<Block>(blk));
}

template<typename T, typename ...Args>
Block ant_new (Args&& ...args) {
	Block blk = allocate<T>();
	*blk.ptr = T(forward<Args>(args)...);
	return blk;
}

template<typename T>
void ant_delete (Block& blk) {
	// assert(blk != nullblock_t);
	reinterpret_cast<T*>(blk.ptr).~T();
	deallocate(blk);
}

}
}
}
