// CMenuCredits: when the menu controls object accepts the query at slot +0x74, updates the credits sub-object at
// +0x20 and forwards the call to the controls object's slot +0x78; else returns the query's result (0x0052F8A0).
#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(void, __fastcall, MenuCredits_UpdateScroller, 0x005078B0, (void* self, void* edx));

static hook::Tail __fastcall CMenuCredits_UpdateAndForwardSlot78(void* self, const uint32_t* /*args*/)
{
    void* controls = MenuControls_ResolveObject(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    int accepted = (*reinterpret_cast<int(__fastcall***)(void*, void*, int)>(controls))[0x74 / 4](controls, nullptr, 0);
    if (!static_cast<uint8_t>(accepted))
        return hook::tail_ret(static_cast<uint32_t>(accepted));

    MenuCredits_UpdateScroller(static_cast<uint8_t*>(self) + 0x20, nullptr);
    controls = MenuControls_ResolveObject(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    return hook::tail_jump(controls, (*reinterpret_cast<void***>(controls))[0x78 / 4]);
}

HOOK_TAIL(0x52F8A0, CMenuCredits_UpdateAndForwardSlot78, "auto");
