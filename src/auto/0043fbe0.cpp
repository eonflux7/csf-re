#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));

static C_0082d410* __cdecl CopyMenuPropertyText(S_00692b80* a1, S_00692b80* keyword, C_0082d410* a3)
{
    C_0082d410* result = nullptr;
    alignas(8) char buffer[256];
    std::memset(buffer, 0, sizeof(buffer));

    if (a1)
    {
        std::uint8_t* owner = reinterpret_cast<std::uint8_t*>(a1);
        char* keywordText = reinterpret_cast<char*>(keyword);
        if (FindProperty(owner, nullptr, keywordText))
            MenuExtras_ReadText(owner, nullptr, keywordText, buffer, 256);

        unsigned int length = 0;
        while (buffer[length] != '\0')
            ++length;

        return reinterpret_cast<C_0082d410*>(
            AssignStringRange_429B60(a3, nullptr, buffer, length));
    }
    return result;
}

HOOK(0x0043FBE0, CopyMenuPropertyText, "auto");
