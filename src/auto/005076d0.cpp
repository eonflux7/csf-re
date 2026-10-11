#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetWideTextLength, 0x00695560, (std::uint16_t* text));

// Copies a NUL-terminated wide string into two buffers, splitting at the first '@'.
// Characters before the '@' go to firstOut, the rest go to secondOut. Each buffer is
// NUL-terminated. Returns true if a '@' was found.
static bool __stdcall SplitWideTextAtAtSign(int textAddress, std::uint16_t* firstOut, std::uint16_t* secondOut)
{
    auto* text = reinterpret_cast<std::uint16_t*>(textAddress);
    bool foundAtSign = false;

    std::uint32_t textLength = static_cast<std::uint32_t>(GetWideTextLength(text));
    std::uint32_t index = 0;
    std::uint16_t* writePtr = firstOut;

    for (index = 0; index < textLength; ++index)
    {
        if (!foundAtSign && text[index] == 0x40)
        {
            *writePtr = 0;
            writePtr = secondOut;
            foundAtSign = true;
        }
        else
        {
            *writePtr++ = text[index];
        }
    }

    *writePtr = 0;
    return foundAtSign;
}

HOOK(0x5076D0, SplitWideTextAtAtSign, "auto");
