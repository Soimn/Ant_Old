#include "poolallocator.h"

namespace Ant {
namespace core {
namespace memory {

template<size_t P, size_t B>
inline bool PoolAllocator<P, B>::owns (const Block& blk) {
    return this->memory < blk.ptr && blk.ptr < reinterpret_cast<byte*>(this->memory) + P;
}

template<size_t P, size_t B>
void PoolAllocator<P, B>::CreateFreeList () {
    byte** ptr = reinterpret_cast<byte**>(align(memory, alignof(void*)));
    size_t block_count = P / B;

    free_list = reinterpret_cast<void**>(ptr);

    for (size_t i = 0; i < block_count - 1; ++i) {
        *ptr = reinterpret_cast<byte*>(ptr) + B;
        ptr = reinterpret_cast<byte**>(*ptr);
    }

    *ptr = nullptr;
}

template<size_t P, size_t B>
PoolAllocator<P, B>::PoolAllocator () : memory(nullptr), free_list(nullptr) {
    memory = malloc(1024);

    if (memory == nullptr)
        puts("ERROR");

    CreateFreeList();
}

template<size_t P, size_t B>
PoolAllocator<P, B>::~PoolAllocator () {
    free(memory);
}

template<size_t P, size_t B>
Block PoolAllocator<P, B>::allocate (size_t size, byte alignment) {
    Block blk{nullptr, 0, 0};

    if (free_list != nullptr) {
        byte adjustment = static_cast<byte>(align(free_list, alignment) - reinterpret_cast<byte*>(free_list));

        if (adjustment + size <= B) {
            blk.ptr = reinterpret_cast<void*>(free_list);
            blk.size = size;
            blk.adjustment = adjustment;
            free_list = reinterpret_cast<void**>(*free_list);
        }
    }

    return blk;
}

template<size_t P, size_t B>
void PoolAllocator<P, B>::deallocate (Block& blk) {
	ASSERT(blk != nullblock_t, "Cannot simply deallocate an uninitialized memory block");
    *(reinterpret_cast<void**>(blk.ptr)) = reinterpret_cast<void*>(free_list);
    free_list = reinterpret_cast<void**>(blk.ptr);

    blk = {nullptr, 0, 0};
}

template<size_t P, size_t B>
void PoolAllocator<P, B>::deallocateAll () {
	CreateFreeList();
}

}
}
}
