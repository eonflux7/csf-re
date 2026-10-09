#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));

static int __fastcall SceneLoader_GetModelSummary(
    void* self, void* /*edx*/, C_0082d410* output)
{
    char* firstName = nullptr;
    std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x30);

    if (begin &&
        static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, self, 0x34) - begin) / 88)
    {
        auto* firstEntry = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, self, 0x30));

        if (CSF_AT(std::uint32_t, firstEntry, 0x18) < 0x10u)
            firstName = reinterpret_cast<char*>(firstEntry + 4);
        else
            firstName = CSF_AT(char*, firstEntry, 4);
    }

    const unsigned int nameLength =
        static_cast<unsigned int>(std::strlen(firstName));
    AssignStringRange_429B60(
        reinterpret_cast<std::uint8_t*>(output) + 28,
        nullptr, firstName, nameLength);
    AssignStringSubstring(
        output, nullptr,
        reinterpret_cast<std::uint8_t*>(self) + 124, 0, -1);

    begin = CSF_AT(std::uint32_t, self, 0x30);
    int count = 0;
    if (begin)
    {
        count = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, self, 0x34) - begin) / 88;
    }

    output[4].field_0008 = static_cast<std::uint32_t>(count);
    output[5].vtbl = CSF_AT(void**, self, 0x0C);
    return count;
}

HOOK(0x00501EE0, SceneLoader_GetModelSummary, "auto");
