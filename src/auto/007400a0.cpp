#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(char, __fastcall, C_74B200_SetFlagAndReturnTrue, 0x0074B200, (uint8_t* self, void*));

// Sets or clears the sign flag (bit 0) at +0x6F from the sign of `value`.
// When value is positive, also stores its truncated integer value as a byte at +0x72
// and refreshes the sub-object at +0x72 and the flag at +0x6F via C_74B200.
static char __fastcall NetCogibleDDL_SetSignFlagAndStoreValue(void* self, void* /*edx*/, float value)
{
    if (value <= g_fZero) {
        uint8_t flags = CSF_AT(uint8_t, self, 0x6F);
        CSF_AT(uint8_t, self, 0x6F) = static_cast<uint8_t>(flags & 0xFE);
        return C_74B200_SetFlagAndReturnTrue(reinterpret_cast<uint8_t*>(self) + 0x6F, nullptr);
    }

    uint8_t flags = CSF_AT(uint8_t, self, 0x6F);
    CSF_AT(uint8_t, self, 0x6F) = static_cast<uint8_t>(flags | 1);
    CSF_AT(uint8_t, self, 0x72) = static_cast<uint8_t>(static_cast<int64_t>(value));
    C_74B200_SetFlagAndReturnTrue(reinterpret_cast<uint8_t*>(self) + 0x72, nullptr);
    return C_74B200_SetFlagAndReturnTrue(reinterpret_cast<uint8_t*>(self) + 0x6F, nullptr);
}

HOOK(0x7400A0, NetCogibleDDL_SetSignFlagAndStoreValue, "auto");
