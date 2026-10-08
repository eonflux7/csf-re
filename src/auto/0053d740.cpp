#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, GetGlobalState, 0x0050D190, ());
CSF_FUNC(void, __fastcall, GlobalState_SetFlagA, 0x0050DC80, (void* self, void* edx, char flag));
CSF_FUNC(void, __fastcall, CMenuMissionInfo_ResetBichosPhotoFlags, 0x0053D530, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CSubObject_ReleaseChild, 0x00436770, (void** self, void* edx));

static int __fastcall CMenuMissionInfo_EnterPanelState(CMenuMissionInfo* self, void* /*edx*/, int /*a3*/)
{
    // Si el estado global tiene el flag en +0x89, lo limpia (sub_50DC80(state, 1)).
    if (CSF_AT(uint8_t, GetGlobalState(), 0x89))
    {
        void* globalState = GetGlobalState();
        GlobalState_SetFlagA(globalState, nullptr, 1);
    }

    CMenuMissionInfo_ResetBichosPhotoFlags(self, nullptr);

    // Llamada virtual sobre el objeto en +0x10 (slot 0x60/4 = 24) con (self, 1).
    void* owner = reinterpret_cast<void*>(self->field_0010);
    void** ownerVtbl = *reinterpret_cast<void***>(owner);
    reinterpret_cast<void(__fastcall*)(void*, void*, CMenuMissionInfo*, int)>(ownerVtbl[24])(
        owner, nullptr, self, 1);

    // Objeto de estado global +0x10: si existe, llamada virtual slot 36/4 = 9.
    void* globalState = GetGlobalState();
    void* globalChild = reinterpret_cast<void*>(CSF_AT(uint32_t, globalState, 0x10));
    if (globalChild)
    {
        void** childVtbl = *reinterpret_cast<void***>(globalChild);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(childVtbl[9])(globalChild, nullptr);
    }

    // Resultado: campo +0x1c; si no es nulo, llamada virtual slot 0x16c/4 = 91.
    int result = static_cast<int>(self->field_001c);
    if (result)
    {
        void* panel = reinterpret_cast<void*>(self->field_001c);
        void** panelVtbl = *reinterpret_cast<void***>(panel);
        result = reinterpret_cast<int(__fastcall*)(void*, void*)>(panelVtbl[91])(panel, nullptr);
    }

    // Campo +0x84: si no es nulo, libera/cierra el subobjeto con sub_436770.
    void** childObject = reinterpret_cast<void**>(self->field_0084);
    if (childObject)
        return CSubObject_ReleaseChild(childObject, nullptr);

    return result;
}

HOOK(0x0053D740, CMenuMissionInfo_EnterPanelState, "auto");
