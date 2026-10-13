#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00860a30_SetFlag0C, 0x006C9FE0, (C_00860a30* self, void* /*edx*/, int flagValue));

// Sets the 0x0C flag when the forced argument is nonzero, or when the virtual
// predicate HasBit1OfField0014 reports true. The original only tests the low byte
// of a2 (bl) and the low byte of the predicate result (al).
static void __fastcall C_00862640_SetFlag0CIfForcedOrHasBit1(C_00862640* self, void* /*edx*/, int a2)
{
    const bool forcedLowByte = (static_cast<uint8_t>(a2) != 0);
    if (forcedLowByte || (static_cast<uint8_t>(self->vHasBit1OfField0014()) != 0))
    {
        C_00860a30_SetFlag0C(reinterpret_cast<C_00860a30*>(self), nullptr, a2);
    }
}

HOOK(0x006F2C70, C_00862640_SetFlag0CIfForcedOrHasBit1, "auto");
