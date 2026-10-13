#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));

CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static int __cdecl CoreFiles_SetOptionTextFromCString(const char* text)
{
    if (text)
    {
        void* optionsTextSlot = reinterpret_cast<uint8_t*>(g_pOptions) + 4;
        size_t textLength = strlen(text);
        return static_cast<int>(reinterpret_cast<uintptr_t>(
            AssignStringRange_429B60(optionsTextSlot, nullptr,
                                     const_cast<char*>(text),
                                     static_cast<unsigned int>(textLength))));
    }

    void* optionsTextSlot = reinterpret_cast<uint8_t*>(g_pOptions) + 4;
    return static_cast<int>(reinterpret_cast<uintptr_t>(
        AssignStringRange_429B60(optionsTextSlot, nullptr,
                                 reinterpret_cast<char*>(&g_byte_82E65C), 0u)));
}

HOOK(0x697C00, CoreFiles_SetOptionTextFromCString, "auto");
