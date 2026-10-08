#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(std::uint8_t, g_GlobalFloatObject, 0x008BABC0);

CSF_FUNC(int, __fastcall, ForwardFloatTriplet, 0x0068F6D0,
         (void* self, void* edx,
          std::uint32_t firstBits,
          std::uint32_t secondBits,
          std::uint32_t thirdBits));

static int __stdcall InvokeGlobalFloatMethod(
    std::uint32_t firstBits,
    std::uint32_t secondBits,
    std::uint32_t thirdBits)
{
    return ForwardFloatTriplet(
        &g_GlobalFloatObject, nullptr, firstBits, secondBits, thirdBits);
}

HOOK(0x0073C6D0, InvokeGlobalFloatMethod, "auto");
