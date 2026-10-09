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

CSF_FUNC(void, __fastcall, sub_497750, 0x00497750, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_498F50, 0x00498F50, (void* self, void* edx, int a2, char a3));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_483540, 0x00483540, (void* self, void* edx, float a2));

// Original: 0x004839D0, __thiscall, ret 0.
// Resets the child handle at +8 (two flag calls), then passes a float read from the
// value block of the object returned by the virtual slot 0xD8 to sub_483540.
static int __fastcall CCharActionFsm_ResetHandleAndApplyValue(CCharActionFsm* self, void* /*edx*/)
{
    sub_497750(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008)), nullptr);
    sub_498F50(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008)), nullptr, 0x4000, 1);
    sub_498F50(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008)), nullptr, 64, 0);

    C_00862490* handleObject = reinterpret_cast<C_00862490*>(self->vSlotD8());
    int valuePointer = Handle_GetValuePointer(handleObject, nullptr);
    float value = CSF_AT(float, reinterpret_cast<void*>(static_cast<uintptr_t>(valuePointer)), 448);
    return sub_483540(self, nullptr, value);
}

HOOK(0x004839D0, CCharActionFsm_ResetHandleAndApplyValue, "auto");
