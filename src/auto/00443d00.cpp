#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(void*, __fastcall, GrowPositionRecords, 0x00443A70,
         (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, MatchesObjectEvent, 0x00443BA0,
         (void* self, void* edx, float a4, uint32_t* arg4));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30,
         (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430,
         (void* self, void* edx));

static C_008342e8* __fastcall ProcessSceneObjectEvent(
    void* self, void* /*edx*/, C_008342e8* event)
{
    if (event->field_0004 != 3 && event->field_0004 != 8)
        return event;

    auto* object = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(event) + 0x8)));

    if (!object)
        return object;

    if (event->field_0004 == 3)
    {
        if (object->field_0050)
            return object;

        const uint32_t count = CSF_AT(uint32_t, self, 0x10);
        uint32_t recordOffset = 0;

        for (uint32_t index = 0; index < count; ++index)
        {
            // The bound float parameter carries the integer index's raw bits.
            float indexArgument;
            std::memcpy(&indexArgument, &index, sizeof(indexArgument));

            if (MatchesObjectEvent(
                    reinterpret_cast<uint8_t*>(self) - 4, nullptr,
                    indexArgument, reinterpret_cast<uint32_t*>(event)))
            {
                auto* records = CSF_AT(uint8_t*, self, 0x8);
                ++CSF_AT(uint32_t, records + recordOffset, 0x24);
            }
            recordOffset += 40;
        }

        return reinterpret_cast<C_008342e8*>(count);
    }

    if (object->field_0050 != 1)
        return object;

    const uint32_t recordTag = CSF_AT(uint32_t, self, 0x4);
    const uint32_t* position = GetObjectPositionVector(object, nullptr);
    const uint32_t positionY = position[1];
    const uint32_t positionZ = position[2];
    const uint32_t positionX = position[0];

    if (CSF_AT(uint32_t, self, 0x20) >= CSF_AT(uint32_t, self, 0x1C))
    {
        GrowPositionRecords(
            reinterpret_cast<uint8_t*>(self) + 0x18, nullptr,
            CSF_AT(int, self, 0x24));
    }

    const uint32_t index = CSF_AT(uint32_t, self, 0x20);
    auto* records = CSF_AT(uint8_t*, self, 0x18);
    auto* record = reinterpret_cast<C_008342e8*>(records + (index << 4));
    CSF_AT(uint32_t, self, 0x20) = index + 1;

    record->vtbl = reinterpret_cast<decltype(record->vtbl)>(recordTag);
    record->field_0004 = positionX;
    CSF_AT(uint32_t, record, 0x8) = positionY;
    record->field_000c = positionZ;
    return record;
}

HOOK(0x00443D00, ProcessSceneObjectEvent, "auto");
