#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(C_0085e65c*, __fastcall, ConstructFontBase, 0x00692390, (C_0085e65c* self, void* edx));
CSF_FUNC(char, __fastcall, S_0041e5f0_ProcessEntryName, 0x006D0040, (void* self, void* edx, uint32_t* a2));

static int __fastcall S_0041e5f0_AddValueEntry(
    S_0041e5f0* self, void* /*edx*/, C_0082d410* name, int value)
{
    S_0041e5f0_ProcessEntryName(
        self, nullptr, reinterpret_cast<uint32_t*>(name));

    const auto count = static_cast<std::int32_t>(self->field_0008);
    std::int32_t index = 0;
    bool foundFreeSlot = false;

    if (count > 0)
    {
        const auto* entries =
            reinterpret_cast<const std::uint32_t*>(self->field_0000);

        while (index < count)
        {
            if (entries[index] == 0)
            {
                foundFreeSlot = true;
                break;
            }
            ++index;
        }
    }

    if (foundFreeSlot)
    {
        void* entry = Mem_Alloc(40);
        int result = 0;

        if (entry)
        {
            alignas(8) std::uint8_t temporaryBytes[0x20];
            auto& temporary =
                *reinterpret_cast<C_0082d410*>(temporaryBytes);

            CSF_AT(std::uint32_t, temporaryBytes, 0x18) = 15;
            CSF_AT(std::uint32_t, temporaryBytes, 0x14) = 0;
            reinterpret_cast<std::uint8_t*>(&temporary.field_0004)[0] = 0;
            AssignStringSubstring(&temporary, nullptr, name, 0, -1);

            ConstructFontBase(reinterpret_cast<C_0085e65c*>(entry), nullptr);
            CSF_AT(void**, entry, 0x00) = CSF_VTBL(0x00861A28);

            auto* entryName = reinterpret_cast<C_0082d410*>(
                static_cast<std::uint8_t*>(entry) + 8);
            CSF_AT(std::uint32_t, entry, 0x20) = 15;
            CSF_AT(std::uint32_t, entry, 0x1C) = 0;
            reinterpret_cast<std::uint8_t*>(&entryName->field_0004)[0] = 0;
            AssignStringSubstring(entryName, nullptr, &temporary, 0, -1);
            CSF_AT(int, entry, 0x24) = value;

            result = value;
            if (CSF_AT(std::uint32_t, temporaryBytes, 0x18) >= 0x10)
            {
                result = static_cast<int>(temporary.field_0004);
                if (temporary.field_0004)
                {
                    result = Mem_FreeDebug(
                        reinterpret_cast<void*>(temporary.field_0004));
                }
            }
        }

        reinterpret_cast<std::uint32_t*>(self->field_0000)[index] =
            static_cast<std::uint32_t>(
                reinterpret_cast<std::uintptr_t>(entry));
        return result;
    }

    void* entry = Mem_Alloc(40);

    if (entry)
    {
        alignas(8) std::uint8_t temporaryBytes[0x20];
        auto& temporary =
            *reinterpret_cast<C_0082d410*>(temporaryBytes);

        CSF_AT(std::uint32_t, temporaryBytes, 0x18) = 15;
        CSF_AT(std::uint32_t, temporaryBytes, 0x14) = 0;
        reinterpret_cast<std::uint8_t*>(&temporary.field_0004)[0] = 0;
        AssignStringSubstring(&temporary, nullptr, name, 0, -1);

        ConstructFontBase(reinterpret_cast<C_0085e65c*>(entry), nullptr);
        CSF_AT(void**, entry, 0x00) = CSF_VTBL(0x00861A28);

        auto* entryName = reinterpret_cast<C_0082d410*>(
            static_cast<std::uint8_t*>(entry) + 8);
        CSF_AT(std::uint32_t, entry, 0x20) = 15;
        CSF_AT(std::uint32_t, entry, 0x1C) = 0;
        reinterpret_cast<std::uint8_t*>(&entryName->field_0004)[0] = 0;
        AssignStringSubstring(entryName, nullptr, &temporary, 0, -1);
        CSF_AT(int, entry, 0x24) = value;

        if (CSF_AT(std::uint32_t, temporaryBytes, 0x18) >= 0x10 &&
            temporary.field_0004)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(temporary.field_0004));
        }
    }

    if (self->field_0008 >= self->field_0004)
    {
        S_0041e5f0_GrowArrayCapacity(
            self, nullptr, static_cast<int>(self->field_000c));
    }

    const std::uint32_t appendIndex = self->field_0008;
    auto* destination =
        reinterpret_cast<std::uint32_t*>(self->field_0000) + appendIndex;
    const std::uint32_t newCount = appendIndex + 1;
    self->field_0008 = newCount;
    *destination = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(entry));
    return static_cast<int>(newCount);
}

HOOK(0x006E13B0, S_0041e5f0_AddValueEntry, "auto");
