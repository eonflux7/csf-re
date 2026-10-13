#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, Text_ConvertToDisplayEncoding, 0x00692580, (int text, int flag));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static void __stdcall HudObjectives_SetFirstNonEmptyTitle(int a1, int a2)
{
    if (a1 == 0)
        return;

    void* self = reinterpret_cast<void*>(a1);

    // Vtable slot 15 (byte offset 0x3C): called with the flag byte's address.
    ((void(__fastcall*)(void*, void*, uint8_t*))CSF_AT(void**, self, 0)[15])(self, nullptr, &g_byte_82E65C);

    if (a2 == 0)
        return;

    const int candidateFlags[2] = {4, 8};

    for (int i = 0; i < 2; ++i)
    {
        uint16_t* displayText = reinterpret_cast<uint16_t*>(Text_ConvertToDisplayEncoding(a2, candidateFlags[i]));
        if (displayText != nullptr && *displayText != 0)
        {
            // Vtable slot 16 (byte offset 0x40): set the first non-empty display text.
            ((void(__fastcall*)(void*, void*, uint16_t*))CSF_AT(void**, self, 0)[16])(self, nullptr, displayText);
            return;
        }
    }
}

HOOK(0x4277F0, HudObjectives_SetFirstNonEmptyTitle, "auto");
