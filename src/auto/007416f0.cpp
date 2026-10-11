#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, ClearListAndFreeNodes, 0x004E9980, (uint32_t* self, void* edx));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void, __fastcall, NetCogible_ReleaseBase890, 0x00740840, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetCogible_BaseDestroy, 0x00741360, (void* self, void* edx));

static int __fastcall C_00867900_Destroy(C_00867900* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00867900);
    self->field_06b0 = (uint32_t)CSF_ADDR_VT(0x008678EC);

    if (self->field_0740 >= 8 && self->field_072c) {
        Mem_FreeDebug((void*)self->field_072c);
    }
    self->field_0740 = 7;
    self->field_073c = 0;
    CSF_AT(uint16_t, self, 0x72C) = 0;

    SceneLoader_NoOp();
    SceneLoader_NoOp();
    SceneLoader_NoOp();

    uint32_t* listB = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x6DC);
    ClearListAndFreeNodes(listB, nullptr);
    if (CSF_AT(uint32_t, self, 0x6E0)) {
        Mem_FreeDebug((void*)CSF_AT(uint32_t, self, 0x6E0));
    }
    CSF_AT(uint32_t, self, 0x6E0) = 0;

    uint32_t* listA = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x6D0);
    ClearListAndFreeNodes(listA, nullptr);
    if (CSF_AT(uint32_t, self, 0x6D4)) {
        Mem_FreeDebug((void*)CSF_AT(uint32_t, self, 0x6D4));
    }
    CSF_AT(uint32_t, self, 0x6D4) = 0;

    NetCogible_ReleaseBase890(reinterpret_cast<uint8_t*>(self) + 0x6B0, nullptr);
    return NetCogible_BaseDestroy(self, nullptr);
}

HOOK(0x007416F0, C_00867900_Destroy, "auto");
