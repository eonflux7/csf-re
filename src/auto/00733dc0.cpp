#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall AssignRecordArray(
    void* self, void* /*edx*/, const void* source)
{
    if (CSF_AT(std::uint32_t, source, 0x08) >
        CSF_AT(std::uint32_t, self, 0x04))
    {
        const std::uint32_t capacity = CSF_AT(std::uint32_t, source, 0x04);
        void* storage = Mem_Alloc(48 * capacity);

        if (storage && static_cast<std::int32_t>(capacity - 1) >= 0)
        {
            auto* record = static_cast<std::uint8_t*>(storage);
            std::uint32_t remaining = capacity;
            do
            {
                CSF_AT(std::uint32_t, record, 0x08) = 0;
                CSF_AT(std::uint32_t, record, 0x0C) = 0;
                CSF_AT(std::int32_t, record, 0x04) = -1;
                CSF_AT(std::uint32_t, record, 0x14) = 0;
                CSF_AT(std::uint32_t, record, 0x18) = 0;
                CSF_AT(std::uint32_t, record, 0x1C) = 0;
                CSF_AT(std::uint32_t, record, 0x20) = 0;
                CSF_AT(std::uint32_t, record, 0x24) = 0;
                CSF_AT(float, record, 0x28) = 1.0f;
                CSF_AT(std::uint32_t, record, 0x10) = 0;
                CSF_AT(std::int32_t, record, 0x00) = -1;
                record += 48;
            } while (--remaining);
        }

        CSF_AT(std::uint32_t, self, 0x04) =
            CSF_AT(std::uint32_t, source, 0x04);

        if (CSF_AT(void*, self, 0x00))
        {
            Mem_Free(CSF_AT(void*, self, 0x00));
            CSF_AT(void*, self, 0x00) = nullptr;
        }
        CSF_AT(void*, self, 0x00) = storage;
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, source, 0x08);
    if (count)
    {
        std::memcpy(
            CSF_AT(void*, self, 0x00),
            CSF_AT(const void*, source, 0x00),
            48 * count);
    }

    CSF_AT(std::uint32_t, self, 0x08) =
        CSF_AT(std::uint32_t, source, 0x08);
    CSF_AT(std::uint32_t, self, 0x0C) =
        CSF_AT(std::uint32_t, source, 0x0C);
    return self;
}

HOOK(0x00733DC0, AssignRecordArray, "auto");
