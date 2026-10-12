#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));

static uint32_t* __stdcall NetPlayerDdl_ReadIntoSlot(
    int /*a1*/, int slot, int /*a3*/, C_008622e8* object, int /*a5*/)
{
    return NetCogibleDdl_ReadIntoSlot(object, nullptr, slot);
}

HOOK(0x00745680, NetPlayerDdl_ReadIntoSlot, "auto");
