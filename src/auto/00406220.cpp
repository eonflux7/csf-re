#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

// Resolves a matrix through a chain of virtual lookups on the object held at self+8
// and writes the result into the output block at outAddr.
// Output block: [0] = matrix pointer (uint32), [4] = owned flag (byte).
// Returns outAddr.
static std::uint32_t __fastcall MatrixSource_ResolveInto(void* self, void* /*edx*/,
                                                         std::uint32_t outAddr, std::uint32_t a3) {
    void* owner = CSF_AT(void*, self, 0x08);

    // v3 = owner->vtbl[10]()
    void* v3 = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[10])(owner, nullptr);

    // v4 = v3->vtbl[26]()
    void* v4 = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, v3, 0)[26])(v3, nullptr);

    // v5 = v4->vtbl[26](a3)
    void* v5 = ((void* (__fastcall*)(void*, void*, std::uint32_t))CSF_AT(void**, v4, 0)[26])(v4, nullptr, a3);

    // v6 = v4->vtbl[27](v5)
    void* v6 = ((void* (__fastcall*)(void*, void*, void*))CSF_AT(void**, v4, 0)[27])(v4, nullptr, v5);

    if (v6) {
        // v6->vtbl[7](outAddr)
        ((void (__fastcall*)(void*, void*, std::uint32_t))CSF_AT(void**, v6, 0)[7])(v6, nullptr, outAddr);
    } else {
        std::uint8_t* out = reinterpret_cast<std::uint8_t*>(outAddr);
        *reinterpret_cast<std::uint32_t*>(out) = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(RwMatrixCreate()));
        out[4] = 1;
    }
    return outAddr;
}

HOOK(0x00406220, MatrixSource_ResolveInto, "auto");
