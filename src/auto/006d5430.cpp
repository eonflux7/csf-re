#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CollisionShape_ResetVertexBuffer, 0x006D5300, (void* self, void* edx));
CSF_FUNC(char, __cdecl, CollisionShape_DestroyVertexRange, 0x006D4B40, (int a1, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

// Destructor-style release of C_00861068 (collision shape, base C_00860170).
static void __fastcall C_00861068_ReleaseDerivedState(C_00861068* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x00861068);

    CollisionShape_ResetVertexBuffer(self, nullptr);

    uint32_t rangeBegin = CSF_AT(uint32_t, self, 0x08);
    if (rangeBegin) {
        CollisionShape_DestroyVertexRange(static_cast<int>(rangeBegin),
                                          static_cast<int>(CSF_AT(uint32_t, self, 0x0C)));
        if (CSF_AT(uint32_t, self, 0x08)) {
            Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08)));
        }
    }

    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006D5430, C_00861068_ReleaseDerivedState, "auto");
