#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(int, g_Value8AFC84, 0x008AFC84);

CSF_FUNC(void**, __fastcall, sub_44B270, 0x0044B270, (void* self, void* edx, char a2));

static void** ReleasePendingHandleIfSet()
{
    void** result = nullptr;

    if (g_Value8AFC84)
    {
        result = sub_44B270(reinterpret_cast<void*>(static_cast<intptr_t>(g_Value8AFC84)), nullptr, 1);
        g_Value8AFC84 = 0;
    }
    return result;
}

HOOK(0x0044B8A0, ReleasePendingHandleIfSet, "auto");
