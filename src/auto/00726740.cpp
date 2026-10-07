#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x007264C0 — __thiscall helper called first with (a3, a4); its return value is ignored.
CSF_FUNC(void, __fastcall, C_008655e8_InvokeCallbackPre, 0x007264C0,
         (C_008655e8* self, void* /*edx*/, int a2, int a3));

// Global singleton pointer; the real callback lives at +0x30 of the object it points to.
CSF_GLOBAL(uint32_t, g_008B68C0, 0x008B68C0);

static int __fastcall C_008655e8_InvokeCallback(C_008655e8* self, void* /*edx*/,
                                                int a2, int a3, int a4)
{
    C_008655e8_InvokeCallbackPre(self, nullptr, a3, a4);

    uint32_t target = g_008B68C0;
    int (__cdecl* callback)(int, int, int) =
        reinterpret_cast<int (__cdecl*)(int, int, int)>(
            *reinterpret_cast<uint32_t*>(target + 0x30));

    return callback(a2, a3, a4);
}

HOOK(0x00726740, C_008655e8_InvokeCallback, "auto");
