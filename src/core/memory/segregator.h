#pragma once
#include "core/common.h"
#include "core/debug/assert.h"
#include "core/utility/utility.h"
#include "block.h"

namespace Ant {
namespace core {
namespace memory {

template<size_t threshold, typename SmallAllocator, typename LargeAllocator>
struct Segregator {
	public:
		bool owns (const Block& blk);
		
		Block allocate (size_t size, size_t alignment);
		void deallocate (Block&& blk);
};

}
}
}
