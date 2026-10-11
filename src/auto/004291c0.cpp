#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, Cheats_SetKeyBinding, 0x004287D0,
         (int a1, int a2, int a3, int a4));
CSF_FUNC(const char*, __cdecl, GetTrucoString, 0x00428B10, ());

using VkKeyScanAFunction = int16_t (__stdcall*)(char);
using MapVirtualKeyAFunction = uint32_t (__stdcall*)(uint32_t, uint32_t);

CSF_GLOBAL(VkKeyScanAFunction, g_VkKeyScanA, 0x0082D294);
CSF_GLOBAL(MapVirtualKeyAFunction, g_MapVirtualKeyA, 0x0082D270);

static uint32_t __fastcall C_008300c0_ConfigureCheatKeyBindings(
    C_008300c0* self, void* /*edx*/, int bindingContext)
{
    self->field_0e98 = 1;

    for (uint32_t binding = 68; binding <= 0x57; ++binding)
        Cheats_SetKeyBinding(bindingContext, static_cast<int>(binding), 0, -1);

    const char* cheatString = GetTrucoString();
    const uint32_t length = static_cast<uint32_t>(std::strlen(cheatString));
    uint32_t result = length;

    if (length != 0)
    {
        const VkKeyScanAFunction scanKey = g_VkKeyScanA;
        uint32_t binding = 68;

        do
        {
            const uint32_t virtualKey =
                static_cast<uint8_t>(scanKey(cheatString[binding - 68]));
            result = g_MapVirtualKeyA(virtualKey, 0);
            if (result == 0)
            {
                self->field_0e98 = 0;
                return result;
            }

            Cheats_SetKeyBinding(bindingContext, static_cast<int>(binding),
                                 0, static_cast<int>(result));
            ++binding;
            result = binding - 68;
        } while (result < length);
    }

    return result;
}

HOOK(0x004291C0, C_008300c0_ConfigureCheatKeyBindings, "auto");
