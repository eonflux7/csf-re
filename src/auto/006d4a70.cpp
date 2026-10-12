#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static C_0082d410* __cdecl GfxClumpDict_CopyEntries(
    int sourceBegin, int sourceEnd, C_0082d410* destination)
{
    std::uint32_t sourceAddress = static_cast<std::uint32_t>(sourceBegin);
    const std::uint32_t endAddress = static_cast<std::uint32_t>(sourceEnd);

    while (sourceAddress != endAddress)
    {
        void* source = reinterpret_cast<void*>(sourceAddress);
        AssignStringSubstring(destination, nullptr, source, 0, -1);

        destination[2].field_0004 = CSF_AT(std::uint32_t, source, 0x1C);

        if (CSF_AT(std::uint32_t, source, 0x28) >
            reinterpret_cast<std::uintptr_t>(destination[3].vtbl))
        {
            void* newStorage =
                Mem_Alloc(4 * CSF_AT(std::uint32_t, source, 0x24));

            destination[3].vtbl = reinterpret_cast<void**>(
                CSF_AT(std::uint32_t, source, 0x24));

            if (destination[2].field_0008)
            {
                Mem_Free(reinterpret_cast<void*>(destination[2].field_0008));
                destination[2].field_0008 = 0;
            }

            destination[2].field_0008 =
                reinterpret_cast<std::uintptr_t>(newStorage);
        }

        const std::uint32_t count = CSF_AT(std::uint32_t, source, 0x28);
        if (count)
        {
            const auto* sourceWords = reinterpret_cast<const std::uint32_t*>(
                CSF_AT(std::uint32_t, source, 0x20));
            auto* destinationWords = reinterpret_cast<std::uint32_t*>(
                destination[2].field_0008);

            // Preserve the original forward dword copy, including overlap.
            for (std::uint32_t i = 0; i != count; ++i)
                destinationWords[i] = sourceWords[i];
        }

        destination[3].field_0004 = CSF_AT(std::uint32_t, source, 0x28);
        destination[3].field_0008 = CSF_AT(std::uint32_t, source, 0x2C);
        destination[4].vtbl = reinterpret_cast<void**>(
            CSF_AT(std::uint32_t, source, 0x30));
        destination[4].field_0004 = CSF_AT(std::uint32_t, source, 0x34);
        destination[4].field_0008 = CSF_AT(std::uint32_t, source, 0x38);

        sourceAddress += 0x3C;
        destination = reinterpret_cast<C_0082d410*>(
            reinterpret_cast<std::uint8_t*>(destination) + 0x3C);
    }

    return destination;
}

HOOK(0x006D4A70, GfxClumpDict_CopyEntries, "auto");
