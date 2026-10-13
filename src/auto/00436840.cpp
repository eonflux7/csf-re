#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));

CSF_GLOBAL(const char*, g_pszKey_TOP_CGuiControl, 0x00899634);
CSF_GLOBAL(const char*, g_pszKey_LEFT_CGuiControl, 0x00899638);
CSF_GLOBAL(const char*, g_pszKey_WIDTH_CGuiControl, 0x0089963C);
CSF_GLOBAL(const char*, g_pszKey_HEIGHT_CGuiControl, 0x00899640);

// Reads the four layout keys (.TOP/.LEFT/.WIDTH/.HEIGHT) from a property
// block and stores them into the owner object at +0x180..+0x18C.
static void __fastcall HudSpriteLayout_ReadRectFromProperties(void* self, void* /*edx*/, S_00692b80* a2)
{
    if (a2 == nullptr)
    {
        return;
    }

    auto* props = reinterpret_cast<std::uint8_t*>(a2);

    int top = ReadIntegerProperty(props, nullptr, const_cast<char*>(g_pszKey_TOP_CGuiControl));
    int left = ReadIntegerProperty(props, nullptr, const_cast<char*>(g_pszKey_LEFT_CGuiControl));
    int width = ReadIntegerProperty(props, nullptr, const_cast<char*>(g_pszKey_WIDTH_CGuiControl));
    int height = ReadIntegerProperty(props, nullptr, const_cast<char*>(g_pszKey_HEIGHT_CGuiControl));

    CSF_AT(int, self, 0x184) = top;
    CSF_AT(int, self, 0x180) = left;
    CSF_AT(int, self, 0x188) = width;
    CSF_AT(int, self, 0x18C) = height;
}

HOOK(0x436840, HudSpriteLayout_ReadRectFromProperties, "auto");
