#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall AiAutonomous_SetCharacterCombatOption(void* self, void* /*edx*/, int combatOption)
{
    CSF_AT(int, self, 0x1DC) = combatOption;
    return combatOption;
}

HOOK(0x492750, AiAutonomous_SetCharacterCombatOption, "auto");
