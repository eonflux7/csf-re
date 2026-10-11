#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall CopyRecordArray(void* self, void* /*edx*/, const void* source)
{
    if (CSF_AT(std::uint32_t, source, 0x08) >
        CSF_AT(std::uint32_t, self, 0x04))
    {
        const std::uint32_t capacity = CSF_AT(std::uint32_t, source, 0x04);
        void* replacement = Mem_Alloc(12u * capacity);

        if (replacement && static_cast<std::int32_t>(capacity - 1u) >= 0)
        {
            auto* record = static_cast<std::uint8_t*>(replacement);
            std::uint32_t remaining = capacity;
            do
            {
                CSF_AT(std::uint32_t, record, 0x04) = 0;
                CSF_AT(std::uint32_t, record, 0x08) = 0;
                CSF_AT(std::int32_t, record, 0x00) = -1;
                record += 12;
            } while (--remaining);
        }

        void* oldStorage = CSF_AT(void*, self, 0x00);
        CSF_AT(std::uint32_t, self, 0x04) =
            CSF_AT(std::uint32_t, source, 0x04);
        if (oldStorage)
        {
            Mem_Free(oldStorage);
            CSF_AT(void*, self, 0x00) = nullptr;
        }
        CSF_AT(void*, self, 0x00) = replacement;
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, source, 0x08);
    if (count)
    {
        std::memcpy(CSF_AT(void*, self, 0x00),
                    CSF_AT(void*, source, 0x00),
                    12u * count);
    }

    CSF_AT(std::uint32_t, self, 0x08) =
        CSF_AT(std::uint32_t, source, 0x08);
    CSF_AT(std::uint32_t, self, 0x0C) =
        CSF_AT(std::uint32_t, source, 0x0C);
    return self;
}

HOOK(0x00733F50, CopyRecordArray, "auto");
