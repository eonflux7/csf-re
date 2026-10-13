#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwD3D9GetCurrentD3DDevice, 0x0062B670, ());
CSF_FUNC(void*, __cdecl, RwD3D9GetCaps, 0x0062C110, ());

// Caches the current D3D9 device in the render engine, then clears bit 7
// (0x80) of the caps dword at offset 0xEC when that bit is set.
static void* __fastcall C_00861460_CacheD3DDeviceAndClearCapsBit7(C_00861460* self, void* /*edx*/)
{
    self->field_005c = static_cast<uint32_t>(RwD3D9GetCurrentD3DDevice());

    void* caps = RwD3D9GetCaps();
    uint32_t capsFlags = CSF_AT(uint32_t, caps, 0xEC);
    if ((capsFlags & 0x80u) != 0)
    {
        CSF_AT(uint32_t, caps, 0xEC) = capsFlags & 0xFFFFFF7Fu;
    }
    return caps;
}

HOOK(0x006DC290, C_00861460_CacheD3DDeviceAndClearCapsBit7, "auto");
