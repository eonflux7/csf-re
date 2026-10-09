#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CCharActionFsm_UpdateWeaponActionState, 0x004849A0, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, CCharActionFsm_GetSelectedEntryOrZero, 0x00485B30, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, CCharActionFsm_GetIndexedEntryOrZero, 0x00485B50, (CCharActionFsm* self, void*));
CSF_FUNC(bool, __fastcall, Character_CanChangeMode, 0x00493C70, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SetMenuContextState, 0x0049A0E0, (void* self, void* edx, int a4, char a5));
CSF_FUNC(char*, __fastcall, ObjectProvider_GetCurrentEntry, 0x004F4E90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ObjectProvider_SetUpdateMode, 0x004F7810, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static char __fastcall C_008342e8_EnableSpecialMode(C_008342e8* self, void* /*edx*/)
{
    if (!Character_CanChangeMode(self, nullptr))
        return 0;

    CCharActionFsm* actionFsm = reinterpret_cast<CCharActionFsm*>(self->field_0044);

    // vtable slot +0x90: is the action state machine ready?
    bool actionReady = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, actionFsm, 0)[0x90 / 4])(actionFsm, nullptr);
    if (!actionReady)
        return 0;

    int selectedEntry = CCharActionFsm_GetSelectedEntryOrZero(actionFsm, nullptr);
    if (selectedEntry != 0)
        return (char)selectedEntry;

    int indexedEntry = CCharActionFsm_GetIndexedEntryOrZero(actionFsm, nullptr);
    if (indexedEntry != 0)
        return (char)indexedEntry;

    void* firstElement = reinterpret_cast<void*>(
        GetFirstElement(reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr));
    void* objectProvider = GetCachedObjectProvider(firstElement, nullptr);

    char* currentEntry = ObjectProvider_GetCurrentEntry(objectProvider, nullptr);
    if (CSF_AT(int, currentEntry, 8) < 0)
        return (char)reinterpret_cast<uintptr_t>(currentEntry);

    CCharActionFsm_UpdateWeaponActionState(actionFsm, nullptr);

    CSF_AT(uint32_t, self, 0x2E0) |= 0x1000u;
    SetMenuContextState(self, nullptr, 1, 0);

    self->vSlot8C(0, 0, -1, 0);
    self->vSlot94();

    if ((CSF_AT(uint32_t, self, 0x2E0) & 0x2000) != 0)
        CSF_AT(uint8_t, self, 0x3A5) = 0;

    return (char)ObjectProvider_SetUpdateMode(objectProvider, nullptr, 8);
}

HOOK(0x0049A8A0, C_008342e8_EnableSpecialMode, "auto");
