#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

// Resolves a matrix through a chain of virtual lookups on the object stored at self+8,
// then either delegates the write to the resolved object or creates a fresh matrix.
// Output block at a2: [0] = matrix pointer (uint32), [4] = owned flag (byte).
// Returns a2.
static int __fastcall C_0082d77c_ResolveMatrixInto(C_0082d77c* self, void* /*edx*/, int a2, int a3) {
    void* owner = reinterpret_cast<void*>(self->field_0008);

    // v3 = owner->vtbl[10]()
    void* v3 = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[10])(owner, nullptr);

    // v4 = v3->vtbl[26]()
    void* v4 = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, v3, 0)[26])(v3, nullptr);

    // v5 = v4->vtbl[26](a3)
    void* v5 = ((void* (__fastcall*)(void*, void*, int))CSF_AT(void**, v4, 0)[26])(v4, nullptr, a3);

    // v6 = v4->vtbl[27](v5)
    void* v6 = ((void* (__fastcall*)(void*, void*, void*))CSF_AT(void**, v4, 0)[27])(v4, nullptr, v5);

    if (v6) {
        // v6->vtbl[5](a2)
        ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, v6, 0)[5])(v6, nullptr, a2);
    } else {
        std::uint8_t* out = reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(a2));
        *reinterpret_cast<std::uint32_t*>(out) =
            static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(RwMatrixCreate()));
        out[4] = 1;
    }
    return a2;
}

HOOK(0x00406A40, C_0082d77c_ResolveMatrixInto, "auto");
