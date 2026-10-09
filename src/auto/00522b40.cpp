#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(void, __fastcall, CSoundManager_ApplyMasterVolumes, 0x005228C0, (void* self, void* edx, float a2, float a3));

// Stores the low byte of the argument as the volume mode flag. When it changes:
// on -> apply the two configured volumes read from the global config block;
// off -> apply 1.0 / 1.0.
static void __fastcall CSoundManager_SetVolumeModeFlag(CSoundManager* self, void* /*edx*/, uint32_t flagArg)
{
    uint8_t flag = static_cast<uint8_t>(flagArg);

    if (self->field_0078 == flag)
        return;

    self->field_0078 = flag;

    if (flag)
    {
        uint8_t* configA = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));
        float volumeA = CSF_AT(float, configA, 0x18A0);

        uint8_t* configB = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));
        float volumeB = CSF_AT(float, configB, 0x18A4);

        CSoundManager_ApplyMasterVolumes(self, nullptr, volumeA, volumeB);
    }
    else
    {
        CSoundManager_ApplyMasterVolumes(self, nullptr, 1.0f, 1.0f);
    }
}

HOOK(0x00522B40, CSoundManager_SetVolumeModeFlag, "auto");
