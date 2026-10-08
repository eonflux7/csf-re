// Commandos: Strike Force (Pyro Studios, 2006)
// Module: menu_scenario_select
// Target: 0x0052F840  int __thiscall sub_52F840(std::uint32_t* this)   (ret 0)
#include <cstdint>
#include "hook.h"
#include "globals.h"

// 0x00508410  unsigned int __thiscall sub_508410(std::uint32_t* this)   (ret 0) - cleanup of the embedded widget
CSF_FUNC(unsigned int, __fastcall, MenuScenarioSelect_Sub508410, 0x508410, (void* self, void* edx));
// 0x0070DDE0  int __cdecl sub_70DDE0()                          (ret 0) - global accessor
CSF_FUNC(int, __cdecl, MenuScenarioSelect_GetGlobal70DDE0, 0x70DDE0, ());
// 0x00521BB0  int __cdecl sub_521BB0()                          (ret 0) - global accessor
CSF_FUNC(int, __cdecl, MenuScenarioSelect_GetGlobal521BB0, 0x521BB0, ());
// 0x005225D0  int __thiscall sub_5225D0(std::uint32_t* this)           (ret 0) - tail-jump target
CSF_FUNC(int, __fastcall, MenuScenarioSelect_Sub5225D0, 0x5225D0, (void* self, void* edx));

// Hides/tears down the scenario-select menu:
//  1) runs the embedded sub-object at this+0x1C's cleanup,
//  2) asks the first global for its current child (vtbl+0x70) and, if present,
//     invokes the child's vtbl+0x14,
//  3) tail-calls sub_5225D0 on the second global.
static int __fastcall CMenuScenarioSelect_Hide(void* self, void* /*edx*/)
{
    MenuScenarioSelect_Sub508410(reinterpret_cast<uint8_t*>(self) + 0x1c, nullptr);

    void* pGlobal = reinterpret_cast<void*>(MenuScenarioSelect_GetGlobal70DDE0());
    void* pChild = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, pGlobal, 0)[0x70 / 4])(pGlobal, nullptr);
    if (pChild)
        ((void (__fastcall*)(void*, void*))CSF_AT(void**, pChild, 0)[0x14 / 4])(pChild, nullptr);

    void* pGlobal2 = reinterpret_cast<void*>(MenuScenarioSelect_GetGlobal521BB0());
    return MenuScenarioSelect_Sub5225D0(pGlobal2, nullptr);
}

HOOK(0x52F840, CMenuScenarioSelect_Hide, "auto");
