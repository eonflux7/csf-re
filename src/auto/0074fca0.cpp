#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Bound by address: __thiscall callee (self = arg4) with 4 stack args (a2, a3, a4, a5), ret 16.
CSF_FUNC(void*, __fastcall, GameConfig_WriteSoundAndSubtitleFields, 0x007539E0, (C_008622e8* self, void* /*edx*/, C_008622e8* a2, int a3, int a4, int a5));

// Original: stdcall, 7 stack args, ret 28. Forwards arg4 as `this`, then a2, a3, a4, a5 as stack args.
// a1 and _10 are not used by the original.
static void* __stdcall NetPostmanDDL_ForwardToWriteSoundAndSubtitle(int a1, C_008622e8* arg4, int unused10, int a3, int a4, C_008622e8* a2, int a5)
{
    return GameConfig_WriteSoundAndSubtitleFields(arg4, nullptr, a2, a3, a4, a5);
}

HOOK(0x0074FCA0, NetPostmanDDL_ForwardToWriteSoundAndSubtitle, "auto");
