#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(char*, g_TextureQuadKeyword, 0x008A8668);

CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0,
         (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, CGuiBuilder_CreateTextureQuad, 0x0071E4E0,
         (CGuiBuilder* self, void* edx, std::uint8_t* properties));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0,
         (void* self, void* edx));

static void __fastcall CGuiBuilder_ApplyTextureQuad(
    CGuiBuilder* self, void* /*edx*/, void* control, std::uint8_t* properties)
{
    if (!properties || !self->field_0008)
        return;

    if (!FindProperty(properties, nullptr, g_TextureQuadKeyword))
        return;

    std::uint8_t* quadProperties =
        FindProperty(properties, nullptr, g_TextureQuadKeyword);
    int quad = CGuiBuilder_CreateTextureQuad(self, nullptr, quadProperties);

    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, control, 0)[0x248 / 4])(control, nullptr, quad);

    if (quad)
    {
        ReleaseObjectReference(
            reinterpret_cast<std::uint8_t*>(quad) + 4, nullptr);
    }
}

HOOK(0x0071F680, CGuiBuilder_ApplyTextureQuad, "auto");
