#include <cstddef>
#include <cstdint>

#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

// Original callees, bound by address.
CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, GlobalMenu_BeginSelectionUpdate, 0x00521E80, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_FUNC(void, __fastcall, CMenuIntroVideo_RefreshState, 0x00535900, (CMenuIntroVideo* self, void* edx));
CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x00506AC0, ());

// Initializes the intro video screen: stops any active playback through the
// global state object, resets playback fields, stores the owner pointer and
// returns the video player object stored in field_0018.
static int __fastcall CMenuIntroVideo_Initialize(CMenuIntroVideo* self, void* /*edx*/,
                                                 int ownerMenu, int /*unusedArg3*/)
{
    // Stop previous playback if the attached object is present and flagged.
    if (self->field_0024 != 0 &&
        CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0024), 0x12) != 0)
    {
        void* globalState = GetGlobalState_50D190();
        ((void(__fastcall*)(void*, void*, CMenuIntroVideo*))CSF_AT(void**, globalState, 0)[6])(
            globalState, nullptr, self);
    }

    void* valueA = GetGlobalValue_521BB0();
    GlobalMenu_BeginSelectionUpdate(valueA, nullptr);

    self->field_0020 = 0;

    void* globalStateForFlag = GetGlobalState_50D190();
    uint8_t stateFlag = ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, globalStateForFlag, 0)[18])(
        globalStateForFlag, nullptr);

    // The original loads the player object (field_0018) before the field writes below.
    void* playerObjA = reinterpret_cast<void*>(self->field_0018);

    self->field_0028 = stateFlag;
    self->field_0029 = 0;
    self->field_002a = 0;
    self->field_0014 = 0;
    self->field_002c = 0.0f;
    self->field_0010 = static_cast<uint32_t>(ownerMenu);

    // Player object: vtable slot 59 (offset 0xEC) with one stack argument of 0.
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, playerObjA, 0)[59])(playerObjA, nullptr, 0);

    // Player object reloaded from field_0018: vtable slot 91 (offset 0x16C), no arguments.
    void* playerObjB = reinterpret_cast<void*>(self->field_0018);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, playerObjB, 0)[91])(playerObjB, nullptr);

    // Owner object reloaded from field_0010: vtable slot 39 (offset 0x9C) with one stack argument of 0.
    void* ownerObj = reinterpret_cast<void*>(self->field_0010);
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, ownerObj, 0)[39])(ownerObj, nullptr, 0);

    void* valueB = GetGlobalValue_70DDE0();
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, valueB, 0)[35])(valueB, nullptr);

    CMenuIntroVideo_RefreshState(self, nullptr);
    Nullsub_506AC0();

    return static_cast<int>(self->field_0018);
}

HOOK(0x00535AF0, CMenuIntroVideo_Initialize, "auto");
