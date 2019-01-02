#pragma once
#include "core/common.h"
#include "core/debug.h"
#include "core/utility/utility.h"
#include "block.h"

namespace Ant {
namespace core {
namespace memory {

struct Mallocator {
	public:
		Block allocate (size_t size, byte alignment);
		void deallocate (Block& blk);
};

}
}
}
