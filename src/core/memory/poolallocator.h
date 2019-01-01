#pragma once

#include "core/common.h"
#include "core/debug/assert.h"
#include "core/memory/block.h"
#include "core/utility/utility.h"

namespace Ant {
namespace core {
namespace memory {

template<size_t PAGE_SIZE, size_t BLOCK_SIZE>
struct PoolAllocator {
    private:
        void* memory;
        void** free_list;

    private:
        void CreateFreeList ();

    public:
        PoolAllocator();
        ~PoolAllocator();

        inline bool owns (const Block& blk);
        Block allocate (size_t size, byte alignment);
        void deallocate (Block& blk);
		void deallocateAll ();
};

}
}
}
