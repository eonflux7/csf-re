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

CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008630e0_InitBase, 0x006FF930, (void* self, void* edx));
CSF_FUNC(int, __cdecl, ConvertObjectToHandle, 0x0081BAD0, (int a1));

// Constructor of C_00863588: runs the base-class init, stores its vtable,
// keeps the owner object, resolves a handle from it and clears the state flags.
static C_00863588* __fastcall C_00863588_Construct(C_00863588* self, void* /*edx*/, C_0085ec18* owner)
{
    C_008630e0_InitBase(self, nullptr);
    self->vtbl = CSF_VTBL(0x00863588);
    self->field_002c = (uint32_t)owner;
    int referencedObject = (int)GetReferencedObject(owner, nullptr);
    self->field_0004 = (uint32_t)ConvertObjectToHandle(referencedObject);
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    return self;
}

HOOK(0x70A420, C_00863588_Construct, "auto");
