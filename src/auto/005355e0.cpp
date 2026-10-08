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

// Intro-video timer tick: accumulates elapsed time and, once the duration is
// reached, stops the video player, resets the timer and shows the menu entry
// string through the owning menu object.
CSF_GLOBAL(float, g_fIntroVideoDuration, 0x0082E118);
CSF_GLOBAL(const char*, g_pszMenu_P_MePrin, 0x00899AE4);

static void __fastcall CMenuIntroVideo_AdvanceTimerAndFinish(CMenuIntroVideo* self, void* /*edx*/, float elapsedSeconds)
{
    // self->field_002c += elapsedSeconds (stored as float)
    float accumulated = elapsedSeconds + self->field_002c;
    self->field_002c = accumulated;

    if (accumulated >= g_fIntroVideoDuration)
    {
        // Player vtable slot 54 (+0xD8): Stop(0)
        void* player = reinterpret_cast<void*>(self->field_0018);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, player, 0)[54])(player, nullptr, 0);

        // Owner object: reset counter, then vtable slot 24 (+0x60) with (self, 0)
        void* owner = reinterpret_cast<void*>(self->field_0010);
        self->field_0014 = 0;
        ((void(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, owner, 0)[24])(owner, nullptr, self, 0);

        // Reload owner, then vtable slot 22 (+0x58) with the menu string
        owner = reinterpret_cast<void*>(self->field_0010);
        ((void(__fastcall*)(void*, void*, const char*))CSF_AT(void**, owner, 0)[22])(owner, nullptr, g_pszMenu_P_MePrin);
    }
}

HOOK(0x005355E0, CMenuIntroVideo_AdvanceTimerAndFinish, "auto");
