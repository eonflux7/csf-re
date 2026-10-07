#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00714100 (32 bytes)
// Calls virtual slot +0xA4, reads the dword at +4 of its result, then subtracts
// the two dwords stored at +0x178 and +0x174 of the list box.
static int __fastcall CGuiListBoxBase_ComputeScrollRange(CGuiListBoxBase* self, void* /*edx*/)
{
    void** vtbl = CSF_AT(void**, self, 0);
    void* pInfo = ((void* (__fastcall*)(CGuiListBoxBase*, void*))vtbl[0xa4 / 4])(self, nullptr);

    // Fields are reloaded after the virtual call (the callee may have changed them).
    int dimension = static_cast<int>(CSF_AT(uint32_t, pInfo, 4));
    return dimension - static_cast<int>(self->field_0178) - static_cast<int>(self->field_0174);
}

HOOK(0x714100, CGuiListBoxBase_ComputeScrollRange, "auto");
