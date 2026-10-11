#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpAtomicSetFrame, 0x0058F300, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwFrameDestroy, 0x00623B10, (int frame));
CSF_FUNC(int, __cdecl, RpPTankAtomicDestroy, 0x005821A0, (uint32_t* a1));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

// Destroys the atomic (and its frame, if any) held in field_0008, resets the
// object's vtable to C_008636ec, runs the base constructor, and frees the
// object's memory when bit 0 of a2 is set (scalar deleting destructor).
static int* __fastcall C_008636ec_DestroyAndOptionallyFree(C_008636ec* self, void* /*edx*/, char a2)
{
    *(void***)self = CSF_VTBL(0x008636EC);

    uint32_t atomicHandle = self->field_0008;
    if (atomicHandle != 0)
    {
        uint8_t* atomicBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(atomicHandle));
        uint32_t frameHandle = CSF_AT(uint32_t, atomicBase, 4);
        if (frameHandle != 0)
        {
            RpAtomicSetFrame(static_cast<int>(atomicHandle), 0);
            RwFrameDestroy(static_cast<int>(frameHandle));
        }
        RpPTankAtomicDestroy(reinterpret_cast<uint32_t*>(self->field_0008));
        self->field_0008 = 0;
    }

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<int*>(self);
}

HOOK(0x0070B650, C_008636ec_DestroyAndOptionallyFree, "auto");
