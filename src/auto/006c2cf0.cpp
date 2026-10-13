#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, RecordObject_PreCleanup, 0x006C2410, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorDestroy, 0x0061EE90, (int a1));

static void* __fastcall RecordObject_Cleanup(S_006c20f0* self, void* /*edx*/)
{
    RecordObject_PreCleanup(self, nullptr);

    void* result = reinterpret_cast<void*>(self->field_0000);
    if (self->field_0000) {
        result = reinterpret_cast<void*>(RtAnimInterpolatorDestroy(static_cast<int>(self->field_0000)));
        self->field_0000 = 0;
    }
    return result;
}

HOOK(0x006C2CF0, RecordObject_Cleanup, "auto");
