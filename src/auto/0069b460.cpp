#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0, (int mode));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, MissionScript_ProcessSection, 0x0069AFE0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, MissionScript_InitializeSection, 0x0069B100, (void* self, void* edx, int a2, unsigned int a3, char a4));

[[maybe_unused]] static const auto kSectionTableBinding = CSF_VTBL(0x0069AFE0);
[[maybe_unused]] static const auto kArrayCapacityBinding = CSF_VTBL(0x0041E5F0);

static int __fastcall S_0069afe0_ProcessSectionTable(
    S_0069afe0* self, void* /*edx*/)
{
    if (self->field_001c != self->field_0024)
        return MissionScript_ProcessSection(self, nullptr);

    const std::int32_t initialCount =
        static_cast<std::int32_t>(self->field_0034);
    int result = 0;
    std::uint32_t examined = 0;

    if (initialCount != 0)
    {
        for (;;)
        {
            const std::int32_t cursor =
                static_cast<std::int32_t>(self->field_003c);
            if (cursor >= initialCount || cursor < 0)
                self->field_003c = 0;

            const std::uint32_t index = self->field_003c;
            self->field_003c = index + 1;
            S_0069afe0* section =
                reinterpret_cast<S_0069afe0**>(self->field_002c)[index];

            if (section->field_001c != section->field_0024)
            {
                result = MissionScript_ProcessSection(section, nullptr);
                --self->field_003c;
                break;
            }

            const std::uint32_t count = self->field_0034;
            ++examined;
            if (examined >= count)
                break;
        }
    }

    if (result == 0)
    {
        SetAllocationMode(5);
        SetAllocationMode(3);
        S_0069afe0* section =
            static_cast<S_0069afe0*>(Mem_Alloc(44));
        if (section)
        {
            section->field_0000 = 0;
            CSF_AT(std::uint8_t, section, 0x28) = 1;
            CSF_AT(std::uint8_t, section, 0x29) = 0;
        }
        SetAllocationMode(0);

        const std::uint32_t elementSize = self->field_0010;
        const char flags = static_cast<char>(self->field_0040);
        if (MissionScript_InitializeSection(
                section, nullptr, static_cast<int>(elementSize),
                0x800 / elementSize, flags))
        {
            SetAllocationMode(3);
            const std::uint32_t count = self->field_0034;
            const std::uint32_t capacity = self->field_0030;
            S_0041e5f0* sections = reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<std::uint8_t*>(self) + 0x2C);

            if (count >= capacity)
                S_0041e5f0_GrowArrayCapacity(
                    sections, nullptr,
                    static_cast<int>(sections->field_000c));

            const std::uint32_t index = sections->field_0008;
            S_0069afe0** destination =
                reinterpret_cast<S_0069afe0**>(sections->field_0000) + index;
            sections->field_0008 = index + 1;
            *destination = section;

            SetAllocationMode(0);
            result = MissionScript_ProcessSection(section, nullptr);
        }
        SetAllocationMode(4);
    }

    return result;
}

HOOK(0x0069B460, S_0069afe0_ProcessSectionTable, "auto");
