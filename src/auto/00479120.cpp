#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_008317c8_AttachActionState, 0x00478420, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008317c8_DetachActionState, 0x00478480, (void* self, void* edx));

static void __fastcall C_008317c8_ResetActionState(
    C_008317c8* self, void* /*edx*/, C_00862490* actionState,
    uint32_t stateData)
{
    auto* previousState = reinterpret_cast<C_00862490*>(self->field_0070);
    if (previousState == actionState)
        return;

    if (previousState &&
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, previousState, 0)[0xA8 / 4])(previousState, nullptr))
    {
        self->vSlotE0();
        C_008317c8_DetachActionState(self, nullptr);
    }

    self->field_0070 = reinterpret_cast<uint32_t>(actionState);
    self->field_0074 = stateData;

    if (!actionState)
        return;

    if (!((int(__fastcall*)(void*, void*))
            CSF_AT(void**, actionState, 0)[0xA8 / 4])(actionState, nullptr))
        return;

    C_008317c8_AttachActionState(self, nullptr);
    self->vSlotDC(0);

    if (self->field_006c & 0x80000000)
        return;

    void* resolvedObject = reinterpret_cast<void*>(CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x64)));
    if (!resolvedObject)
        return;

    void* component =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, resolvedObject, 0)[0x28 / 4])(
                resolvedObject, nullptr);

    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, component, 0)[0x3C / 4])(component, nullptr, 1);

    auto* currentState = reinterpret_cast<C_00862490*>(self->field_0070);
    void* lookupObject = reinterpret_cast<void*>(
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, currentState, 0)[0xA8 / 4])(currentState, nullptr));

    void** lookupVtable = CSF_AT(void**, lookupObject, 0);
    int entry =
        ((int(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, resolvedObject, 0)[0x28 / 4])(
                resolvedObject, nullptr, 100, 0);

    ((void(__fastcall*)(void*, void*, int))
        lookupVtable[0xA0 / 4])(lookupObject, nullptr, entry);
}

HOOK(0x00479120, C_008317c8_ResetActionState, "auto");
