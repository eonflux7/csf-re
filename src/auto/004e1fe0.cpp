#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Character_DisableSpecialMode, 0x0049A970, (void* self, void*));
CSF_FUNC(char, __fastcall, C_0083e160_CompleteActivation, 0x004E1D70, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_004f5420_SetProviderState, 0x004F5610, (S_004f5420* self, void* , int state));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void, __fastcall, GlobalValue4FBDE0_SetToggleMode, 0x004FC210, (void* self, void* edx, int a3));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0082f9f8_TryAcquire, 0x0048E5D0, (void* self, void* edx, void* a2, int a3, int a4));

static char __fastcall C_0083e5e8_ActivateSpecialModeSequence(C_0083e5e8* self, void* /*edx*/, int arg0)
{
    Character_DisableSpecialMode(reinterpret_cast<void*>(self->field_000c), nullptr);

    void* character = reinterpret_cast<void*>(self->field_000c);
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, character, 0)[24])(character, nullptr, arg0);

    character = reinterpret_cast<void*>(self->field_000c);
    if (CSF_AT(uint8_t, character, 0x228) != 0) {
        CSF_AT(uint8_t, self, 0x1C) = 0;
    } else {
        int globalValue = GetGlobalValue_4FBDE0();
        int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
        void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);
        S_004f5420_SetProviderState(reinterpret_cast<S_004f5420*>(provider), nullptr, 6);

        int toggleOwner = GetGlobalValue_4FBDE0();
        GlobalValue4FBDE0_SetToggleMode(reinterpret_cast<void*>(toggleOwner), nullptr, 3);
        CSF_AT(uint8_t, self, 0x1C) = 1;
    }

    C_0082f9f8_TryAcquire(reinterpret_cast<void*>(self->field_000c), nullptr, self, 0, 100);

    character = reinterpret_cast<void*>(self->field_000c);
    ((void(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, character, 0)[35])(
        character, nullptr, 49, 0, -1, 0);

    CSF_AT(int, self, 0x18) = 2;
    return C_0083e160_CompleteActivation(self, nullptr);
}

HOOK(0x4E1FE0, C_0083e5e8_ActivateSpecialModeSequence, "auto");
