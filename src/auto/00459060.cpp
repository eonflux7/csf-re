#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callees (bound by address)
CSF_FUNC(int, __fastcall, ReleaseInnerSubobject, 0x00458E40, (void* self, void* edx));
CSF_FUNC(unsigned int, __cdecl, Hud_UnregisterObject, 0x004754D0, (int a1));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall C_008313dc_Destroy(C_008313dc* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x008313DC);
    CSF_AT(void*, self, 0x4) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x008313C0));

    Hud_UnregisterObject(static_cast<int>(reinterpret_cast<uintptr_t>(self)));

    if (self->field_002c) {
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_002c));
    }
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;

    if (self->field_001c) {
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_001c));
    }
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;

    int result = ReleaseInnerSubobject(static_cast<uint8_t*>(static_cast<void*>(self)) + 8, nullptr);

    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x459060, C_008313dc_Destroy, "auto");
