#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpLightGetWorld, 0x0058DFD0, (int a1));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00860e68_DetachLight, 0x006D1CB0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, C_00860e68_RemoveType3Light, 0x006D1D30, (void* self, void* edx, void* a2));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static void __fastcall C_00860e68_RemoveWorldLights(
    C_00860e68* self, void* /*edx*/, int world)
{
    if (!world)
        return;

    const std::uint32_t worldCount = self->field_0090;
    if (worldCount)
    {
        const auto* worlds =
            reinterpret_cast<const std::uint32_t*>(self->field_0088);
        std::uint32_t index = 0;

        while (index < worldCount &&
               worlds[index] != static_cast<std::uint32_t>(world))
        {
            ++index;
        }

        if (index < worldCount && index != UINT32_MAX)
        {
            const std::uint32_t newCount = self->field_0090 - 1;
            self->field_0090 = newCount;

            if (index != newCount)
            {
                auto* destination = reinterpret_cast<std::uint8_t*>(
                    self->field_0088 + 4 * index);
                _memcpy(destination, destination + 4, 4 * (newCount - index));
            }

            self->field_0098 = self->field_0090 > 1;
        }
    }

    C_00860e68_DetachLight(
        self, nullptr, reinterpret_cast<void*>(self->field_0030));
    C_00860e68_DetachLight(
        self, nullptr, reinterpret_cast<void*>(self->field_002c));

    for (std::int32_t index =
             static_cast<std::int32_t>(self->field_003c - 1);
         index >= 0; --index)
    {
        auto* light = reinterpret_cast<C_0085e7b8**>(
            self->field_0034)[index];
        void* provider = GetCachedObjectProvider(light, nullptr);
        const int lightWorld =
            RpLightGetWorld(reinterpret_cast<int>(provider));

        if (lightWorld && lightWorld == world)
            self->vSlot2C(reinterpret_cast<int>(light));
    }

    for (std::int32_t index =
             static_cast<std::int32_t>(self->field_004c - 1);
         index >= 0; --index)
    {
        auto* light = reinterpret_cast<C_0085e7b8**>(
            self->field_0044)[index];
        void* provider = GetCachedObjectProvider(light, nullptr);

        if (RpLightGetWorld(reinterpret_cast<int>(provider)) == world)
        {
            const int lightType =
                reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, light, 0)[0x1C / 4])(light, nullptr);

            if (lightType == 3)
                C_00860e68_RemoveType3Light(self, nullptr, light);
            else
                self->vSlot2C(reinterpret_cast<int>(light));
        }
    }
}

HOOK(0x006D2110, C_00860e68_RemoveWorldLights, "auto");
