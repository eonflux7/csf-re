#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CScene_AddObjectFlags, 0x00452800, (CScene* self, void* , int handleAddress, int flags));
CSF_FUNC(int, __fastcall, CScene_RemoveObjectFromCategories, 0x004528D0, (CScene* self, void* , int handleAddress, int categoryMask));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall CPlayerCharacter_SetInteractionState(
    CPlayerCharacter* self, void* /*edx*/, int state)
{
    int result = state;
    const uint8_t requestedState = static_cast<uint8_t>(state);

    if (static_cast<uint8_t>((self->field_0170 & 0x10) == 16) != requestedState)
    {
        if (requestedState)
            self->field_0170 |= 0x10;
        else
            self->field_0170 &= 0xFFFFFFEF;

        void* linkedObject = reinterpret_cast<void*>(self->field_0044);
        if (linkedObject)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, linkedObject, 0)[0x88 / 4])(
                    linkedObject, nullptr, state);
        }

        const uint32_t shiftedFlags = self->field_0094 >> 3;
        result = static_cast<int>(shiftedFlags);
        if ((shiftedFlags & 1) == 0)
        {
            if ((self->field_0170 & 0x10) != 0)
            {
                return CScene_AddObjectFlags(
                    static_cast<CScene*>(g_pCurrentScene), nullptr,
                    reinterpret_cast<int>(
                        reinterpret_cast<uint8_t*>(self) + 0x1C),
                    1);
            }
            else if ((self->field_0094 & 1) != 0)
            {
                return CScene_RemoveObjectFromCategories(
                    static_cast<CScene*>(g_pCurrentScene), nullptr,
                    reinterpret_cast<int>(
                        reinterpret_cast<uint8_t*>(self) + 0x1C),
                    1);
            }
        }
    }

    return result;
}

HOOK(0x0048EEC0, CPlayerCharacter_SetInteractionState, "auto");
