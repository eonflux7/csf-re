#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, Shared_HandleFloatArg, 0x004E5280, (void* self, void* edx, float a2));

// Forwards the caller's float argument unchanged to the shared handler at 0x4E5280.
static void __fastcall C_0083ec48_ForwardFloatArgToSharedHandler(C_0083ec48* self, void* /*edx*/, float a2)
{
    Shared_HandleFloatArg(self, nullptr, a2);
}

HOOK(0x004E8BC0, C_0083ec48_ForwardFloatArgToSharedHandler, "auto");
