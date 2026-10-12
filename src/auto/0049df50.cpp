#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_49D6C0, 0x0049D6C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_49D8A0, 0x0049D8A0, (void* self, void* edx));

// Dispatches on the attached object's state code (vtable slot 0x98) when present,
// otherwise on the local state code field_0050 (range 14..16).
static int __fastcall C_00834050_DispatchByStateOrCode(C_00834050* self, void* /*edx*/, int /*a2*/)
{
    void* stateObject = reinterpret_cast<void*>(self->field_0034);
    if (stateObject) {
        using SlotFn = int(__fastcall*)(void*, void*);
        int stateCode = reinterpret_cast<SlotFn>(CSF_AT(void**, stateObject, 0)[0x98 / 4])(stateObject, nullptr);
        if (stateCode == 10)
            return sub_49D8A0(self, nullptr);
    } else {
        int localStateCode = static_cast<int>(self->field_0050);
        if (localStateCode >= 14 && localStateCode <= 16)
            return sub_49D8A0(self, nullptr);
    }
    return sub_49D6C0(self, nullptr);
}

HOOK(0x0049DF50, C_00834050_DispatchByStateOrCode, "auto");
