#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, RwTextureSetName, 0x00631800, (void* texture, char* name));
CSF_FUNC(bool, __cdecl, IsParticleTextureNameSpecial, 0x006E2F80, (char* Source));

// Registers a particle texture: sets its name, then resets the 8-dword
// state block at +0x30..+0x4F and stores the flag bytes and the 0x11 marker at +0x51.
static void __cdecl GfxParticleRender_RegisterTexture(int a1, char* Source, char a3, char a4)
{
    if (a1 == 0) {
        return;
    }

    uint8_t* texture = reinterpret_cast<uint8_t*>(a1);

    RwTextureSetName(texture, Source);

    char flagValue;
    if (IsParticleTextureNameSpecial(Source)) {
        flagValue = 0;
    } else {
        flagValue = a3;
    }

    CSF_AT(uint32_t, texture, 0x30) = 0;
    CSF_AT(uint32_t, texture, 0x34) = 0;
    CSF_AT(uint32_t, texture, 0x38) = 0;
    CSF_AT(uint32_t, texture, 0x3C) = 0;
    CSF_AT(uint32_t, texture, 0x40) = 0;
    CSF_AT(uint32_t, texture, 0x44) = 0;
    CSF_AT(uint32_t, texture, 0x48) = 0;
    CSF_AT(uint32_t, texture, 0x4C) = 0;

    CSF_AT(char, texture, 0x30) = flagValue;
    CSF_AT(char, texture, 0x31) = a4;
    CSF_AT(uint8_t, texture, 0x51) = 0x11;
}

HOOK(0x006E3020, GfxParticleRender_RegisterTexture, "auto");
