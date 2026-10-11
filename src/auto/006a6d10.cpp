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

CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

// C_0085fe1c destructor-style teardown: resets the vtable, destroys the embedded
// temporary string at +0x8, and optionally releases the transport storage (bit 0 of a2).
// __thiscall, ret 4 (one stack argument).
static uint32_t* __fastcall C_0085fe1c_Destroy(C_0085fe1c* self, void* /*edx*/, char a2)
{
    *(void***)self = CSF_VTBL(0x0085FE1C);
    DestroyTemporaryString(reinterpret_cast<uint8_t*>(self) + 8, nullptr);
    if ((a2 & 1) != 0)
    {
        NetSession_ReleaseTransportStorage(static_cast<int>(reinterpret_cast<uintptr_t>(self)));
    }
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x6A6D10, C_0085fe1c_Destroy, "auto");
