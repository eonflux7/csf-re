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

// Callees (bound by original address; signatures as given by the packet)
CSF_FUNC(char, __fastcall, sub_525C30, 0x00525C30, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_5261C0, 0x005261C0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_5251B0, 0x005251B0, (void* self, void* edx, float a2));

// Refreshes the object state, then applies the float argument (level/value) to it.
// Original: void __thiscall (this, float a2), ret 4.
static void __fastcall S_00525f70_RefreshThenSetLevel(S_00525f70* self, void* /*edx*/, float a2)
{
    sub_525C30(self, nullptr);
    sub_5261C0(self, nullptr);
    sub_5251B0(self, nullptr, a2);
}

HOOK(0x00526300, S_00525f70_RefreshThenSetLevel, "auto");
