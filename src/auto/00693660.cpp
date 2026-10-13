#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static std::int16_t __fastcall CorePaths_InternComponentName(
    void* self, void* /*edx*/, char* componentName, char createIfMissing)
{
    if (std::strlen(componentName) == 0)
        return -1;

    std::uint16_t index = 0;
    if (CSF_AT(std::uint16_t, self, 0x24) != 0)
    {
        do
        {
            if (__stricmp(componentName, CSF_AT(char**, self, 0x1C)[index]) == 0)
                return static_cast<std::int16_t>(index);

            ++index;
        }
        while (index < CSF_AT(std::uint16_t, self, 0x24));
    }

    if (!createIfMissing)
        return -1;

    char* storedName = reinterpret_cast<char*>(
        CoreMemory_AllocUntagged(std::strlen(componentName) + 1));
    std::strcpy(storedName, componentName);

    if (CSF_AT(std::uint32_t, self, 0x24) >=
        CSF_AT(std::uint32_t, self, 0x20))
    {
        // The packet also identifies this address as a vtable reference.
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<std::uint8_t*>(self) + 0x1C),
            nullptr,
            CSF_AT(int, self, 0x28));
    }

    const std::uint32_t insertionIndex = CSF_AT(std::uint32_t, self, 0x24);
    char** destination = CSF_AT(char**, self, 0x1C) + insertionIndex;
    CSF_AT(std::uint32_t, self, 0x24) = insertionIndex + 1;
    *destination = storedName;

    return static_cast<std::int16_t>(
        CSF_AT(std::uint32_t, self, 0x24) - 1);
}

HOOK(0x00693660, CorePaths_InternComponentName, "auto");
