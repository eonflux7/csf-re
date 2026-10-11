#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(uint32_t, g_SecurityCookie, 0x008AA0E8);

// gen_codes: edx = the bit-length counts, edi = the tree, one stack argument (max code), the caller pops.
static uint32_t __cdecl CoreZlibLog_GenerateCodes(hook::Regs* regs, const uint32_t* args)
{
    const auto* counts = reinterpret_cast<const uint16_t*>(regs->edx);
    void* symbols = reinterpret_cast<void*>(regs->edi);
    const int maxSymbol = static_cast<int>(args[0]);
    // The original guards its code table with the stack cookie (compiler-generated check).
    const uint32_t savedCookie = g_SecurityCookie;
    (void)savedCookie;
    uint16_t nextCode[16] = {};
    uint16_t runningCode = 0;

    for (int bits = 1; bits <= 15; ++bits)
    {
        runningCode = static_cast<uint16_t>(
            2 * (runningCode + counts[bits - 1]));
        nextCode[bits] = runningCode;
    }

    for (int symbol = 0; symbol <= maxSymbol; ++symbol)
    {
        int bitsLeft = CSF_AT(uint16_t, symbols, 4 * symbol + 2);
        if (bitsLeft != 0)
        {
            uint32_t code = nextCode[bitsLeft];
            nextCode[bitsLeft] =
                static_cast<uint16_t>(nextCode[bitsLeft] + 1);

            uint32_t reversed = 0;
            do
            {
                const uint32_t bit = code & 1;
                code >>= 1;
                reversed = 2 * (bit | reversed);
                --bitsLeft;
            } while (bitsLeft > 0);

            CSF_AT(uint16_t, symbols, 4 * symbol) =
                static_cast<uint16_t>(reversed >> 1);
        }
    }

    return static_cast<uint32_t>(maxSymbol);
}

HOOK_REGS(0x006A4610, CoreZlibLog_GenerateCodes, 0, "auto");
