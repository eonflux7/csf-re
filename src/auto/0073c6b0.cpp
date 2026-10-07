#include <cstdint>
#include "globals.h"
#include "hook.h"

// Object whose address is loaded into ecx (this) before the tail call: `mov ecx, 0x8BABC0`.
CSF_GLOBAL(uint8_t, g_GlobalObject_8BABC0, 0x008BABC0);

// 0x0068F650 : __thiscall callee, one float stack argument, cleans 4 stack bytes (ret 4).
CSF_FUNC(int, __fastcall, Sub_68F650, 0x0068F650, (void* self, void* /*edx*/, float a1));

// 0x0073C6B0 : forwards its float argument to Sub_68F650 with this = &g_GlobalObject_8BABC0.
static int __stdcall ForwardFloatToGlobalObject(float a1)
{
    return Sub_68F650(&g_GlobalObject_8BABC0, nullptr, a1);
}

HOOK(0x0073C6B0, ForwardFloatToGlobalObject, "auto");
