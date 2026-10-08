#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall (this = ecx, one stack arg), ret 4. The second argument is the
// std::_String_base pointer; it is passed through untouched, so it is typed as void*.
CSF_FUNC(int, __fastcall, Hud_ProcessWidgetEntry, 0x00433EA0, (void* self, void* edx, void* a2));

// Forwards a2 to sub_433EA0 (with a1 as the this pointer) when a1 is non-null;
// otherwise nothing is done. The original returned an uninitialized eax in that
// case; this returns 0 instead.
static int __stdcall Hud_ForwardToWidgetEntryIfNonNull(int a1, void* a2)
{
    if (a1)
        return Hud_ProcessWidgetEntry(reinterpret_cast<void*>(static_cast<intptr_t>(a1)), nullptr, a2);
    return 0;
}

HOOK(0x00433EF0, Hud_ForwardToWidgetEntryIfNonNull, "auto");
