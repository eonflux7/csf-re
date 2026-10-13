#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0083e9d0_InvokeVtblB0, 0x4E5290, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083ef30_ForwardVtbl8C, 0x4F0CC0, (void* self, void* edx));

static int __fastcall C_00833b88_RunSubObjectHandlers(C_00833b88* self, void* /*edx*/)
{
    int result = 0;

    auto* first = reinterpret_cast<C_0083e9d0*>(self->field_003c);
    if (first) {
        result = C_0083e9d0_InvokeVtblB0(first, nullptr);
    }

    auto* second = reinterpret_cast<C_0083ef30*>(self->field_0040);
    if (second) {
        return C_0083ef30_ForwardVtbl8C(second, nullptr);
    }
    return result;
}

HOOK(0x492BF0, C_00833b88_RunSubObjectHandlers, "auto");
