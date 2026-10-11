#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static void __fastcall S_0051f950_RemoveFinishedParticles(
    S_0051f950* self, void* /*edx*/, int /*unused*/)
{
    std::int32_t index = static_cast<std::int32_t>(self->field_0034 - 1);
    std::uint32_t offset = 20u * static_cast<std::uint32_t>(index);

    while (index >= 0)
    {
        auto* entry = reinterpret_cast<std::uint8_t*>(self->field_002c) + offset;
        if (CSF_AT(std::uint8_t, entry, 0x10))
        {
            void* object = CSF_AT(void*, entry, 0x08);
            if (((bool(__fastcall*)(void*, void*))
                 CSF_AT(void**, object, 0)[0x34 / 4])(object, nullptr))
            {
                void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
                void* registry = reinterpret_cast<void*>(
                    SceneLoader_GetObjectRegistry(sceneLoader, nullptr));
                ((void(__fastcall*)(void*, void*, void*))
                 CSF_AT(void**, registry, 0)[0x18 / 4])(
                    registry, nullptr, CSF_AT(void*, entry, 0x08));

                const std::uint32_t count = self->field_0034 - 1;
                self->field_0034 = count;
                if (static_cast<std::uint32_t>(index) != count)
                {
                    auto* destination =
                        reinterpret_cast<std::uint8_t*>(self->field_002c) + offset;
                    _memcpy(destination, destination + 20,
                            20u * (count - static_cast<std::uint32_t>(index)));
                }
            }
        }
        --index;
        offset -= 20;
    }
}

HOOK(0x0051FA40, S_0051f950_RemoveFinishedParticles, "auto");
