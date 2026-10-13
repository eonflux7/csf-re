#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0085e92c_Refresh, 0x0069C950, (void* self, void* edx));

static int __fastcall C_0085e92c_RefreshThenGetField0034(C_0085e92c* self, void* /*edx*/)
{
    C_0085e92c_Refresh(self, nullptr);
    return self->field_0034;
}

HOOK(0x0069CBD0, C_0085e92c_RefreshThenGetField0034, "auto");
