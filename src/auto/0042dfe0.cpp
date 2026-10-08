// Commandos: Strike Force — HUD module.
// Refreshes the global menu object referenced at +0x2D0 of the HUD object.
// If that slot is non-null, the original tail-jumps to 0x4386D0 with the slot value
// as `this`, so the callee's result is returned unchanged. Otherwise it returns
// without touching the return register.

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x4386D0 : sub_4386D0, void *__thiscall(std::uint32_t *this). Takes no stack args, ret 0.
CSF_FUNC(void*, __fastcall, MenuObject_Refresh, 0x4386D0, (void* self, void* edx));

static int __fastcall Hud_RefreshGlobalMenuObject(void* self, void* /*edx*/)
{
    void* pGlobalMenuObject = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x2D0));
    if (pGlobalMenuObject == nullptr)
        return 0;

    void* refreshResult = MenuObject_Refresh(pGlobalMenuObject, nullptr);
    return static_cast<int>(reinterpret_cast<uintptr_t>(refreshResult));
}

HOOK(0x42DFE0, Hud_RefreshGlobalMenuObject, "auto");
