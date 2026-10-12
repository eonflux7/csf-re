#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpAtomicSetPipeline, 0x0058CC90, (int a1, int a2));
CSF_FUNC(void, __fastcall, sub_6D2110, 0x006D2110, (void* self, void* edx, int a2));

// __thiscall void (this, int atomic): if atomic is non-null, clears its render
// pipeline and then forwards the atomic to this object's sub-handler.
static void __fastcall C_008631c8_SetAtomicPipelineAndRefresh(C_008631c8* self, void* /*edx*/, int atomic)
{
    if (atomic)
    {
        RpAtomicSetPipeline(atomic, 0);
        sub_6D2110(self, nullptr, atomic);
    }
}

HOOK(0x00705F90, C_008631c8_SetAtomicPipelineAndRefresh, "auto");
