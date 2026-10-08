#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00563AB0 — __thiscall helper in the same module (menu_mp_sessions).
// It takes no stack arguments (ret 0) and leaves a pointer in eax; the caller
// below forwards `this` unchanged and returns whatever it produces.
CSF_FUNC(void*, __fastcall, CMenuMpSessions_BuildSessionEntry, 0x563AB0, (CMenuMpSessions* self, void* edx));

// 0x00563FC0 — thunk: ignores its single stack argument (a2), forwards `this`
// untouched to 0x00563AB0 and returns that result. `ret 4` pops the ignored
// argument, so the free function keeps exactly one stack parameter.
static void* __fastcall CMenuMpSessions_GetSessionEntry(CMenuMpSessions* self, void* /*edx*/, int /*unusedArg*/)
{
    return CMenuMpSessions_BuildSessionEntry(self, nullptr);
}

HOOK(0x563FC0, CMenuMpSessions_GetSessionEntry, "auto");
