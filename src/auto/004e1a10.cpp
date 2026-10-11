#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Character_DisableSpecialMode, 0x0049A970, (void* self, void*));
CSF_FUNC(int, __fastcall, C_0082f9f8_TryAcquire, 0x0048E5D0, (void* self, void* edx, void* owner, int mode, int priority));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_InvokeAction, 0x0048E530, (void* self, void* edx, int action));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_004f5420_SetProviderState, 0x004F5610, (S_004f5420* self, void* , int state));
CSF_FUNC(void, __fastcall, GlobalValue4FBDE0_SetToggleMode, 0x004FC210, (void* self, void* edx, int a3));

static int __fastcall C_0083e5e8_Activate(C_0083e5e8* self, void* /*edx*/, int argument)
{
    Character_DisableSpecialMode(reinterpret_cast<void*>(self->field_000c), nullptr);

    void* character = reinterpret_cast<void*>(self->field_000c);
    self->field_0008 = 0;
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, character, 0)[0x58 / 4])(
        character, nullptr, argument);

    C_0082f9f8_TryAcquire(
        reinterpret_cast<void*>(self->field_000c), nullptr, self, 0, 100);

    int result;
    if (static_cast<uint8_t>(SceneObject_PassesFirstCheck(
            reinterpret_cast<void*>(self->field_000c), nullptr)) != 0)
    {
        result = SceneObject_InvokeAction(
            reinterpret_cast<void*>(self->field_000c), nullptr, 48);
    }
    else
    {
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_000c), 0x228) == 0)
        {
            int globalValue = GetGlobalValue_4FBDE0();
            int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
            void* provider = GetCachedObjectProvider(
                reinterpret_cast<void*>(firstElement), nullptr);
            S_004f5420_SetProviderState(
                static_cast<S_004f5420*>(provider), nullptr, 6);

            globalValue = GetGlobalValue_4FBDE0();
            GlobalValue4FBDE0_SetToggleMode(
                reinterpret_cast<void*>(globalValue), nullptr, 3);
        }

        character = reinterpret_cast<void*>(self->field_000c);
        result = ((int(__fastcall*)(void*, void*, int, int, int, int))
            CSF_AT(void**, character, 0)[0x8C / 4])(
                character, nullptr, 10, 0, -1, 0);
    }

    CSF_AT(uint32_t, self, 0x18) = 4;
    return result;
}

HOOK(0x004E1A10, C_0083e5e8_Activate, "auto");
