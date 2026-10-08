#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// std::_String_base::assign-style helper at 0x4109E0.
// __thiscall: ecx = destination element, stack = (source, 0, -1), callee pops 12.
CSF_FUNC(void*, __fastcall, StringBase_AssignFrom, 0x4109E0,
         (void* self, void* edx, void* source, int zero, int minusOne));

// Fills every 36-byte slot in [first, last) with the string from `source`
// and copies the five trailing flag bytes (offsets 0x1C..0x20) from `source`.
// Returns the value of source[0x1F] as loaded in the final iteration.
static char __cdecl MenuSaveGame_FillSlotRange(uint8_t* first, uint8_t* last, uint8_t* source)
{
    char result = 0;
    for (uint8_t* slot = first; slot != last; slot += 36)
    {
        StringBase_AssignFrom(slot, nullptr, source, 0, -1);

        CSF_AT(uint8_t, slot, 0x1C) = CSF_AT(uint8_t, source, 0x1C);
        CSF_AT(uint8_t, slot, 0x1D) = CSF_AT(uint8_t, source, 0x1D);
        CSF_AT(uint8_t, slot, 0x1E) = CSF_AT(uint8_t, source, 0x1E);
        result = static_cast<char>(CSF_AT(uint8_t, source, 0x1F));
        CSF_AT(uint8_t, slot, 0x1F) = static_cast<uint8_t>(result);
        CSF_AT(uint8_t, slot, 0x20) = CSF_AT(uint8_t, source, 0x20);
    }
    return result;
}

HOOK(0x544D80, MenuSaveGame_FillSlotRange, "auto");
