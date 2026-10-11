#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008316d8_AdvanceShellCasingState(C_008316d8* self, void* /*edx*/)
{
    const int result = static_cast<int32_t>(self->field_0020);
    if (result > 0)
    {
        self->field_001c += CSF_AT(uint32_t,
            reinterpret_cast<void*>(self->field_0040), 0x104);
        return reinterpret_cast<int(__fastcall*)(C_008316d8*, void*, int)>(
            CSF_AT(void**, self, 0)[0x78 / 4])(self, nullptr, 1);
    }
    return result;
}

HOOK(0x00476760, C_008316d8_AdvanceShellCasingState, "auto");
