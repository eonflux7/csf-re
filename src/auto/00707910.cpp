#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Adds a 3-float offset to the position vector at this->field_0014, then calls
// virtual slot 35 (+0x8C) on the child object at field_0008 with field_000c as its
// argument. Marks the object as dirty (field_0020 = 1) when a vector is present.
static int __fastcall AnimInterpolator_AddOffsetAndRefresh(C_008633a0* self, void* /*edx*/, float* a2) {
    int result = 0;
    if (self->field_0014) {
        float* vector = reinterpret_cast<float*>(self->field_0014);
        *vector = *vector + a2[0];

        vector = reinterpret_cast<float*>(self->field_0014);
        vector[1] = a2[1] + vector[1];

        vector = reinterpret_cast<float*>(self->field_0014);
        vector[2] = a2[2] + vector[2];

        void* child = reinterpret_cast<void*>(self->field_0008);
        result = reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
                     CSF_AT(void**, child, 0)[0x8c / 4])(child, nullptr, self->field_000c);

        self->field_0020 = 1;
    }
    return result;
}

HOOK(0x707910, AnimInterpolator_AddOffsetAndRefresh, "auto");
