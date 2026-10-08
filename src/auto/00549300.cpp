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

// Menu string "P_Video" (pointer stored at 0x00899AB0)
CSF_GLOBAL(const char*, g_pszMenu_P_Video, 0x00899AB0);

// Resets the advanced-video flag, asks the parent screen to switch screens,
// then looks up the video menu child by name and activates it.
static int __fastcall CMenuVideoAdvanced_ReturnToVideoMenu(CMenuVideoAdvanced* self, void* /*edx*/)
{
    // Original: v2 = *(this + 0x14); this->field_0050 = 0;
    self->field_0050 = 0;

    // Call vtbl[+0x60] on parent with (this, 0)
    {
        void* parent = reinterpret_cast<void*>(self->field_0014);
        ((void(__fastcall*)(void*, void*, void*, int))
            CSF_AT(void**, parent, 0)[0x60 / 4])(parent, nullptr, static_cast<void*>(self), 0);
    }

    // v3 = parent->vtbl[+0x5C](g_pszMenu_P_Video)
    void* parentAgain = reinterpret_cast<void*>(self->field_0014);
    void* child = ((void*(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, parentAgain, 0)[0x5C / 4])(parentAgain, nullptr, g_pszMenu_P_Video);

    // v4 = v3->vtbl[+0x20]()
    void* childMenu = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, child, 0)[0x20 / 4])(child, nullptr);

    // return v4->vtbl[+0xA8](0)
    return ((int(__fastcall*)(void*, void*, int))
        CSF_AT(void**, childMenu, 0)[0xA8 / 4])(childMenu, nullptr, 0);
}

HOOK(0x00549300, CMenuVideoAdvanced_ReturnToVideoMenu, "auto");
