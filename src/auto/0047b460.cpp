#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bind as given in the packet: __thiscall callee with one stack argument, pops 4.
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// Returns CharTask_LookupByOffset(this, this->field_000c + 40).
static int __fastcall C_00834fa8_LookupFromBaseOffset(C_00834fa8* self, void* /*edx*/)
{
    int offsetValue = CSF_AT(int, self, 0x0C);
    return CharTask_LookupByOffset(self, nullptr, offsetValue + 40);
}

HOOK(0x0047B460, C_00834fa8_LookupFromBaseOffset, "auto");
