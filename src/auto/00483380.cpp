#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __cdecl, sub_761040, 0x00761040, (int a1));

// Returns non-zero when the char action FSM is in state 3 or 5 (weapon attack/reload)
// or when the attached sub-object reports busy through its field-74 query and sub_761040.
static char __fastcall C_008319c0_IsAttackOrReloadBusy(C_008319c0* self, void* /*edx*/)
{
    char result;
    const int currentState = CSF_AT(int, self, 0x0C);

    result = (currentState == 3 || currentState == 5) ? 1 : 0;

    C_008319c0* subObject = CSF_AT(C_008319c0*, self, 0xA4);
    if (subObject)
    {
        if (result)
            return 1;

        const int subObjectField74 = C_008319c0_GetSubObjectField74(subObject, nullptr);
        result = static_cast<char>(sub_761040(subObjectField74));
        if (result)
            return 1;
    }
    return result;
}

HOOK(0x00483380, C_008319c0_IsAttackOrReloadBusy, "auto");
