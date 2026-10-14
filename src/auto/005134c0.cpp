#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
CSF_FUNC(int, __fastcall, S_004f21d0_AssignFromRecord, 0x00512D80,
         (S_004f21d0* self, void*, S_004f21d0* record));
static int __cdecl Assign_005134c0(uint8_t* first, uint8_t* last, S_004f21d0* source) {
    int result = 0; // No specified return value for an empty range.
    for (; first != last; first += 0xC8)
        result = S_004f21d0_AssignFromRecord(reinterpret_cast<S_004f21d0*>(first), nullptr, source);
    return result;
}
HOOK(0x005134C0, Assign_005134c0, "stl");
