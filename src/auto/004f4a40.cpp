#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, SetSubtitlePanelSize, 0x004F1D70,
         (void* self, void* edx, float width, float height));
CSF_FUNC(int, __fastcall, ResizeSubtitleContent, 0x004F4180,
         (void* self, void* edx, int a2, int a3));

static int __fastcall HudStringShow_SetSize(
    void* self, void* /*edx*/, int widthBits, int heightBits)
{
    float width;
    float height;
    std::memcpy(&width, &widthBits, sizeof(width));
    std::memcpy(&height, &heightBits, sizeof(height));

    SetSubtitlePanelSize(CSF_AT(void*, self, 0x4), nullptr, width, height);
    SetSubtitlePanelSize(CSF_AT(void*, self, 0x8), nullptr, width, height);
    return ResizeSubtitleContent(
        CSF_AT(void*, self, 0xC), nullptr, widthBits, heightBits);
}

HOOK(0x004F4A40, HudStringShow_SetSize, "auto");
