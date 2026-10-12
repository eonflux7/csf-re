#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static std::uint32_t* __fastcall CopyRecordArray(
    std::uint32_t* self, void* /*edx*/, std::uint32_t* source)
{
    const std::uint32_t capacity = CSF_AT(std::uint32_t, source, 0x4);
    void* storage = nullptr;

    if (capacity > 0)
    {
        storage = Mem_Alloc(capacity * 16u);
        if (storage && static_cast<std::int32_t>(capacity - 1u) >= 0)
        {
            auto* record = static_cast<std::uint8_t*>(storage);
            std::uint32_t remaining = capacity;
            do
            {
                CSF_AT(std::uint32_t, record, 0x0) = 0;
                CSF_AT(std::uint32_t, record, 0x4) = 0xFFFFFFFFu;
                CSF_AT(std::uint32_t, record, 0x8) = 0xFFFFFFFFu;
                CSF_AT(std::uint32_t, record, 0xC) = 0xFFFFFFFFu;
                record += 16;
            } while (--remaining);
        }

        CSF_AT(std::uint32_t, self, 0x0) =
            reinterpret_cast<std::uintptr_t>(storage);
        const std::uint32_t bytes = CSF_AT(std::uint32_t, source, 0x8) * 16u;
        const void* sourceStorage = reinterpret_cast<const void*>(
            CSF_AT(std::uint32_t, source, 0x0));
        if (bytes)
            std::memcpy(storage, sourceStorage, bytes);
    }
    else
    {
        CSF_AT(std::uint32_t, self, 0x0) = 0;
    }

    CSF_AT(std::uint32_t, self, 0x8) = CSF_AT(std::uint32_t, source, 0x8);
    CSF_AT(std::uint32_t, self, 0x4) = CSF_AT(std::uint32_t, source, 0x4);
    CSF_AT(std::uint32_t, self, 0xC) = CSF_AT(std::uint32_t, source, 0xC);
    return self;
}

HOOK(0x0048AE50, CopyRecordArray, "auto");
