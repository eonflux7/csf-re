#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Water_InitBaseObject, 0x006FF930, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Water_LookupRefValue, 0x0081C120, (int a2));

static C_00863678* __fastcall C_00863678_Construct(C_00863678* self, void* /*edx*/, C_0085ec18* ref)
{
    Water_InitBaseObject(self, nullptr);
    self->vtbl = CSF_VTBL(0x00863678);

    void* referenced = GetReferencedObject(ref, nullptr);
    self->field_002c = static_cast<uint32_t>(
        Water_LookupRefValue(static_cast<int>(reinterpret_cast<uintptr_t>(referenced))));

    return self;
}

HOOK(0x70B020, C_00863678_Construct, "auto");
