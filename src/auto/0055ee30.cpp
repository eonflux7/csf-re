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

// Helper macros are provided by the project headers (CSF_FUNC, CSF_GLOBAL, CSF_AT, HOOK).

CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x550CC0, (void));
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);
CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);

// Refreshes the online-match menu: pushes an availability flag into the child
// object at +0x1C (vtable slot 0xA8), then, if the object at +0x28 exists,
// calls its slot 0xA8 with argument 1 and returns that result.
static int __fastcall CMenuMpOnline_RefreshOnlineMatchState(CMenuMpOnline* self, void* /*edx*/)
{
    if (self->field_001c)
    {
        int enableFlag;
        if (GetGlobal8B0688())
        {
            GetGlobal8B0688();
            enableFlag = (g_OnlineMatchFlag == 0) ? 1 : 0;
        }
        else
        {
            enableFlag = (g_MpNetworkType == 2) ? 1 : 0;
        }

        void* childA = reinterpret_cast<void*>(self->field_001c);
        ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, childA, 0)[0xA8 / 4])(childA, nullptr, enableFlag);
    }

    int result = 0;
    if (self->m_friendsButton)
    {
        void* childB = reinterpret_cast<void*>(self->m_friendsButton);
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, childB, 0)[0xA8 / 4])(childB, nullptr, 1);
    }
    return result;
}

HOOK(0x0055EE30, CMenuMpOnline_RefreshOnlineMatchState, "auto");
