#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, OverlayRects_AcquireRect, 0x00522160,
         (void* self, void* edx, float* a2));
CSF_FUNC(void, __fastcall, OverlayObject_SetState, 0x005269B0,
         (void* self, void* edx, int a2));
CSF_FUNC(unsigned int*, __fastcall, OverlayManager_ProcessObjectReference, 0x00526CF0,
         (void* self, void* edx, unsigned int* a2));

static int __fastcall S_00521df0_PushOverlayRect(
    S_00521df0* self, void* /*edx*/, float* rect)
{
    const uint32_t count = self->field_0038;
    uint32_t index = 0;

    if (count != 0)
    {
        auto* entry = reinterpret_cast<uint8_t*>(self->field_0030);
        while (index < count)
        {
            if (CSF_AT(uint32_t, entry, 4) != 0 &&
                CSF_AT(float, entry, 8) == rect[0] &&
                CSF_AT(float, entry, 12) == rect[1] &&
                CSF_AT(float, entry, 16) == rect[2] &&
                CSF_AT(float, entry, 20) == rect[3])
            {
                break;
            }

            ++index;
            entry += 24;
        }
    }

    if (index < count && index != static_cast<uint32_t>(-1))
    {
        const uint32_t entryOffset = 24 * index;
        void* object = CSF_AT(
            void*, reinterpret_cast<uint8_t*>(self->field_0030) + entryOffset, 4);

        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x2c / 4])(object, nullptr);

        auto* objectReference = reinterpret_cast<unsigned int*>(
            reinterpret_cast<uint8_t*>(self->field_0030) + entryOffset + 4);
        OverlayManager_ProcessObjectReference(
            reinterpret_cast<void*>(self->field_0040), nullptr, objectReference);

        return static_cast<int>(index);
    }

    const int acquiredIndex = OverlayRects_AcquireRect(self, nullptr, rect);
    if (acquiredIndex != -1)
    {
        const uint32_t entryOffset = 12 * static_cast<uint32_t>(acquiredIndex);
        CSF_AT(uint8_t,
               reinterpret_cast<uint8_t*>(self->field_004c) + entryOffset, 0) = 0;

        void* object = CSF_AT(
            void*, reinterpret_cast<uint8_t*>(self->field_004c) + entryOffset, 8);

        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x2c / 4])(object, nullptr);

        auto* entry = reinterpret_cast<uint8_t*>(self->field_004c) + entryOffset;
        object = CSF_AT(void*, entry, 8);
        unsigned int linkedObject = CSF_AT(unsigned int, object, 0x60);

        if (linkedObject != 0)
        {
            OverlayObject_SetState(CSF_AT(void*, entry, 8), nullptr, 0);
            OverlayManager_ProcessObjectReference(
                reinterpret_cast<void*>(self->field_0040), nullptr, &linkedObject);
        }
    }

    return acquiredIndex;
}

HOOK(0x00522460, S_00521df0_PushOverlayRect, "auto");
