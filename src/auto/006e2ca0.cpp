#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(double, g_TinyPositiveDouble, 0x861A38);
CSF_GLOBAL(double, g_ZeroDouble, 0x846120);

static int __fastcall C_00860120_ResetImmediateState(C_00860120* self, void* /*edx*/) {
    self->field_0020 = g_TinyPositiveDouble;
    self->field_0014 = 0x800000;
    self->field_0018 = 0x800000;
    self->field_0028 = g_ZeroDouble;
    self->field_0030 = 0;
    self->field_0034 = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0x7F7FFFFF;
    CSF_AT(uint32_t, self, 0x10) = 0;
    self->field_0038 = 2;

    void* probe = reinterpret_cast<void*>(self->field_003c);
    int result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, probe, 0)[3])(probe, nullptr);
    if (!static_cast<uint8_t>(result)) {
        void* fallback = reinterpret_cast<void*>(self->field_003c);
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, fallback, 0)[2])(fallback, nullptr);
    }
    return result;
}

HOOK(0x6E2CA0, C_00860120_ResetImmediateState, "auto");
