#include "stackallocator.h"


namespace Ant {
namespace core {
namespace memory {

template<size_t PAGE_SIZE>
inline bool StackAllocator<PAGE_SIZE>::owns (const Block& blk) const {
    return this->memory < blk.ptr && blk.ptr < this->memory + this->remaining_space;
}

template<size_t PAGE_SIZE>
inline size_t StackAllocator<PAGE_SIZE>::get_size () const {
    return static_cast<size_t>((reinterpret_cast<byte*>(stack_ptr) + remaining_space) - memory);
}

template<size_t PAGE_SIZE>
inline size_t StackAllocator<PAGE_SIZE>::get_space () const {
    return remaining_space;
}

template<size_t PAGE_SIZE>
Block StackAllocator<PAGE_SIZE>::allocate (size_t _size, byte _alignment) {
    Block blk{nullptr, 0, 0};
    size_t prev_remaining_space = remaining_space;

    if (align_std(_alignment, _size, stack_ptr, remaining_space) != nullptr) {
        blk = {stack_ptr, _size, static_cast<byte>(prev_remaining_space - remaining_space)};
        stack_ptr = reinterpret_cast<byte*>(stack_ptr) + _size;
        remaining_space -= _size;
    }
    
    return blk;
}

template<size_t PAGE_SIZE>
void StackAllocator<PAGE_SIZE>::deallocate (Block&& blk) {
    if (reinterpret_cast<byte*>(blk.ptr) + blk.size == stack_ptr) {
        stack_ptr = reinterpret_cast<byte*>(stack_ptr) - (blk.adjustment + blk.size);
        remaining_space += blk.size + blk.adjustment;
        blk = {nullptr, 0, 0};
    } 
}

template<size_t PAGE_SIZE>
void StackAllocator<PAGE_SIZE>::deallocateAll () {
    remaining_space += static_cast<size_t>(reinterpret_cast<byte*>(stack_ptr) - memory);
    stack_ptr = memory;
}

}
}
}
