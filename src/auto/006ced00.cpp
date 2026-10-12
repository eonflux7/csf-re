#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00860c88_ReleaseOwnedState, 0x006CEA20, (void* self, void* edx));
CSF_FUNC(int, __cdecl, VectorRange_Destroy, 0x006CE270, (int a1, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

// Destructor body of C_00860c88 (vector-like buffer owner, base C_00860170).
static void __fastcall C_00860c88_Destroy(C_00860c88* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x00860C88);

    C_00860c88_ReleaseOwnedState(self, nullptr);

    uint32_t bufferBegin = CSF_AT(uint32_t, self, 0x08);
    if (bufferBegin) {
        VectorRange_Destroy(static_cast<int>(bufferBegin),
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

HOOK(0x006CED00, C_00860c88_Destroy, "auto");
