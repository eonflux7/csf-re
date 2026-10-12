#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int*, __fastcall, GfxParticleEmitters_InitializeSettings, 0x006DE210, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RpClumpClone, 0x0058FC60, (int a1));

// Returns the particle settings block owned by this atomic. The first call
// allocates a 0x154-byte copy of the shared source settings (and clones the
// attached clump, if any); later calls return the existing copy.
static int* __fastcall RpPrtStd_GetOwnedSettingsCopy(uint8_t* self, void* /*edx*/)
{
    int* sourceSettings = CSF_AT(int*, self, 0x10);

    if (CSF_AT(uint8_t, self, 0x14) != 0)
    {
        CSF_AT(uint8_t, self, 0x15) = 1;
        return sourceSettings;
    }

    void* block = Mem_Alloc(0x154);
    int* settingsCopy = nullptr;
    if (block)
        settingsCopy = GfxParticleEmitters_InitializeSettings(block, nullptr);

    std::memcpy(settingsCopy, sourceSettings, 0x154);

    int sourceClump = sourceSettings[1];
    if (sourceClump)
        settingsCopy[1] = RpClumpClone(sourceClump);

    CSF_AT(uint8_t, self, 0x14) = 1;
    CSF_AT(uint8_t, self, 0x15) = 1;
    CSF_AT(int*, self, 0x10) = settingsCopy;
    return settingsCopy;
}

HOOK(0x006F39A0, RpPrtStd_GetOwnedSettingsCopy, "auto");
