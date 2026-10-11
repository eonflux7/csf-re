#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, S_0051f950_UpdateParticleState, 0x0051F950, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, S_0051f950_RemoveFinishedParticles, 0x0051FA40, (S_0051f950* self, void* , int));
CSF_FUNC(int, __fastcall, S_0051f950_DetachTransientEffect, 0x00521070, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall S_0051f950_UpdateEffects(
    S_0051f950* self, void* /*edx*/, int elapsedBits)
{
    // This address is also listed as a vtable reference in the packet.
    (void)CSF_VTBL(0x0051F950);

    float elapsed;
    std::memcpy(&elapsed, &elapsedBits, sizeof(elapsed));

    for (std::int32_t index =
             static_cast<std::int32_t>(self->field_0074 - 1);
         index >= 0; --index)
    {
        S_0051f950_DetachTransientEffect(
            self, nullptr,
            reinterpret_cast<int>(
                reinterpret_cast<void**>(self->field_006c)[index]));

        void* effect = reinterpret_cast<void**>(self->field_006c)[index];
        if (effect)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, effect, 0)[0])(effect, nullptr, 1);
            reinterpret_cast<void**>(self->field_006c)[index] = nullptr;
        }

        const std::uint32_t newCount = self->field_0074 - 1;
        self->field_0074 = newCount;
        if (static_cast<std::uint32_t>(index) != newCount)
        {
            auto* destination =
                reinterpret_cast<std::uint8_t*>(self->field_006c)
                + 4 * static_cast<std::uint32_t>(index);
            _memcpy(destination, destination + 4,
                    4 * (newCount - static_cast<std::uint32_t>(index)));
        }
    }

    for (std::uint32_t index = 0; index < self->field_0024; ++index)
    {
        void* effect = reinterpret_cast<void**>(self->field_001c)[index];
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, effect, 0)[0x18 / 4])(
                effect, nullptr, elapsedBits);
    }

    std::int32_t index =
        static_cast<std::int32_t>(self->field_0054 - 1);
    std::uint32_t recordOffset = 16 * static_cast<std::uint32_t>(index);

    for (; index >= 0; --index, recordOffset -= 16)
    {
        auto* record =
            reinterpret_cast<std::uint8_t*>(self->field_004c) + recordOffset;
        void* effect = CSF_AT(void*, record, 0);

        const bool finished =
            reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, effect, 0)[0x34 / 4])(effect, nullptr);

        if (finished)
        {
            const int handle = CSF_AT(int, record, 0x0C);
            if (handle == -1)
            {
                const int global = GetGlobalInt();
                void* registry = reinterpret_cast<void*>(
                    SceneLoader_GetObjectRegistry(
                        reinterpret_cast<void*>(global), nullptr));

                void* registeredEffect = CSF_AT(void*, record, 0);
                reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, registry, 0)[0x18 / 4])(
                        registry, nullptr, registeredEffect);
            }
            else
            {
                void* owner = reinterpret_cast<void*>(self->field_0090);
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, owner, 0)[0x0C / 4])(
                        owner, nullptr, handle);
            }

            const std::uint32_t newCount = self->field_0054 - 1;
            self->field_0054 = newCount;
            if (static_cast<std::uint32_t>(index) != newCount)
            {
                auto* destination =
                    reinterpret_cast<std::uint8_t*>(self->field_004c)
                    + recordOffset;
                _memcpy(destination, destination + 16,
                        16 * (newCount - static_cast<std::uint32_t>(index)));
            }
        }
        else if (CSF_AT(float, record, 0x04) > g_fZero)
        {
            const float remaining = CSF_AT(float, record, 0x04) - elapsed;
            CSF_AT(float, record, 0x04) = remaining;
            if (remaining <= g_fZero)
            {
                CSF_AT(float, record, 0x04) = 0.0f;
                void* expiredEffect = CSF_AT(void*, record, 0);
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, expiredEffect, 0)[0x2C / 4])(
                        expiredEffect, nullptr, 0);
            }
        }
    }

    S_0051f950_UpdateParticleState(self, nullptr, elapsed);
    S_0051f950_RemoveFinishedParticles(self, nullptr, elapsedBits);
}

HOOK(0x00521A10, S_0051f950_UpdateEffects, "auto");
