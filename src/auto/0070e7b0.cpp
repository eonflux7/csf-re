#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

using SleepImportFn = void(__stdcall*)(uint32_t);
CSF_GLOBAL(SleepImportFn, g_SleepImportSlot, 0x0082D05C);

static int __fastcall C_008639b0_ReleaseAll(C_008639b0* self, void* /*edx*/) {
    uint8_t readyFlag = self->field_0045;
    self->vtbl = CSF_VTBL(0x008639B0);
    if (!readyFlag) {
        self->field_0040 = 1;
        do {
            g_SleepImportSlot(0);
        } while (!self->field_0045);
    }
    if (self->field_0014) {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0014));
        self->field_0014 = 0;
    }
    void* nestedObject = reinterpret_cast<void*>(self->field_0018);
    if (nestedObject) {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, nestedObject, 0)[0])(nestedObject, nullptr, 1);
        self->field_0018 = 0;
    }
    int result = static_cast<int>(self->field_0024);
    if (result) {
        result = CoreMemory_Free(reinterpret_cast<void*>(self->field_0024));
        self->field_0024 = 0;
    }
    self->vtbl = CSF_VTBL(0x00863958);
    return result;
}

HOOK(0x0070E7B0, C_008639b0_ReleaseAll, "auto");
