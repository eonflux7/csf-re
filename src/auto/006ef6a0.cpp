#include <cstddef>
#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(int, __fastcall, C_00860894_Init, 0x006C45F0, (C_00860894* self, void*, int a2, int a3));

static int __fastcall C_008621c8_InitializeUvAnimState(C_008621c8* self, void* /*edx*/)
{
    C_00860178_Construct(self, nullptr);
    *(void***)self = CSF_VTBL(0x008621C8);
    CSF_AT(int, self, 0x1C) = 15;
    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(uint8_t, self, 0x08) = 0;
    CSF_AT(int, self, 0x20) = 0;
    CSF_AT(int, self, 0x24) = 0;
    CSF_AT(int, self, 0x28) = 0;
    CSF_AT(int, self, 0x2C) = 0;

    int allocated = CoreMemory_AllocUntagged(28);
    uint32_t v3 = static_cast<uint32_t>(allocated);
    if (v3 != 0) {
        void* block = reinterpret_cast<void*>(v3);
        C_00860178_Construct(reinterpret_cast<C_00860178*>(block), nullptr);
        *(void***)block = CSF_VTBL(0x00862190);
        C_00860894_Init(reinterpret_cast<C_00860894*>(static_cast<uint8_t*>(block) + 8), nullptr, -1, 0);
        CSF_AT(uint32_t, block, 4) = reinterpret_cast<uint32_t>(self);
        CSF_AT(uint32_t, self, 0x40) = v3;
    } else {
        CSF_AT(uint32_t, self, 0x40) = 0;
    }
    CSF_AT(uint8_t, self, 0x3C) = 0;
    CSF_AT(uint8_t, self, 0x3D) = 1;
    return reinterpret_cast<int>(self);
}

HOOK(0x006EF6A0, C_008621c8_InitializeUvAnimState, "auto");
