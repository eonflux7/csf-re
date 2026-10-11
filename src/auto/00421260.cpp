#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Original: 0x004DB1D0, __fastcall-style callee taking one stack argument.
CSF_FUNC(void, __fastcall, sub_4DB1D0, 0x004DB1D0, (void* self, void* edx, int a2));

// TFncGirar::<method>(int a2, int a3), __thiscall, ret 8.
// Forwards a single 0 flag to the base-task routine; the two incoming
// arguments are not used by the original.
static void __fastcall TFncGirar_ApplyDefaultSetup(TFncGirar* self, void* /*edx*/, int /*a2*/, int /*a3*/) {
    sub_4DB1D0(self, nullptr, 0);
}

HOOK(0x421260, TFncGirar_ApplyDefaultSetup, "auto");
