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

CSF_FUNC(char, __cdecl, RpSkin_UpdateSkinSubObject, 0x006EFB20, (int a1, void* a2));

// __thiscall: self arrives in ecx, edx unused, no stack arguments (ret 0).
// Forwards this->field_0004 and the embedded sub-object at this+8 to the skin update routine.
static int __fastcall RpSkin_ForwardSkinSubObjectUpdate(void* self, void* /*edx*/)
{
    int skinHandle = CSF_AT(int, self, 0x04);
    void* subObject = (uint8_t*)self + 8;
    return RpSkin_UpdateSkinSubObject(skinHandle, subObject);
}

HOOK(0x006C6200, RpSkin_ForwardSkinSubObjectUpdate, "auto");
