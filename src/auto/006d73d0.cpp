#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxSkin_SetKeyedValue, 0x006C4BF0, (void* self, void* /*edx*/, int key, unsigned int value));

// Stores the mode flag at +0xA0, then sets keyed values 10 (5 or 1 depending on flag) and 11 (2)
// on the skin object embedded at +0x11C.
static int __fastcall C_008611d4_SetModeAndApplySkinKeys(C_008611d4* self, void* /*edx*/, char a2)
{
    CSF_AT(uint8_t, self, 0xA0) = static_cast<uint8_t>(a2);

    void* skinObject = reinterpret_cast<uint8_t*>(self) + 0x11C;

    int keyTenValue = 5;
    if (!a2)
        keyTenValue = 1;

    GfxSkin_SetKeyedValue(skinObject, nullptr, 10, static_cast<unsigned int>(keyTenValue));
    return GfxSkin_SetKeyedValue(skinObject, nullptr, 11, 2u);
}

HOOK(0x006D73D0, C_008611d4_SetModeAndApplySkinKeys, "auto");
