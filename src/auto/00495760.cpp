#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base initializer bound by address (__thiscall callee, one stack arg, ret 4).
CSF_FUNC(int, __fastcall, CPlayerCharacter_InitBase, 0x00492E90, (void* self, void* edx, int* a2));

// Initializes the base part of the player character with the given argument,
// then sets the character's state word to 2 and clears the byte flag at +0x404.
static int __fastcall CPlayerCharacter_InitFromArgAndSetState(CPlayerCharacter* self, void* /*edx*/, int* a2)
{
    int result = CPlayerCharacter_InitBase(self, nullptr, a2);
    self->field_0400 = 2;
    self->field_0404 = 0;
    return result;
}

HOOK(0x00495760, CPlayerCharacter_InitFromArgAndSetState, "auto");
