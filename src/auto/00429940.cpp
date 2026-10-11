#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, CheatMenu_InitBase, 0x0042A3B0, (unsigned int a1, unsigned int uCode, void* a3));

// Runs the base initializer, then upper-cases every UTF-16 code unit of the
// buffer: ASCII a-z, Latin-1 accented letters E0-FD (except F7 is not special-cased
// by the original), and leaves 0xFF untouched. Returns the buffer pointer.
static uint32_t* __stdcall CheatMenu_UppercaseWideBuffer(unsigned int a1, unsigned int uCode, uint32_t* a3)
{
    CheatMenu_InitBase(a1, uCode, a3);

    uint32_t length = CSF_AT(uint32_t, a3, 0x14);
    for (unsigned int i = 0; i < length; ++i)
    {
        // Inline storage when capacity < 8, otherwise heap pointer at +4.
        uint16_t* chars;
        if (CSF_AT(uint32_t, a3, 0x18) < 8u)
            chars = reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(a3) + 4);
        else
            chars = reinterpret_cast<uint16_t*>(CSF_AT(uint32_t, a3, 4));

        uint16_t ch = chars[i];
        uint16_t upper = ch;
        if (ch >= 0x61u && ch <= 0x7Au)
            upper = static_cast<uint16_t>(ch - 32);
        if (ch >= 0xE0u && ch <= 0xFDu)
            upper = static_cast<uint16_t>(ch - 32);
        if (ch == 0xFFu)
            upper = 0xFF;
        chars[i] = upper;
    }
    return a3;
}

HOOK(0x00429940, CheatMenu_UppercaseWideBuffer, "auto");
