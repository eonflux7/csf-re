#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, ClearListAndFreeNodes, 0x004E9980, (uint32_t* self, void*));
CSF_FUNC(void**, __fastcall, MpSpawnPoints_ReleaseSpawnResources, 0x00553FD0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, MpSpawnPoints_DestroyEmbeddedState, 0x005546C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, MpSpawnPoints_DestroyEmbeddedObject, 0x0073F710, (void* self, void* edx));

static unsigned int __fastcall C_00844694_Destroy(C_00844694* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00844694);
    MpSpawnPoints_ReleaseSpawnResources(self, nullptr);

    if (self->field_1490)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_1490));
        self->field_1490 = 0;
    }

    auto* bytes = reinterpret_cast<uint8_t*>(self);
    ClearListAndFreeNodes(reinterpret_cast<uint32_t*>(bytes + 0x1464), nullptr);

    if (CSF_AT(uint32_t, self, 0x1468))
        Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x1468)));
    CSF_AT(uint32_t, self, 0x1468) = 0;

    self->field_1410 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    self->field_1404 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));

    MpSpawnPoints_DestroyEmbeddedState(bytes + 0xCA8, nullptr);
    MpSpawnPoints_DestroyEmbeddedObject(bytes + 0xC94, nullptr);

    auto* stringEnd = bytes + 0xCA4;
    unsigned int result;
    int remaining = 50;
    do
    {
        result = CSF_AT(uint32_t, stringEnd, -0x40);
        stringEnd -= 0x40;
        if (result >= 8)
        {
            result = CSF_AT(uint32_t, stringEnd, -0x14);
            if (result)
                result = Mem_FreeDebug(
                    reinterpret_cast<void*>(CSF_AT(uint32_t, stringEnd, -0x14)));
        }

        --remaining;
        CSF_AT(uint32_t, stringEnd, 0) = 7;
        CSF_AT(uint32_t, stringEnd, -4) = 0;
        CSF_AT(uint16_t, stringEnd, -0x14) = 0;
    } while (remaining);

    return result;
}

HOOK(0x005554A0, C_00844694_Destroy, "auto");
