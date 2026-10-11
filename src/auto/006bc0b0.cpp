#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpPVSWorldGetProgressCallBack, 0x005863D0, (int a1));

// Stores the enable flag at +0x90. When enabled, the flag is replaced by whether
// RpPVSWorldGetProgressCallBack(field_0004) returned a nonzero value.
static int __fastcall C_008605d8_SetProgressCallbackEnabled(C_008605d8* self, void* /*edx*/, char enable) {
    int result = enable;
    self->field_0090 = static_cast<uint8_t>(enable);
    if (enable) {
        result = RpPVSWorldGetProgressCallBack(static_cast<int>(self->field_0004));
        self->field_0090 = (result != 0) ? 1 : 0;
    }
    return result;
}

HOOK(0x6BC0B0, C_008605d8_SetProgressCallbackEnabled, "auto");
