#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
static CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static std::uint32_t* __fastcall C_00840f28_RemoveInactiveParticles(
    C_00840f28* self, void* /*edx*/)
{
    auto* result = reinterpret_cast<std::uint32_t*>(GetValue8B0554());
    std::int32_t index = static_cast<std::int32_t>(self->field_0010 - 1);
    void* manager = CSF_AT(void*, result, 0x1FCC);

    for (; index >= 0; --index)
    {
        const std::uint32_t particle =
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0008), index * 4);
        void* entry =
            reinterpret_cast<void* (__fastcall*)(void*, void*, std::uint32_t)>(
                CSF_AT(void**, manager, 0)[2])(manager, nullptr, particle);

        result = reinterpret_cast<std::uint32_t*>(
            static_cast<std::uint8_t*>(entry) + 4);
        if (CSF_AT(std::uint32_t, entry, 0x4) != 0 ||
            CSF_AT(std::uint8_t, result, 0x44) == 1 ||
            CSF_AT(std::uint8_t, result, 0x4C) == 1)
        {
            const std::uint32_t newCount = self->field_0010 - 1;
            result = reinterpret_cast<std::uint32_t*>(newCount);
            self->field_0010 = newCount;
            if (static_cast<std::uint32_t>(index) != newCount)
            {
                auto* destination =
                    reinterpret_cast<std::uint8_t*>(self->field_0008) + index * 4;
                result = static_cast<std::uint32_t*>(_memcpy(
                    destination, destination + 4,
                    4 * (newCount - static_cast<std::uint32_t>(index))));
            }
        }
    }
    return result;
}

HOOK(0x0051DFA0, C_00840f28_RemoveInactiveParticles, "auto");
