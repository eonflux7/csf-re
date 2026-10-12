#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall S_004480f0_UpdateTimedControls(
    S_004480f0* self, void* /*edx*/, float elapsedTime)
{
    std::int32_t index = static_cast<std::int32_t>(self->field_0044 - 1);
    std::uint32_t offset = static_cast<std::uint32_t>(index) * 16;

    for (; index >= 0; --index, offset -= 16)
    {
        auto* entry = reinterpret_cast<std::uint8_t*>(self->field_003c) + offset;
        CSF_AT(float, entry, 0x0C) -= elapsedTime;

        void* resolved = reinterpret_cast<void*>(CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            static_cast<int>(self->field_003c + offset)));

        bool removeEntry = !resolved;
        if (resolved)
        {
            entry = reinterpret_cast<std::uint8_t*>(self->field_003c) + offset;
            float remainingTime = CSF_AT(float, entry, 0x0C);
            void* control = CSF_AT(void*, resolved, 0x34);

            if (remainingTime < g_fZero)
            {
                if ((CSF_AT(std::uint32_t, control, 0x44) & (2 | 0x40)) != 0)
                {
                    alignas(8) std::uint8_t expirationEvent[0x24]{};
                    CSF_AT(int, expirationEvent, 0x04) = -1;
                    CSF_AT(int, expirationEvent, 0x08) = -1;
                    CSF_AT(void**, expirationEvent, 0x00) = CSF_VTBL(0x0082D448);
                    CSF_AT(int, expirationEvent, 0x10) = 0;
                    CSF_AT(int, expirationEvent, 0x14) = 0;
                    CSF_AT(int, expirationEvent, 0x18) = 0;
                    CSF_AT(std::uint8_t, expirationEvent, 0x0C) = 0;
                    CSF_AT(int, expirationEvent, 0x20) = 8;
                    CSF_AT(float, expirationEvent, 0x1C) = 270.0f;

                    ((void(__fastcall*)(void*, void*, void*))
                        CSF_AT(void**, control, 0)[0x44 / 4])(
                            control, nullptr, expirationEvent);

                    CSF_AT(void**, expirationEvent, 0x00) = CSF_VTBL(0x0082D410);
                }
                removeEntry = true;
            }
            else if ((CSF_AT(std::uint8_t, control, 0x44) & 0xC2) == 0)
            {
                alignas(8) std::uint8_t activationEvent[0x24]{};
                CSF_AT(int, activationEvent, 0x04) = -1;
                CSF_AT(int, activationEvent, 0x08) = -1;
                CSF_AT(void**, activationEvent, 0x00) = CSF_VTBL(0x0082D448);
                CSF_AT(int, activationEvent, 0x10) = 0;
                CSF_AT(int, activationEvent, 0x14) = 0;
                CSF_AT(int, activationEvent, 0x18) = 0;
                CSF_AT(std::uint8_t, activationEvent, 0x0C) = 0;
                CSF_AT(int, activationEvent, 0x20) = 7;
                CSF_AT(float, activationEvent, 0x1C) = 270.0f;

                ((void(__fastcall*)(void*, void*, void*))
                    CSF_AT(void**, control, 0)[0x44 / 4])(
                        control, nullptr, activationEvent);

                CSF_AT(void**, activationEvent, 0x00) = CSF_VTBL(0x0082D410);
            }
        }

        if (removeEntry)
        {
            std::uint32_t newCount = self->field_0044 - 1;
            self->field_0044 = newCount;
            if (static_cast<std::uint32_t>(index) != newCount)
            {
                auto* destination =
                    reinterpret_cast<std::uint8_t*>(self->field_003c) + offset;
                _memcpy(destination, destination + 16,
                    16 * (newCount - static_cast<std::uint32_t>(index)));
            }
        }
    }
}

HOOK(0x004486E0, S_004480f0_UpdateTimedControls, "auto");
