#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpWorldRemoveLight, 0x0058DFA0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpLightGetWorld, 0x0058DFD0, (int a1));
CSF_FUNC(int, __cdecl, RpClumpGetWorld, 0x0058F0E0, (int a1));
CSF_FUNC(int, __cdecl, RpClumpRemoveLight, 0x0058F1D0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpLightDestroy, 0x0059D000, (int a1));
CSF_FUNC(int, __cdecl, RwFrameRemoveChild, 0x00623750, (int a1));
CSF_FUNC(int, __cdecl, RwFrameDestroy, 0x00623B10, (int frame));
CSF_FUNC(int, __cdecl, _rwObjectHasFrameSetFrame, 0x0063CE30, (int object, int frame));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static int __fastcall CLightObject_ReleaseLightResources(
    CLightObject* self, void* /*edx*/)
{
    if (CSF_AT(void*, self, 0x38))
    {
        CoreMemory_Free(CSF_AT(void*, self, 0x38));
        CSF_AT(void*, self, 0x38) = nullptr;
    }

    int result = CSF_AT(int, self, 0x0C);
    if (result)
    {
        const int world = RpLightGetWorld(result);
        if (world)
            RpWorldRemoveLight(world, CSF_AT(int, self, 0x0C));

        int frame = CSF_AT(
            int, reinterpret_cast<void*>(CSF_AT(int, self, 0x0C)), 0x04);
        if (frame && CSF_AT(int, reinterpret_cast<void*>(frame), 0x04))
            RwFrameRemoveChild(frame);

        const int clump = RpClumpGetWorld(CSF_AT(int, self, 0x0C));
        if (clump)
        {
            const int clumpFrame =
                CSF_AT(int, reinterpret_cast<void*>(clump), 0x04);
            if (clumpFrame == frame && frame)
            {
                _rwObjectHasFrameSetFrame(CSF_AT(int, self, 0x0C), 0);
                frame = 0;
                CSF_AT(int, reinterpret_cast<void*>(clump), 0x04) = 0;
                RwFrameDestroy(clumpFrame);
            }
            RpClumpRemoveLight(clump, CSF_AT(int, self, 0x0C));
        }

        if (frame)
        {
            _rwObjectHasFrameSetFrame(CSF_AT(int, self, 0x0C), 0);
            RwFrameDestroy(frame);
        }

        result = RpLightDestroy(CSF_AT(int, self, 0x0C));
        CSF_AT(int, self, 0x0C) = 0;
    }
    return result;
}

HOOK(0x006FCD40, CLightObject_ReleaseLightResources, "auto");
