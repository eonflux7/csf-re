#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, S_006b0320_AssignBaseData, 0x006B0320, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, CharacterDefs_AssignEmbeddedMember, 0x006CAF70, (void* self, void* , void* source));

static int __fastcall S_006b0320_AssignWithClonedEntries(
    S_006b0320* self, void* /*edx*/, int sourceAddress)
{
    int result = S_006b0320_AssignBaseData(self, nullptr, sourceAddress);
    auto* source = reinterpret_cast<S_006b0320*>(
        static_cast<std::uintptr_t>(sourceAddress));

    const std::uint32_t count = source->field_0018;
    if (count != 0)
    {
        auto* entries = reinterpret_cast<S_0041e5f0*>(
            reinterpret_cast<std::uint8_t*>(self) + 0x10);

        const std::uint32_t capacity = self->field_0014;
        if (count > capacity)
        {
            S_0041e5f0_GrowArrayCapacity(
                entries, nullptr, static_cast<int>(count - capacity));
        }

        std::uint32_t index = 0;
        do
        {
            void* allocation = Mem_Alloc(24);
            void* clone = nullptr;
            if (allocation)
            {
                CSF_AT(std::uint32_t, allocation, 0x00) = 0;
                CSF_AT(std::uint32_t, allocation, 0x04) = 0;
                CSF_AT(std::uint32_t, allocation, 0x08) = 0;
                CSF_AT(std::uint32_t, allocation, 0x0C) = 10;
                CSF_AT(std::uint32_t, allocation, 0x10) = 0;
                CSF_AT(std::uint32_t, allocation, 0x14) = 0;
                clone = allocation;
            }

            void* sourceEntry =
                reinterpret_cast<void**>(source->field_0010)[index];
            CSF_AT(std::uint32_t, clone, 0x10) =
                CSF_AT(std::uint32_t, sourceEntry, 0x10);
            CSF_AT(std::uint32_t, clone, 0x14) =
                CSF_AT(std::uint32_t, sourceEntry, 0x14);

            CharacterDefs_AssignEmbeddedMember(
                clone, nullptr,
                reinterpret_cast<void**>(source->field_0010)[index]);

            if (entries->field_0008 >= entries->field_0004)
            {
                S_0041e5f0_GrowArrayCapacity(
                    entries, nullptr,
                    static_cast<int>(entries->field_000c));
            }

            const std::uint32_t destinationIndex = entries->field_0008;
            void** destination =
                reinterpret_cast<void**>(entries->field_0000) + destinationIndex;
            entries->field_0008 = destinationIndex + 1;
            result = static_cast<int>(count);
            ++index;
            *destination = clone;
        }
        while (index < count);
    }

    return result;
}

HOOK(0x006B03B0, S_006b0320_AssignWithClonedEntries, "auto");
