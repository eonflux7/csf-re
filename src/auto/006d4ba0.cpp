#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __cdecl GfxClumpDict_CopyEntriesBackward(
    const void* first, const void* last, int destinationEnd, int /*unused*/)
{
    if (first == last)
        return destinationEnd;

    const auto* source = static_cast<const std::uint8_t*>(last);
    auto* destination = reinterpret_cast<std::uint8_t*>(destinationEnd);

    do
    {
        source -= 0x3C;
        destination -= 0x3C;

        AssignStringSubstring(destination, nullptr, source, 0, -1);
        CSF_AT(std::uint32_t, destination, 0x1C) =
            CSF_AT(std::uint32_t, source, 0x1C);

        if (CSF_AT(std::uint32_t, source, 0x28) >
            CSF_AT(std::uint32_t, destination, 0x24))
        {
            void* replacement =
                Mem_Alloc(4 * CSF_AT(std::uint32_t, source, 0x24));

            CSF_AT(std::uint32_t, destination, 0x24) =
                CSF_AT(std::uint32_t, source, 0x24);

            void* oldBuffer = CSF_AT(void*, destination, 0x20);
            if (oldBuffer)
            {
                Mem_Free(oldBuffer);
                CSF_AT(void*, destination, 0x20) = nullptr;
            }
            CSF_AT(void*, destination, 0x20) = replacement;
        }

        const std::uint32_t count = CSF_AT(std::uint32_t, source, 0x28);
        if (count)
        {
            std::memcpy(
                CSF_AT(void*, destination, 0x20),
                CSF_AT(const void*, source, 0x20),
                4 * count);
        }

        CSF_AT(std::uint32_t, destination, 0x28) =
            CSF_AT(std::uint32_t, source, 0x28);
        CSF_AT(std::uint32_t, destination, 0x2C) =
            CSF_AT(std::uint32_t, source, 0x2C);
        CSF_AT(std::uint32_t, destination, 0x30) =
            CSF_AT(std::uint32_t, source, 0x30);
        CSF_AT(std::uint32_t, destination, 0x34) =
            CSF_AT(std::uint32_t, source, 0x34);
        CSF_AT(std::uint32_t, destination, 0x38) =
            CSF_AT(std::uint32_t, source, 0x38);
    }
    while (source != first);

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(destination));
}

HOOK(0x006D4BA0, GfxClumpDict_CopyEntriesBackward, "auto");
