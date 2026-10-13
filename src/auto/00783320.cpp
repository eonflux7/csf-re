#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(int, g_Value8BC1EC, 0x008BC1EC);

CSF_FUNC(int, __fastcall, sub_783260, 0x00783260, (void* self, void* edx, int a4));

static int ReleasePendingValueIfSet()
{
    int result = 0;

    if (g_Value8BC1EC)
    {
        result = sub_783260(reinterpret_cast<void*>(static_cast<intptr_t>(g_Value8BC1EC)), nullptr, 1);
        g_Value8BC1EC = 0;
    }
    return result;
}

HOOK(0x00783320, ReleasePendingValueIfSet, "auto");
