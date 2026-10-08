#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Gate routine at 0x696B60: __thiscall(this, a2), pops 4 bytes.
CSF_FUNC(int, __fastcall, MenuAccount_GateCheck_696B60, 0x00696B60, (void* self, void* edx, void* a2));

// Global byte used as the gate argument.
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// Returns 1 when the 16-bit character array of the object (inline buffer at +4 when
// the capacity field at +0x18 is below 8, otherwise the heap pointer stored at +4)
// consists solely of 0x20 (space) characters, counting +0x14 elements.
// Returns 0 if the gate routine reports a nonzero status.
static char __cdecl MenuAccount_IsNameAllSpaces(uint32_t* a1)
{
    char result = static_cast<char>(
        MenuAccount_GateCheck_696B60(a1, nullptr, &g_byte_82E65C));

    if (!result)
    {
        uint32_t elementCount = CSF_AT(uint32_t, a1, 0x14);
        uint32_t index = 0;
        result = 1;
        while (index < elementCount)
        {
            if (!result)
                break;

            uint8_t* charBase;
            if (CSF_AT(uint32_t, a1, 0x18) < 8u)
                charBase = reinterpret_cast<uint8_t*>(a1) + 4;
            else
                charBase = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, a1, 0x04));

            if (*reinterpret_cast<uint16_t*>(charBase + index * 2) != 32)
                result = 0;

            ++index;
        }
    }
    return result;
}

HOOK(0x0055C900, MenuAccount_IsNameAllSpaces, "auto");
