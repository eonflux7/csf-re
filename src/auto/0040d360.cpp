#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082deb8_ReleaseChildBase_41E110, 0x0041E110, (C_0082e520* self, void*));
CSF_FUNC(int, __fastcall, FinalizeObject_40F760, 0x0040F760, (void* self, void* edx));

// Resets the object's vtable to C_0082dc88, releases the child base, then
// tail-forwards to the finalizer at 0x40F760 with the same `this`.
static int __fastcall C_0082dc88_ReleaseChildAndFinalize(C_0082dc88* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x0082DC88);
    C_0082deb8_ReleaseChildBase_41E110(self, nullptr);
    return FinalizeObject_40F760(self, nullptr);
}

HOOK(0x0040D360, C_0082dc88_ReleaseChildAndFinalize, "auto");
