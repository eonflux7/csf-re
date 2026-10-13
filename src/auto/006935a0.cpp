#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

// The packet labels the direct capacity-growth callee as a vtable reference.
static const auto kAttributeArrayCapacityReference = CSF_VTBL(0x0041E5F0);

static int __fastcall CorePaths_InternAttributeText(
    void* self, void* /*edx*/, char* text, char allowInsert)
{
    std::uint32_t index = 0;
    if (CSF_AT(std::uint32_t, self, 0x34) != 0)
    {
        do
        {
            if (__stricmp(text, CSF_AT(char**, self, 0x2C)[index]) == 0)
                return static_cast<int>(index);

            ++index;
        } while (index < CSF_AT(std::uint32_t, self, 0x34));
    }

    if (!allowInsert)
        return -1;

    char* ownedText = reinterpret_cast<char*>(
        CoreMemory_AllocUntagged(std::strlen(text) + 1));
    std::strcpy(ownedText, text);

    if (CSF_AT(std::uint32_t, self, 0x34) >=
        CSF_AT(std::uint32_t, self, 0x30))
    {
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                static_cast<std::uint8_t*>(self) + 0x2C),
            nullptr,
            CSF_AT(int, self, 0x38));
    }

    index = CSF_AT(std::uint32_t, self, 0x34);
    char** destination = CSF_AT(char**, self, 0x2C) + index;
    CSF_AT(std::uint32_t, self, 0x34) = index + 1;
    *destination = ownedText;

    return static_cast<int>(CSF_AT(std::uint32_t, self, 0x34) - 1);
}

HOOK(0x006935A0, CorePaths_InternAttributeText, "auto");
