#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Character_SetActionMode, 0x00492760, (void* self, void* edx, int a2));

// Returns the character object owned by this autonomo task (owner at +0x08, character at owner+0x14).
static void* TAutonomo_GetOwnerCharacter(TAutonomo* self)
{
    void* owner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08));
    return CSF_AT(void*, owner, 0x14);
}

static int __fastcall TAutonomo_ApplyStanceAndActionMode(TAutonomo* self, void* /*edx*/)
{
    CoverAttack_SetStance_492720(TAutonomo_GetOwnerCharacter(self), nullptr, static_cast<int>(self->field_0078));
    Character_SetActionMode(TAutonomo_GetOwnerCharacter(self), nullptr, static_cast<int>(self->field_0080));

    void* character = TAutonomo_GetOwnerCharacter(self);
    if (CSF_AT(int, character, 0x1C4) == 1)
        Character_SetActionMode(TAutonomo_GetOwnerCharacter(self), nullptr, 0);

    int result = static_cast<int>(self->field_0084);
    if (result != -1)
    {
        void* actor = CSF_AT(void*, TAutonomo_GetOwnerCharacter(self), 0x44);
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, actor, 0)[196 / 4])(actor, nullptr, result);
        if (result != -1)
        {
            actor = CSF_AT(void*, TAutonomo_GetOwnerCharacter(self), 0x44);
            return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, actor, 0)[248 / 4])(actor, nullptr, result);
        }
    }
    return result;
}

HOOK(0x4D73E0, TAutonomo_ApplyStanceAndActionMode, "auto");
