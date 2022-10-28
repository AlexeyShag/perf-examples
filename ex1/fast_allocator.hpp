#pragma once

#include <algorithm>
#include <cassert>

template<size_t __block_size>
class fixed_allocator_t {
    struct block_t {
        block_t* next = nullptr;
    };

    static constexpr const size_t block_size = std::max<size_t>(__block_size, 8);

public:
    static fixed_allocator_t* instance() {
        static fixed_allocator_t inst;
        return &inst;
    }

    char* allocate() {
        if(first_block_) {
            block_t* block = first_block_;
            first_block_ = block->next;
            return reinterpret_cast<char* >(block);
        } else {
            id_ += block_size;
            return reinterpret_cast<char*>(&buffer_[id_ - block_size]);
        }
    }

    void deallocate(void* p) {
        block_t* ptr = reinterpret_cast<block_t* >(p);
        ptr->next = first_block_;
        first_block_ = ptr;
    }

private:
    fixed_allocator_t() = default;

private:
    static constexpr const size_t __buffer_size = 100'000'000;
    char buffer_[__buffer_size];
    block_t* first_block_ = nullptr;
    uint64_t id_{0};
    

};

template<typename T>
class fast_allocator_t {
public:
    template<class U>
    struct rebind {
        using other = fast_allocator_t<U>;
    };

    T* allocate(size_t n) const {
        assert(n == 1);
        return reinterpret_cast<T*>(fixed_allocator_t<__obi_size>::instance()->allocate());
    }

    template<typename... args_t>
    void construct(T* p, args_t&&... args) const {
        new(p) T(std::forward<args_t>(args)...);
    }

    void destroy(T* p) const {
        p->T();
    }

    void deallocate(T* p, size_t n) const {
        assert(n == 1);
        return fixed_allocator_t<__obi_size>::instance()->deallocate(p);
    }

private:
    static constexpr const size_t __obi_size = std::max(sizeof(T), alignof(T));
};