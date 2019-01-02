#pragma once
#include "core/common.h"
#include "core/debug.h"
#include "core/utility/utility.h"
#include "block.h"

namespace Ant {
namespace core {
namespace memory {

template<size_t PAGE_SIZE>
struct StackAllocator {
    private:
        void* stack_ptr;
        byte memory[PAGE_SIZE];
        size_t remaining_space;

    public:
        StackAllocator() : stack_ptr(memory), remaining_space(sizeof(memory)) {}
        ~StackAllocator() { ASSERT(stack_ptr == memory && remaining_space == PAGE_SIZE, "Memory leak in stack allocator"); }

        StackAllocator(const StackAllocator& other) = delete;
        StackAllocator(StackAllocator&& other) = delete;

        StackAllocator& operator = (const StackAllocator& other) = delete;
        StackAllocator& operator = (StackAllocator&& other) = delete;

        inline bool owns (const Block& blk) const;
        inline size_t get_size () const;
        inline size_t get_space () const;

        Block allocate (size_t size, byte alignment);
        void deallocate (Block& blk);
        void deallocateAll ();
};

}
}
}
