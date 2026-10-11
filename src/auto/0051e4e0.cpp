#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
static CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));
static CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static void* __fastcall C_00840f48_RemoveFinishedParticles(
    C_00840f48* self, void* /*edx*/, int /*unused*/)
{
    const char active = self->vSlot14();
    void* result = reinterpret_cast<void*>(static_cast<std::uintptr_t>(
        static_cast<unsigned char>(active)));

    if (active)
    {
        for (std::int32_t i = static_cast<std::int32_t>(self->field_0024 - 1);
             i >= 0; --i)
        {
            void* particle = CSF_AT(void*, reinterpret_cast<void*>(self->field_001c), 4 * i);
            const char finished =
                reinterpret_cast<char (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, particle, 0)[0x34 / 4])(particle, nullptr);
            result = reinterpret_cast<void*>(static_cast<std::uintptr_t>(
                static_cast<unsigned char>(finished)));

            if (finished)
            {
                void* registry = reinterpret_cast<void*>(
                    SceneLoader_GetObjectRegistry(
                        reinterpret_cast<void*>(GetGlobalInt()), nullptr));

                reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, registry, 0)[0x18 / 4])(
                        registry, nullptr,
                        CSF_AT(void*, reinterpret_cast<void*>(self->field_001c), 4 * i));

                const std::uint32_t remaining = self->field_0024 - 1;
                self->field_0024 = remaining;
                result = reinterpret_cast<void*>(remaining);
                if (static_cast<std::uint32_t>(i) != remaining)
                {
                    auto* destination =
                        reinterpret_cast<std::uint8_t*>(self->field_001c) + 4 * i;
                    result = _memcpy(
                        destination, destination + 4,
                        4 * (remaining - static_cast<std::uint32_t>(i)));
                }
            }
        }
    }
    return result;
}

HOOK(0x0051E4E0, C_00840f48_RemoveFinishedParticles, "auto");
