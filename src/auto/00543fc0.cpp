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

// Base constructor CMenuGameOptions (thiscall, one stack argument, ret 4).
CSF_FUNC(char*, __fastcall, CMenuGameOptions_Construct, 0x0053B6F0, (void* self, void* edx, int a2));

// Constructor of CMenuGameOptionsPC. Calls the base constructor, installs its two
// vtables (primary at +0x00 = 0x008439B8, secondary base at +0x04 = 0x00843968),
// zeroes +0x58, sets the float at +0x5C to 0.35f (bits 0x3EB33333), sets the byte at
// +0x60 to 1, and returns this in eax. Pops its one stack argument (ret 4).
static void* __fastcall CMenuGameOptionsPC_Construct(void* self, void* /*edx*/, int a2)
{
    CMenuGameOptions_Construct(self, nullptr, a2);

    CSF_AT(uint32_t, self, 0x000) = CSF_ADDR_VT(0x008439B8);
    CSF_AT(uint32_t, self, 0x004) = 0x00843968;
    CSF_AT(uint32_t, self, 0x058) = 0;
    CSF_AT(uint32_t, self, 0x05C) = 0x3EB33333;
    CSF_AT(uint8_t, self, 0x060) = 1;

    return self;
}

HOOK(0x00543FC0, CMenuGameOptionsPC_Construct, "auto");
