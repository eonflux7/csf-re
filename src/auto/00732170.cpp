#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns element `index` of the float array that C_00832eec keeps at field_001c, widened to double.
static double __fastcall GetFloatArrayElement(C_00832eec* self, void* /*edx*/, int index) {
    float* values = reinterpret_cast<float*>(self->field_001c);
    return static_cast<double>(values[index]);
}

HOOK(0x00732170, GetFloatArrayElement, "auto");
