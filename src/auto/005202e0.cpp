#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, FxParticleManager_GrowRetiredParticleArray, 0x0051FCD0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static const auto g_ArrayGrowthVtableReference = CSF_VTBL(0x0041E5F0);

static void __fastcall C_00840fdc_HandleParticleEvent(
    C_00840fdc* self, void* /*edx*/, uint32_t* event)
{
    if (event[1] == 19)
    {
        int32_t index = static_cast<int32_t>(self->field_0064 - 1);
        uint32_t byteOffset = 36 * static_cast<uint32_t>(index);

        while (index >= 0)
        {
            auto* record =
                reinterpret_cast<uint8_t*>(self->field_005c) + byteOffset;
            const uint32_t eventId = event[3];

            if (CSF_AT(uint32_t, record, 0x08) == eventId)
            {
                void* particle = CSF_AT(void*, record, 0x14);
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, particle, 0)[0x2C / 4])(
                        particle, nullptr, 0);

                void** particleVtable = CSF_AT(void**, particle, 0);
                int state = ((int(__fastcall*)(void*, void*))
                    particleVtable[0x48 / 4])(particle, nullptr);
                ((void(__fastcall*)(void*, void*, int))
                    particleVtable[0x38 / 4])(particle, nullptr, state);

                record = reinterpret_cast<uint8_t*>(self->field_005c)
                    + byteOffset;
                const uint32_t associatedFirst =
                    CSF_AT(uint32_t, record, 0x1C);
                const uint32_t associatedSecond =
                    CSF_AT(uint32_t, record, 0x20);

                if (self->field_0054 >= CSF_AT(uint32_t, self, 0x50))
                {
                    FxParticleManager_GrowRetiredParticleArray(
                        reinterpret_cast<uint8_t*>(self) + 0x4C,
                        nullptr,
                        CSF_AT(int, self, 0x58));
                }

                auto* retiredRecord =
                    reinterpret_cast<uint8_t*>(self->field_004c)
                    + 16 * self->field_0054;
                ++self->field_0054;
                CSF_AT(void*, retiredRecord, 0x00) = particle;
                CSF_AT(uint32_t, retiredRecord, 0x04) = 0;
                CSF_AT(uint32_t, retiredRecord, 0x08) = associatedFirst;
                CSF_AT(uint32_t, retiredRecord, 0x0C) = associatedSecond;

                const uint32_t remainingCount = self->field_0064 - 1;
                self->field_0064 = remainingCount;
                if (static_cast<uint32_t>(index) != remainingCount)
                {
                    auto* destination =
                        reinterpret_cast<uint8_t*>(self->field_005c)
                        + byteOffset;
                    _memcpy(
                        destination,
                        destination + 36,
                        36 * (remainingCount - static_cast<uint32_t>(index)));
                }
            }

            --index;
            byteOffset -= 36;
        }
    }
    else
    {
        const int eventValue = ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, event, 0)[0x1C / 4])(event, nullptr);

        auto* pendingEvents = reinterpret_cast<S_0041e5f0*>(
            reinterpret_cast<uint8_t*>(self) + 0x6C);

        if (self->field_0074 >= pendingEvents->field_0004)
        {
            S_0041e5f0_GrowArrayCapacity(
                pendingEvents, nullptr,
                static_cast<int>(pendingEvents->field_000c));
        }

        const uint32_t count = pendingEvents->field_0008;
        auto* destination =
            reinterpret_cast<uint8_t*>(pendingEvents->field_0000) + 4 * count;
        pendingEvents->field_0008 = count + 1;
        CSF_AT(int, destination, 0) = eventValue;
    }
}

HOOK(0x005202E0, C_00840fdc_HandleParticleEvent, "auto");
