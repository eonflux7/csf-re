#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall S_0051f950_UpdateParticles(
    S_0051f950* self, void* /*edx*/, float elapsedTime)
{
    std::int32_t index = static_cast<std::int32_t>(self->field_0064 - 1u);
    if (index < 0)
        return;

    std::uint32_t byteOffset = 36u * static_cast<std::uint32_t>(index);
    for (; index >= 0; --index, byteOffset -= 36u)
    {
        auto* entry = reinterpret_cast<std::uint8_t*>(self->field_005c) + byteOffset;
        void* particle = CSF_AT(void*, entry, 0x14);

        if (((bool(__fastcall*)(void*, void*))
                 CSF_AT(void**, particle, 0)[0x34 / 4])(particle, nullptr))
        {
            const std::uint32_t handle = CSF_AT(std::uint32_t, entry, 0x20);
            if (handle == 0xFFFFFFFFu)
            {
                void* global = reinterpret_cast<void*>(GetGlobalInt());
                void* registry = reinterpret_cast<void*>(
                    SceneLoader_GetObjectRegistry(global, nullptr));

                ((void(__fastcall*)(void*, void*, void*))
                    CSF_AT(void**, registry, 0)[0x18 / 4])(
                        registry, nullptr, CSF_AT(void*, entry, 0x14));
            }
            else
            {
                void* manager = reinterpret_cast<void*>(self->field_0090);
                ((void(__fastcall*)(void*, void*, std::uint32_t))
                    CSF_AT(void**, manager, 0)[0x0C / 4])(
                        manager, nullptr, handle);
            }

            const std::uint32_t newCount = self->field_0064 - 1u;
            self->field_0064 = newCount;
            if (static_cast<std::uint32_t>(index) != newCount)
            {
                auto* destination =
                    reinterpret_cast<std::uint8_t*>(self->field_005c) + byteOffset;
                _memcpy(destination, destination + 36,
                        36u * (newCount - static_cast<std::uint32_t>(index)));
            }
        }
        else if (CSF_AT(float, entry, 0x18) > g_fZero)
        {
            const float remainingTime = CSF_AT(float, entry, 0x18) - elapsedTime;
            CSF_AT(float, entry, 0x18) = remainingTime;
            if (remainingTime <= g_fZero)
            {
                CSF_AT(float, entry, 0x18) = 0.0f;
                particle = CSF_AT(void*, entry, 0x14);
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, particle, 0)[0x2C / 4])(
                        particle, nullptr, 0);
            }
        }
    }
}

HOOK(0x0051F950, S_0051f950_UpdateParticles, "auto");
