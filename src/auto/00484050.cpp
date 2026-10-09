#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns 1 when the sub-object held at +0xA4 reports mode 14 through vtable slot 0x4C, otherwise 0.
static int __fastcall CCharActionFsm_IsSubFsmModeFourteen(CCharActionFsm* self, void* /*edx*/)
{
    void* subObject = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_00a4));
    if (subObject == nullptr) {
        return 0;
    }
    int subFsmMode = ((int(__fastcall*)(void*, void*))CSF_AT(void**, subObject, 0)[0x4C / 4])(subObject, nullptr);
    return subFsmMode == 14 ? 1 : 0;
}

HOOK(0x00484050, CCharActionFsm_IsSubFsmModeFourteen, "auto");
