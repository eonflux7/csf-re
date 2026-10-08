#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(char*, g_FontKeyword, 0x008A85E8);

CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0,
         (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0,
         (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));

static int __fastcall CGuiBuilder_GetFont(
    CGuiBuilder* self, void* /*edx*/, std::uint8_t* properties)
{
    if (!FindProperty(properties, nullptr, g_FontKeyword))
        return 0;

    char buffer[256]{};
    MenuExtras_ReadText(properties, nullptr, g_FontKeyword, buffer, 256);

    void* manager = reinterpret_cast<void*>(self->field_0004);
    return reinterpret_cast<int(__fastcall*)(void*, void*, char*)>(
        CSF_AT(void**, manager, 0)[0x4C / 4])(manager, nullptr, buffer);
}

HOOK(0x0071DB60, CGuiBuilder_GetFont, "auto");
