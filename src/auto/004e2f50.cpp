#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base constructor for C_0083e160 (`this` ctor chain entry, ecx=this, two stack args, ret 8).
CSF_FUNC(uint32_t*, __fastcall, sub_4E1200, 0x004E1200, (void* self, void* edx, int a2, int a3));
// Returns the global state object, cdecl, no args.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
// Returns the menu transition scale, thiscall with the state object as `this`.
CSF_FUNC(float, __fastcall, MenuSystem_GetTransitionScale_50D860, 0x0050D860, (void* self, void* edx));

// Constructor of C_0083e248: runs the base constructor, installs this class's vtable and
// default field values, then rescales the field_0008 value when a2 is a state with type 1.
static int __fastcall C_0083e248_Construct(C_0083e248* self, void* /*edx*/, int a2, int a3)
{
    sub_4E1200(self, nullptr, a2, a3);

    self->vtbl = CSF_VTBL(0x0083E248);
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x0083E22C)));

    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 10;
    self->field_0018 = 0;
    self->field_0020 = 0.0f;
    self->field_001c = 0.0f;
    self->field_0024 = 0.0f;
    self->field_0028 = 0.0f;
    self->field_002c = 0;
    self->field_002d = 0;
    self->field_0030 = 5.0f;
    self->field_0044 = 0;
    self->field_0048 = 0;

    if (a2 != 0 && CSF_AT(int, reinterpret_cast<void*>(a2), 0x50) == 1)
    {
        int globalState = GetGlobalState_50D190();
        float transitionScale = MenuSystem_GetTransitionScale_50D860(reinterpret_cast<void*>(globalState), nullptr);
        self->field_0008 = static_cast<uint32_t>(
            static_cast<int64_t>(transitionScale * static_cast<double>(static_cast<int>(self->field_0008))));
    }

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x004E2F50, C_0083e248_Construct, "auto");
