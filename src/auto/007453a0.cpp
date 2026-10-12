#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));
static CSF_FUNC(char, __fastcall, C_00858180_ApplySpawnFlag, 0x007410F0, (C_00858180* self, void* , int spawnValue));

static char __fastcall C_00858180_ReadAndApplySpawnFlag(
    C_00858180* self, void* /*edx*/, C_008622e8* source)
{
    NetCogibleDdl_ReadIntoSlot(
        source, nullptr, reinterpret_cast<int>(&source));
    return C_00858180_ApplySpawnFlag(
        self, nullptr, reinterpret_cast<int>(source));
}

HOOK(0x007453A0, C_00858180_ReadAndApplySpawnFlag, "auto");
