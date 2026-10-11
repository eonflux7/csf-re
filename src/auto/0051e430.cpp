#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(unsigned int, __fastcall, ParticleManager_ReleaseActiveEntry, 0x0051C070, (void* self, void* edx));

static unsigned int __fastcall C_00840f48_ReleaseIfActive(C_00840f48* self, void* /*edx*/)
{
    if (self->field_0008 != UINT32_MAX)
        return ParticleManager_ReleaseActiveEntry(self, nullptr);
    return 0;
}

HOOK(0x0051E430, C_00840f48_ReleaseIfActive, "auto");
