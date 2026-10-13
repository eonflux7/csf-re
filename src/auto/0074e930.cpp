#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, NetPlayerDDL_ResolveObject, 0x00655DF0, (void* self, void* edx, bool* a2));
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));

static uint32_t* __fastcall C_008622e8_ReadResolvedSlots(C_008622e8* self, void* /*edx*/, C_008622e8* a2)
{
    NetPlayerDDL_ResolveObject(a2, nullptr, reinterpret_cast<bool*>(self));
    NetCogibleDdl_ReadIntoSlot(a2, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(&self->field_0004)));
    NetCogibleDdl_ReadIntoSlot(a2, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(&self->field_0008)));
    return NetCogibleDdl_ReadIntoSlot(a2, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x0C)));
}

HOOK(0x0074E930, C_008622e8_ReadResolvedSlots, "auto");
