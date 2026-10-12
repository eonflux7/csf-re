#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static C_0082d410* __cdecl GfxClumpDict_FillEntryRange(
    C_0082d410* first, C_0082d410* last, const void* source)
{
    for (auto* entry = reinterpret_cast<std::uint8_t*>(first);
         entry != reinterpret_cast<std::uint8_t*>(last);
         entry += 0x3C)
    {
        AssignStringSubstring(entry, nullptr, source, 0, -1);
        CSF_AT(std::uint32_t, entry, 0x1C) =
            CSF_AT(const std::uint32_t, source, 0x1C);

        if (CSF_AT(const std::uint32_t, source, 0x28) >
            CSF_AT(std::uint32_t, entry, 0x24))
        {
            void* storage = Mem_Alloc(
                4 * CSF_AT(const std::uint32_t, source, 0x24));

            CSF_AT(std::uint32_t, entry, 0x24) =
                CSF_AT(const std::uint32_t, source, 0x24);

            if (CSF_AT(void*, entry, 0x20))
            {
                Mem_Free(CSF_AT(void*, entry, 0x20));
                CSF_AT(void*, entry, 0x20) = nullptr;
            }
            CSF_AT(void*, entry, 0x20) = storage;
        }

        const std::uint32_t count =
            CSF_AT(const std::uint32_t, source, 0x28);
        if (count)
        {
            // The original copies dwords forward, including when ranges overlap.
            const auto* input =
                CSF_AT(const std::uint32_t*, source, 0x20);
            auto* output = CSF_AT(std::uint32_t*, entry, 0x20);
            for (std::uint32_t index = 0; index != count; ++index)
                output[index] = input[index];
        }

        CSF_AT(std::uint32_t, entry, 0x28) =
            CSF_AT(const std::uint32_t, source, 0x28);
        CSF_AT(std::uint32_t, entry, 0x2C) =
            CSF_AT(const std::uint32_t, source, 0x2C);
        CSF_AT(std::uint32_t, entry, 0x30) =
            CSF_AT(const std::uint32_t, source, 0x30);
        CSF_AT(std::uint32_t, entry, 0x34) =
            CSF_AT(const std::uint32_t, source, 0x34);
        CSF_AT(std::uint32_t, entry, 0x38) =
            CSF_AT(const std::uint32_t, source, 0x38);
    }
    return last;
}

HOOK(0x006D4D20, GfxClumpDict_FillEntryRange, "auto");
