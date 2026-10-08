#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10,
         (void* self, void* edx));

static std::uint32_t __fastcall InvokeLazyIndexedResource(
    void* self, void* /*edx*/, std::uint32_t index, int argument)
{
    const std::uint32_t beginAddress =
        CSF_AT(std::uint32_t, self, 0x19C);
    if (beginAddress == 0)
        return 0;

    const std::uint32_t byteCount =
        CSF_AT(std::uint32_t, self, 0x1A0) - beginAddress;
    const std::int32_t count =
        static_cast<std::int32_t>(byteCount) / 12;

    if (index >= static_cast<std::uint32_t>(count))
        return static_cast<std::uint32_t>(count);

    const std::uint32_t byteOffset = index * 12u;
    const std::uint32_t initialEntryAddress =
        CSF_AT(std::uint32_t, self, 0x19C) + byteOffset;

    if (CSF_AT(void*, reinterpret_cast<void*>(initialEntryAddress), 8) == nullptr)
    {
        void* factory = GetCachedObjectProvider(self, nullptr);
        const std::uint32_t destinationAddress =
            CSF_AT(std::uint32_t, self, 0x19C) + byteOffset;

        void* resource =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, factory, 0)[0x48 / 4])(factory, nullptr);

        CSF_AT(void*, reinterpret_cast<void*>(destinationAddress), 8) = resource;
    }

    const std::uint32_t currentEntryAddress =
        CSF_AT(std::uint32_t, self, 0x19C) + byteOffset;
    void* resource =
        CSF_AT(void*, reinterpret_cast<void*>(currentEntryAddress), 8);

    return ((std::uint32_t (__fastcall*)(void*, void*, int))
        CSF_AT(void**, resource, 0)[0x2C / 4])(resource, nullptr, argument);
}

HOOK(0x007155E0, InvokeLazyIndexedResource, "auto");
