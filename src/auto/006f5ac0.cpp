#include <cstddef>
#include "globals.h"
#include <cstdint>

#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, sub_6C4490, 0x006C4490, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_6C35C0, 0x006C35C0, (void* self, void* edx, int a2, int a3));

// Allocates a 52-byte projection block, initializes it through sub_6C4490,
// applies the 600/0 setup call when initialization succeeded, and marks the
// block with state 3 at +0x1C. When allocation fails, the original still writes
// 3 to address 0x1C (eax is zero) and returns 0; the same unconditional write
// happens when initialization returns null.
static int __cdecl GfxCameraProjection_Create()
{
    void* block = Mem_Alloc(52);
    if (block)
    {
        int projection = sub_6C4490(block, nullptr);
        if (projection)
            sub_6C35C0(reinterpret_cast<void*>(projection), nullptr, 0x258, 0);
        CSF_AT(int, reinterpret_cast<void*>(projection), 0x1C) = 3;
        return projection;
    }

    // The original writes to absolute address 0x1C (eax == 0 on this path).
    CSF_AT(int, static_cast<void*>(nullptr), 0x1C) = 3;
    return 0;
}

HOOK(0x006F5AC0, GfxCameraProjection_Create, "auto");
