#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall(this, a2, a3), callee pops 8 bytes.
// Sets one integer rule option of the game-mode object, chosen by a2.
static int __fastcall MpGameModes_SetIntegerRuleOption(void* self, void* /*edx*/, int optionId, int value)
{
    int result = optionId - 20;

    switch (optionId)
    {
    case 20:
        CSF_AT(int, self, 0x40) = value;
        break;
    case 21:
        CSF_AT(int, self, 0x48) = value;
        result = value;
        break;
    case 29:
        CSF_AT(int, self, 0x3C) = value;
        break;
    case 30:
        CSF_AT(int, self, 0x44) = value;
        result = value;
        break;
    default:
        return result;
    }
    return result;
}

HOOK(0x7440E0, MpGameModes_SetIntegerRuleOption, "auto");
