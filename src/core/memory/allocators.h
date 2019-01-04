#pragma once
#include "core/common.h"
#include "core/debug/assert.h"
#include "block.h"

#include "stackallocator.h"
#include "stackallocator.cpp"

#include "poolallocator.h"
#include "poolallocator.cpp"

#include "segregator.h"
#include "segregator.cpp"

#include "fallbackallocator.h"
#include "fallbackallocator.cpp"

#include "mallocator.h"

namespace Ant {
namespace core {
namespace memory {

namespace {
auto& get_allocator () {
	static Segregator<17, PoolAllocator<1024, 16>, Segregator<65, StackAllocator<1024>, Mallocator>> global_allocator;
	return global_allocator;
}
}

template<typename T>
Block allocate () {
	// lock_guard
	return get_allocator().allocate(sizeof(T), alignof(T));
}

void deallocate (Block& blk) {
	ASSERT(blk != nullblock, "Cannot simply deallocate an uninitialized memory block");
	
	// lock_guard
	get_allocator().deallocate(blk);
}

template<typename T, typename ...Args>
Block ant_new (Args&& ...args) {
	Block blk = allocate<T>();
	*reinterpret_cast<T*>(blk.ptr) = T(forward<Args>(args)...);
	return blk;
}

template<typename T>
void ant_delete (Block& blk) {
	ASSERT(blk != nullblock, "Cannot simply delete an uninitialized memory block");
	reinterpret_cast<T*>(blk.ptr)->~T();
	deallocate(blk);
}

}
}
}
