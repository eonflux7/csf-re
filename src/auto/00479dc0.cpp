#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharActionFsm_GetCharacterToggleState, 0x00496DC0, (CPlayerCharacter* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_008318c8_ApplyCharacterWeaponAction(
    C_008318c8* self, void* /*edx*/)
{
    char enabled = 1;
    void* values;
    if (self->field_0070)
        values = reinterpret_cast<void*>(
            Handle_GetValuePointer(reinterpret_cast<void*>(self->field_0070), nullptr));
    else
        values = reinterpret_cast<void*>(self->field_0040);

    const int defaultAction = CSF_AT(int, values, 0x130);
    const uint32_t character = self->field_0004;
    int action = defaultAction;

    if (character == GetSceneMenuContext(g_pCurrentScene, nullptr))
    {
        if (static_cast<uint8_t>(CharActionFsm_GetCharacterToggleState(
                reinterpret_cast<CPlayerCharacter*>(character), nullptr)))
            enabled = 0;

        action = CSF_AT(int, values, 0x138);
        if (action == -1)
            action = defaultAction;
    }

    return self->vSlot7C(action, 1000002u, enabled, 1);
}

HOOK(0x00479DC0, C_008318c8_ApplyCharacterWeaponAction, "auto");
