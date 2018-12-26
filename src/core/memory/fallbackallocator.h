#pragma once
#include "core/common.h"
#include "core/utility/utility.h"
#include "block.h"

namespace Ant {
namespace core {
namespace memory {

template<typename Primary, typename Fallback>
struct FallbackAllocator : private Primary, private Fallback {
	public:
		bool owns (const Block& blk);

		Block allocate (size_t size, size_t alignment);
		void deallocate (Block&& blk);
};

}
}
}
