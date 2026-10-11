#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include <cfloat>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static const void* __fastcall GrowBoundsArray(void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t capacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* replacement = Mem_Alloc(44u * capacity);
    if (replacement && static_cast<std::int32_t>(capacity - 1u) >= 0)
    {
        auto* record = static_cast<std::uint8_t*>(replacement);
        std::uint32_t remaining = capacity;
        do
        {
            CSF_AT(float, record, 0x0C) = FLT_MAX;
            CSF_AT(float, record, 0x10) = FLT_MAX;
            CSF_AT(float, record, 0x14) = FLT_MAX;
            CSF_AT(std::uint32_t, record, 0x18) = 0;
            CSF_AT(float, record, 0x1C) = FLT_MAX;
            CSF_AT(float, record, 0x20) = FLT_MAX;
            CSF_AT(float, record, 0x24) = FLT_MAX;
            CSF_AT(std::uint32_t, record, 0x28) = 0;

            CSF_AT(std::int32_t, record, 0x08) = -1;
            CSF_AT(std::uint8_t, record, 0x00) = 0;
            CSF_AT(std::uint32_t, record, 0x04) = 0;

            CSF_AT(float, record, 0x0C) = FLT_MAX;
            CSF_AT(float, record, 0x10) = FLT_MAX;
            CSF_AT(float, record, 0x14) = FLT_MAX;
            CSF_AT(std::uint32_t, record, 0x18) = 0;
            CSF_AT(float, record, 0x1C) = FLT_MAX;
            CSF_AT(float, record, 0x20) = FLT_MAX;
            CSF_AT(float, record, 0x24) = FLT_MAX;
            CSF_AT(std::uint32_t, record, 0x28) = 0;

            record += 44;
        } while (--remaining);
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x08);
    if (count > 0)
    {
        std::memcpy(replacement, CSF_AT(void*, self, 0x00), 44u * count);
    }

    void* oldStorage = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) +=
        static_cast<std::uint32_t>(additionalCapacity);

    const void* result = oldStorage;
    if (oldStorage)
    {
        result = reinterpret_cast<const void*>(
            static_cast<std::uintptr_t>(Mem_Free(oldStorage)));
        CSF_AT(void*, self, 0x00) = nullptr;
    }

    CSF_AT(void*, self, 0x00) = replacement;
    return result;
}

HOOK(0x00406EC0, GrowBoundsArray, "auto");
