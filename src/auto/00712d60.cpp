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

CSF_FUNC(void*, __fastcall, ResetEmbeddedMemberAt8, 0x00712D10, (void** self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// CInputBase teardown: resets the vtable, resets the embedded member at +0x8,
// and frees the object when bit 0 of a2 is set. __thiscall, ret 4 (one stack argument).
static void** __fastcall CInputBase_Destroy(CInputBase* self, void* /*edx*/, char a2)
{
    *(void***)self = CSF_VTBL(0x00863E58);
    ResetEmbeddedMemberAt8(reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(self) + 8), nullptr);
    if ((a2 & 1) != 0)
    {
        CoreMemory_Free(self);
    }
    return reinterpret_cast<void**>(self);
}

HOOK(0x712D60, CInputBase_Destroy, "auto");
