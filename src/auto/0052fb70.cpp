#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00521BB0 - int __cdecl GetGlobalValue_521BB0()
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x521BB0, ());

// 0x005231D0 - void __thiscall sub_5231D0(int this, int value); ret 4
CSF_FUNC(void, __fastcall, GlobalSetValue_5231D0, 0x5231D0, (int self, void* /*edx*/, int value));

// Stores the given value into the menu's selected-scene field and forwards it
// to the global menu/session object obtained from GetGlobalValue_521BB0().
static void __fastcall CMenuSceneSelect_SetSelectedScene(CMenuSceneSelect* self, void* /*edx*/, int value)
{
    self->field_004c = static_cast<uint32_t>(value);

    int globalValue = GetGlobalValue_521BB0();
    GlobalSetValue_5231D0(globalValue, nullptr, value);
}

HOOK(0x52FB70, CMenuSceneSelect_SetSelectedScene, "auto");
