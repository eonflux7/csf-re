#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __cdecl, SetOptionsByteAt40, 0x00697330, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte40, 0x00697350, ());
CSF_FUNC(int, __cdecl, GfxIm3d_LoadTextureWithOptions, 0x006E2EB0, (char* Source, char a2, int a3));
CSF_FUNC(int, __cdecl, GfxIm3d_TryLoadTexture, 0x006E2ED0, (char* Source, int a2));
CSF_FUNC(bool, __cdecl, GfxIm3d_IsSpecialResourceName, 0x006E2F80, (char* source));

static int __cdecl GfxAnimInterpolator_LoadTextureWithFallbackOptions(char* source)
{
    alignas(8) uint8_t statusBytes[0x1] = {0};

    if (!source || !*source)
        return 0;

    const char loadOptions = !GfxIm3d_IsSpecialResourceName(source);
    const char savedOption = GetOptionsByte40();
    SetOptionsByteAt40(0);

    int texture = GfxIm3d_TryLoadTexture(
        source, reinterpret_cast<int>(statusBytes));

    if (!texture)
    {
        texture = GfxIm3d_LoadTextureWithOptions(
            source, loadOptions, reinterpret_cast<int>(statusBytes));

        if (!texture && savedOption)
        {
            SetOptionsByteAt40(1);
            texture = GfxIm3d_TryLoadTexture(
                source, reinterpret_cast<int>(statusBytes));

            if (!texture)
            {
                texture = GfxIm3d_LoadTextureWithOptions(
                    source, loadOptions, reinterpret_cast<int>(statusBytes));
            }
        }
    }

    SetOptionsByteAt40(savedOption);
    return texture;
}

HOOK(0x00707D60, GfxAnimInterpolator_LoadTextureWithFallbackOptions, "auto");
