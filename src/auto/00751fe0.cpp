#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(bool, __fastcall, RemoveSpawnEntryBase, 0x007A9BE0,
    (void* self, void* edx, uint8_t a2, int a3));

static bool __fastcall C_00868abc_RemoveSpawnEntry(
    C_00868abc* self, void* /*edx*/, uint8_t mode, int entryId)
{
    uint32_t* entry = &self->field_0028;
    for (int remaining = 32; remaining != 0; --remaining, ++entry)
    {
        if (*entry == static_cast<uint32_t>(entryId))
            *entry = 0xFFFFFFFF;
    }

    --self->field_00a8;
    return RemoveSpawnEntryBase(self, nullptr, mode, entryId);
}

HOOK(0x00751FE0, C_00868abc_RemoveSpawnEntry, "auto");
