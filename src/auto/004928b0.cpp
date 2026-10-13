#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __fastcall, C_00833908_DeactivateAndDispatchRecord, 0x0048FC70, (C_00833908* self, void* , int source));
CSF_FUNC(char, __fastcall, IsHudSuspended, 0x00495520, (C_00833908* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(void, __fastcall, C_00833908_SendCharacterEvent, 0x00525B30, (C_00833908* self, void* , int character, int eventType));
CSF_FUNC(int, __cdecl, CharActionFsm_CheckSubObjectState, 0x00761330, (int a1));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_00833b88_DeactivateAndNotifyCharacter(
    C_00833b88* self, void* /*edx*/, int source)
{
    C_00833908_DeactivateAndDispatchRecord(self, nullptr, source);

    int result = static_cast<int>(self->field_004c);
    if (result != 0)
    {
        result = GetGlobalState_50D190();
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0xD0) == 0)
        {
            void* child = reinterpret_cast<void*>(self->field_004c);
            result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, child, 0)[0x2C / 4])(child, nullptr, 0);
        }
    }

    if (self->field_0050 != 1)
        return result;

    result = CSF_AT(int, reinterpret_cast<void*>(source), 0x8);
    if (result < 0)
        return result;

    result = CHandleTable_Resolve(g_pCurrentScene, nullptr, source);
    if (result == 0)
        return result;

    C_00833908* character = reinterpret_cast<C_00833908*>(result);
    result = static_cast<int>(character->field_0050);
    if (result != 0)
        return result;

    void* controller = reinterpret_cast<void*>(character->field_0044);
    result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, controller, 0)[0xDC / 4])(controller, nullptr);
    if (result == 0)
        return result;

    C_008319c0* weapon = reinterpret_cast<C_008319c0*>(result);
    int subObject = C_008319c0_GetSubObjectField74(weapon, nullptr);
    result = CharActionFsm_CheckSubObjectState(subObject);
    if (static_cast<uint8_t>(result) == 0)
        return result;

    result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, weapon, 0)[0x9C / 4])(weapon, nullptr);
    if (static_cast<uint8_t>(result) == 0)
        return result;

    const int eventType = IsHudSuspended(character, nullptr) ? 104 : 103;
    C_00833908* eventManager =
        reinterpret_cast<C_00833908*>(Get_008b0678());
    C_00833908_SendCharacterEvent(
        eventManager, nullptr, reinterpret_cast<int>(character), eventType);

    // The final callee is void; there is no defined result on this path.
    return 0;
}

HOOK(0x004928B0, C_00833b88_DeactivateAndNotifyCharacter, "auto");
