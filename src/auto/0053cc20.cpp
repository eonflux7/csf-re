// Reimplementation of 0x0053CC20 (Commandos: Strike Force).
// __thiscall hook: this = ecx, two stack args (a3 unused), ret 8.
//
// Behaviour:
//   if ((uint8)sub_7A8ED0(a2)) { vtable-of(self-4)[0x4C/4](self-4); }
//   return sub_428500(a2);

#include <cstdint>

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

// 0x007A8ED0 - one-argument predicate; only the low byte of its result is tested.
CSF_FUNC(int, __cdecl, Sub_7A8ED0, 0x7A8ED0, (int a2));

// 0x00428500 - one-argument handler; its return value becomes this function's result.
CSF_FUNC(int, __cdecl, MenuScenarioSelect_FallbackHandle, 0x428500, (int a2));

// Virtual entry invoked as `call dword ptr [eax+0x4c]` with ecx = self-4.
typedef void (__fastcall* MenuSoundOptions_VirtualActivateFn)(void* self, void* edx);

static int __fastcall CMenuSoundOptions_HandleEvent(void* self, void* /*edx*/, int a2, int a3)
{
  if ((uint8_t)Sub_7A8ED0(a2))
  {
    // The owning object starts 4 bytes before the `this` passed in ecx
    // (secondary sub-object); its vtable pointer lives at self-4.
    uint8_t* owner = (uint8_t*)self - 4;
    void* vtbl = CSF_AT(void*, self, -4);
    MenuSoundOptions_VirtualActivateFn activate =
        reinterpret_cast<MenuSoundOptions_VirtualActivateFn>(CSF_AT(void*, vtbl, 0x4C));
    activate(owner, nullptr);
  }
  return MenuScenarioSelect_FallbackHandle(a2);
}

HOOK(0x53CC20, CMenuSoundOptions_HandleEvent, "auto");
