#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, S_00508360_ReinitializeAndStoreResult, 0x00508CB0, (S_00508360* self, void* , int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __stdcall, OnlineMenu_SetSharedText, 0x00521E30, (int a1));
CSF_FUNC(void, __fastcall, AudioSoundManager_ForwardToCurrentIfQuerySucceeds, 0x00521E80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuPause_CenterOnTarget, 0x0052F950, (CMenuPause* self, void*));
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_69B700, 0x0069B700, ());
CSF_FUNC(void, __fastcall, CTextDatabase_SetTextDatabasePath, 0x0069BD90, (CTextDatabase* self, void* , unsigned int mask, char* path));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static uint32_t __fastcall CMenuCredits_Open(
    CMenuCredits* self, void* /*edx*/, uint32_t owner, int /*unused*/)
{
    if (!self->field_0018)
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        reinterpret_cast<void (__fastcall*)(void*, void*, CMenuCredits*)>(
            CSF_AT(void**, state, 0)[0x18 / 4])(state, nullptr, self);
    }

    self->field_0014 = 0;
    self->field_0010 = owner;

    MenuProgressInput_UpdateGlobalState(
        reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr);

    void* screen = reinterpret_cast<void*>(self->field_0018);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, screen, 0)[0x16C / 4])(screen, nullptr);

    CMenuPause_CenterOnTarget(reinterpret_cast<CMenuPause*>(self), nullptr);

    CTextDatabase* textDatabase =
        reinterpret_cast<CTextDatabase*>(GetGlobalValue_69B700());
    CTextDatabase_SetTextDatabasePath(
        textDatabase, nullptr, 0x100u,
        const_cast<char*>(CSF_STR(0x00841910, "Texts/CTitles.fli")));

    AudioSoundManager_ForwardToCurrentIfQuerySucceeds(
        reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr);

    uint8_t* globalState = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
    uint32_t capacity = CSF_AT(uint32_t, globalState, 0x13D8);
    uint8_t* sharedString = globalState + 0x13C0;
    char* sharedText;
    if (capacity < 0x10)
        sharedText = reinterpret_cast<char*>(sharedString + 4);
    else
        sharedText = CSF_AT(char*, sharedString, 4);

    GetGlobalValue_521BB0();
    OnlineMenu_SetSharedText(reinterpret_cast<int>(sharedText));

    S_00508360_ReinitializeAndStoreResult(
        reinterpret_cast<S_00508360*>(reinterpret_cast<uint8_t*>(self) + 0x20),
        nullptr,
        reinterpret_cast<int>(CSF_STR(0x008418FC, "menus\\credits.fbs")));

    return self->field_0018;
}

HOOK(0x0052FA80, CMenuCredits_Open, "auto");
