#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x0043AC60: __thiscall, one stack argument (ret 4).
CSF_FUNC(void, __fastcall, Hud_ApplyEnabledWidgetValue, 0x0043AC60, (void* self, void* edx, float a3));

// Reimplementation of 0x0043B4E0 (__thiscall, one stack argument, ret 4).
// Calls the object's virtual slot at +0x20 (vtable index 8) with no arguments,
// then forwards the float argument to Hud_ApplyEnabledWidgetValue.
static void __fastcall Hud_UpdateEnabledWidget(float* self, void* /*edx*/, float a2)
{
    // (*(void (__thiscall **)(float *))(*(std::uint32_t *)this + 32))(this);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, self, 0)[8])(self, nullptr);

    // sub_43AC60(this, a2);
    Hud_ApplyEnabledWidgetValue(self, nullptr, a2);
}

HOOK(0x0043B4E0, Hud_UpdateEnabledWidget, "auto");
