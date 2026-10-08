// Module: menu_game_options
// Refreshes the mute/voice toggle button text for CMenuGameOptions.

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

CSF_FUNC(int, __cdecl, GetGlobalValue, 0x75FDF0, (void));
CSF_GLOBAL(const char*, g_pszMenu_P_Option, 0x00899ACC);
CSF_GLOBAL(const char*, g_pszMenu_P_OptIng, 0x00899AD4);

static int __fastcall CMenuGameOptions_RefreshMuteButtonText(CMenuGameOptions* self, void* /*edx*/)
{
    uint8_t* globals = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue()));
    if (globals[0xCC])
    {
        void* extra = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0020));
        if (extra)
        {
            void** extraVt = CSF_AT(void**, extra, 0);
            ((void(__fastcall*)(void*, void*))extraVt[2])(extra, nullptr);
        }
    }

    void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0018));
    void** ownerVt = CSF_AT(void**, owner, 0);
    ((void(__fastcall*)(void*, void*, void*, int))ownerVt[24])(owner, nullptr, self, 0);

    void** selfVt = CSF_AT(void**, self, 0);
    uint8_t toggled = ((uint8_t(__fastcall*)(void*, void*))selfVt[12])(self, nullptr);

    const char* text = toggled ? g_pszMenu_P_OptIng : g_pszMenu_P_Option;

    owner = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0018));
    ownerVt = CSF_AT(void**, owner, 0);
    return ((int(__fastcall*)(void*, void*, const char*))ownerVt[22])(owner, nullptr, text);
}

HOOK(0x0053B470, CMenuGameOptions_RefreshMuteButtonText, "auto");
