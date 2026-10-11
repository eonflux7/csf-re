#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x00402350: S_00402350 initializer, __thiscall (ecx = this).
CSF_FUNC(uint32_t*, __fastcall, InitS00402350Object, 0x00402350, (void* self, void* edx));

// Vtable of S_00402350, referenced by the original's call chain.
static void* const s_S00402350Vtable = CSF_VTBL(0x00402350);

// Initializes the S_00402350 object in place and returns a pointer to its first field.
static uint32_t* __stdcall WaterS00402350_Construct(S_00402350* a1) {
    InitS00402350Object(a1, nullptr);
    return reinterpret_cast<uint32_t*>(&a1->field_0000);
}

HOOK(0x0070C8E0, WaterS00402350_Construct, "auto");
