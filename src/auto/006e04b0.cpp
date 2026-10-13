#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwards to ProcessEmitterStepWithFallback on the sub-object at +0x20.
CSF_FUNC(int, __fastcall, ProcessEmitterStepWithFallback, 0x00703470,
         (void* self, void* edx, int a2, unsigned int a3));

static int __fastcall GfxPrtStd_GetStateResource(S_007030d0* self, void* /*edx*/,
                                                 S_007030d0* a1, int a2)
{
    void* subObject = reinterpret_cast<uint8_t*>(self) + 0x20;
    return ProcessEmitterStepWithFallback(subObject, nullptr,
                                          reinterpret_cast<int>(a1),
                                          static_cast<unsigned int>(a2));
}

HOOK(0x006E04B0, GfxPrtStd_GetStateResource, "auto");
