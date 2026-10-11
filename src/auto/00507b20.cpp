#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_ColorBlack,   0x008A7DA0);
CSF_GLOBAL(float, g_ColorBlack_G, 0x008A7DA4);
CSF_GLOBAL(float, g_ColorBlack_B, 0x008A7DA8);
CSF_GLOBAL(float, g_ColorBlack_A, 0x008A7DAC);

static float* __fastcall S_005080e0_FindColorByName(
    S_005080e0* self, void* /*edx*/, float* output, const char* name)
{
    float red = g_ColorBlack;
    float green = g_ColorBlack_G;
    float blue = g_ColorBlack_B;
    float alpha = g_ColorBlack_A;

    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x3C);
    std::uint32_t entryOffset = 0;

    for (std::uint32_t index = 0; index < count; ++index, entryOffset += 44)
    {
        auto* entry = CSF_AT(std::uint8_t*, self, 0x34) + entryOffset;
        const char* entryName =
            CSF_AT(std::uint32_t, entry, 0x18) < 0x10
                ? reinterpret_cast<const char*>(entry + 4)
                : CSF_AT(const char*, entry, 0x04);

        const char* requestedCharacter = name;
        const char* entryCharacter = entryName;
        bool matches = false;

        for (;;)
        {
            const char character = *requestedCharacter;
            if (character != *entryCharacter)
                break;
            if (character == '\0')
            {
                matches = true;
                break;
            }
            ++requestedCharacter;
            ++entryCharacter;
        }

        if (matches)
        {
            red = CSF_AT(float, entry, 0x1C);
            green = CSF_AT(float, entry, 0x20);
            blue = CSF_AT(float, entry, 0x24);
            alpha = CSF_AT(float, entry, 0x28);
            break;
        }
    }

    output[0] = red;
    output[1] = green;
    output[2] = blue;
    output[3] = alpha;
    return output;
}

HOOK(0x00507B20, S_005080e0_FindColorByName, "auto");
