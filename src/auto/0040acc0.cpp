#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall CopyFourComponentArray(
    void* self, void* /*edx*/, const void* source)
{
    if (CSF_AT(std::uint32_t, source, 0x08) >
        CSF_AT(std::uint32_t, self, 0x04))
    {
        const std::uint32_t capacity =
            CSF_AT(std::uint32_t, source, 0x04);
        void* replacement = Mem_Alloc(16 * capacity);

        if (replacement &&
            static_cast<std::int32_t>(capacity - 1) >= 0)
        {
            auto* element = static_cast<std::uint8_t*>(replacement);
            std::uint32_t remaining = capacity;
            do
            {
                CSF_AT(std::uint32_t, element, 0x00) = 0;
                CSF_AT(std::uint32_t, element, 0x04) = 0;
                CSF_AT(std::uint32_t, element, 0x08) = 0;
                CSF_AT(float, element, 0x0C) = -1.0f;
                element += 16;
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
        const void* sourceStorage = CSF_AT(void*, source, 0x00);
        void* destinationStorage = CSF_AT(void*, self, 0x00);
        std::memcpy(destinationStorage, sourceStorage, 16 * count);
    }

    CSF_AT(std::uint32_t, self, 0x08) =
        CSF_AT(std::uint32_t, source, 0x08);
    CSF_AT(std::uint32_t, self, 0x0C) =
        CSF_AT(std::uint32_t, source, 0x0C);
    return self;
}

HOOK(0x0040ACC0, CopyFourComponentArray, "auto");
