#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(int, g_CurrentSceneIndex, 0x0089972C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

static void __fastcall C_008311c4_UpdateTrackedObjectPositionFromEvent(
    C_008311c4* self, void* /*edx*/, int eventAddress)
{
    auto* event = reinterpret_cast<uint8_t*>(eventAddress);
    if (CSF_AT(int, event, 0x04) != 8)
        return;

    uint32_t count = self->field_0010;
    if (count)
        count = static_cast<int32_t>(self->field_0014 - count) / 100;

    if (!count)
        return;

    uint32_t index = 0;
    auto* candidate = reinterpret_cast<uint8_t*>(self->field_0010) + 20;
    while (CSF_AT(int, candidate, 0x08) != g_CurrentSceneIndex ||
           CSF_AT(uint32_t, candidate, 0x00) != CSF_AT(uint32_t, event, 0x0C))
    {
        ++index;
        candidate += 100;
        if (index >= count)
            return;
    }

    int objectAddress = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        static_cast<int>(static_cast<uint32_t>(eventAddress) + 8));

    auto* record = reinterpret_cast<uint8_t*>(self->field_0010) + 100 * index;
    CSF_AT(uint8_t, record, 0x28) = 1;

    if (objectAddress)
    {
        record = reinterpret_cast<uint8_t*>(self->field_0010) + 100 * index;
        uint32_t* position = GetObjectPositionVector(
            reinterpret_cast<void*>(objectAddress), nullptr);

        CSF_AT(uint32_t, record, 0x2C) = position[0];
        CSF_AT(uint32_t, record, 0x30) = position[1];
        CSF_AT(uint32_t, record, 0x34) = position[2];
    }
}

HOOK(0x0044BDF0, C_008311c4_UpdateTrackedObjectPositionFromEvent, "auto");
