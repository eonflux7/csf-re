#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
CSF_FUNC(int, __fastcall, HudStringShow_AssignEntry, 0x004F2AD0,
         (C_0082d410* self, void*, int sourceEntry));
// Cdecl range helper; the element assignment itself pops its one argument.
static int __cdecl Assign_004f2ec0(uint8_t* first, uint8_t* last, int source) {
    int result = 0; // No specified return value for an empty range.
    for (; first != last; first += 0x7C)
        result = HudStringShow_AssignEntry(reinterpret_cast<C_0082d410*>(first), nullptr, source);
    return result;
}
HOOK(0x004F2EC0, Assign_004f2ec0, "stl");
