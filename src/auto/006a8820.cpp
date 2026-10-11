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

CSF_FUNC(bool, __fastcall, sub_79FF30, 0x0079FF30, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, sub_790F30, 0x00790F30, (void* self, void* edx));

// Virtual slot at byte offset 0x44 (index 17), called as vtable[17](this, 0).
static int __fastcall C_0085fe28_ProcessStateTransition(C_0085fe28* self, void* /*edx*/, int /*a2*/)
{
    int result = sub_79FF30(reinterpret_cast<void*>(self->field_0330), nullptr);
    if (result) {
        self->field_0334 = 1;
    } else {
        result = (int)self->field_0008;
        if (result == 2 || result == 3 || result == 7) {
            if (!sub_790F30(reinterpret_cast<void*>(self->field_0028), nullptr)) {
                ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x44 / 4])(self, nullptr, 0);
                self->field_0338 = 1;
            }
        }
    }
    return result;
}

HOOK(0x006A8820, C_0085fe28_ProcessStateTransition, "auto");
