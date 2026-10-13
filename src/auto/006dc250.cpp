#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwD3D9EngineSetRefreshRate, 0x0062B200, (int a1));
CSF_FUNC(unsigned int, __cdecl, RwD3D9EngineSetMultiSamplingLevels, 0x0062B170, (unsigned int a1));
CSF_FUNC(int, __cdecl, RwD3D9EngineSetFullScreenDialogBoxMode, 0x0062B2A0, (int a1));

static unsigned int __fastcall C_00861460_ApplyDisplayMode(C_00861460* self, void* /*edx*/)
{
    if (!self->field_0029 && self->field_0018) {
        RwD3D9EngineSetRefreshRate(0);
    }

    uint32_t multiSamplingLevels = self->field_001c;
    if (multiSamplingLevels) {
        return RwD3D9EngineSetMultiSamplingLevels(multiSamplingLevels);
    }
    return static_cast<unsigned int>(RwD3D9EngineSetFullScreenDialogBoxMode(1));
}

HOOK(0x006DC250, C_00861460_ApplyDisplayMode, "auto");
