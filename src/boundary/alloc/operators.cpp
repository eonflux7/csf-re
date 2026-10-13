// No allocator implementation here: retain the game's pooling/debug allocator.
// Its backing malloc/free address slots are provided by boundary/crt (UCRT /MT).
#ifdef CSF_STANDALONE
#include "core/memory.h"
#include <new>

void* __cdecl operator new(size_t n) {
    void* p = game::Mem_Alloc(n ? n : 1);
    if (!p) throw std::bad_alloc();
    return p;
}
void* __cdecl operator new[](size_t n) { return ::operator new(n); }
void __cdecl operator delete(void* p) noexcept { if (p) game::Mem_Free(p); }
void __cdecl operator delete[](void* p) noexcept { ::operator delete(p); }
void __cdecl operator delete(void* p, size_t) noexcept { ::operator delete(p); }
void __cdecl operator delete[](void* p, size_t) noexcept { ::operator delete(p); }
void* __cdecl operator new(size_t n, const std::nothrow_t&) noexcept {
    return game::Mem_Alloc(n ? n : 1);
}
void* __cdecl operator new[](size_t n, const std::nothrow_t& t) noexcept { return ::operator new(n,t); }
void __cdecl operator delete(void* p, const std::nothrow_t&) noexcept { ::operator delete(p); }
void __cdecl operator delete[](void* p, const std::nothrow_t&) noexcept { ::operator delete(p); }
#endif
