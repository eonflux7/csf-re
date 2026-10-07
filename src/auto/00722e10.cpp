#include <cstdint>
#include "globals.h"
#include "hook.h"

// Copies four consecutive dwords from the source block into the sub-object
// embedded at this + 0x1F8 (dst[0..3] = src[0..3]) and returns the last
// copied dword as a 32-bit int.
static int __fastcall SetFourDwordsAt1F8(void* self, void* /*edx*/, const uint32_t* src)
{
    uint32_t* dst = reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(self) + 0x1F8);

    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    int result = static_cast<int>(src[3]);
    dst[3] = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x00722E10, SetFourDwordsAt1F8, "auto");
