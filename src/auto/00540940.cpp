#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(uint8_t, __fastcall, IsMpStatsDisplayActive, 0x0042DFA0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState, 0x0050D190, ());
CSF_FUNC(int, __fastcall, CMenuMpScoreBar_ApplyChildDisplayMode, 0x005408F0, (CMenuMpScoreBar* self, void* edx));

// Calls virtual slot 0xA8 (vtable index 42) of a child object with one stack argument.
// The slot is thiscall-style (ecx = child, one pushed argument, callee pops it).
// Return value is only meaningful for the last call in the original's second path.
static int SetScoreBarChildEnabled(uint32_t childAddress, int enabledValue)
{
    void* child = reinterpret_cast<void*>(childAddress);
    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[42])(child, nullptr, enabledValue);
}

static int __fastcall CMenuMpScoreBar_RefreshChildVisibility(CMenuMpScoreBar* self, void* /*edx*/)
{
    if (self->field_0014 == 0)
        return 0;
    if (self->field_0018 == 0)
        return 0;
    if (self->field_001c == 0)
        return 0;
    if (self->field_0020 == 0)
        return 0;
    if (self->field_0024 == 0)
        return 0;

    void* gameContext = reinterpret_cast<void*>(GetGlobal8afc78());
    const uint8_t isActive = IsMpStatsDisplayActive(gameContext, nullptr);
    self->field_0038 = isActive;

    if (isActive)
    {
        void* globalState = reinterpret_cast<void*>(GetGlobalState());
        const int childEnabledArg = (CSF_AT(uint32_t, globalState, 0xCC) == 3) ? 0 : 1;
        SetScoreBarChildEnabled(self->field_0018, childEnabledArg);
        SetScoreBarChildEnabled(self->field_001c, childEnabledArg);
        SetScoreBarChildEnabled(self->field_0020, childEnabledArg);
        SetScoreBarChildEnabled(self->field_0024, childEnabledArg);
        return CMenuMpScoreBar_ApplyChildDisplayMode(self, nullptr);
    }

    CMenuMpScoreBar_ApplyChildDisplayMode(self, nullptr);
    SetScoreBarChildEnabled(self->field_0018, 1);
    SetScoreBarChildEnabled(self->field_001c, 1);
    SetScoreBarChildEnabled(self->field_0020, 1);
    return SetScoreBarChildEnabled(self->field_0024, 1);
}

HOOK(0x00540940, CMenuMpScoreBar_RefreshChildVisibility, "auto");
