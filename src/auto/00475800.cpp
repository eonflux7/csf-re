#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// Returns the index at which a2 is stored. If a2 is already present, returns its index
// without modifying the array. Otherwise appends a2 and returns the new element count.
static unsigned int __fastcall S_0044d360_RegisterSceneObjects(S_0044d360* self, void* /*edx*/, int a2)
{
    uint32_t count = self->field_0030;
    unsigned int result = 0;

    if (count != 0) {
        int* elements = reinterpret_cast<int*>(self->field_0028);
        do {
            if (*elements == a2) {
                return result;
            }
            ++result;
            ++elements;
        } while (result < count);
    }

    // Not found: append (original LABEL_5)
    if (self->field_0030 >= CSF_AT(uint32_t, self, 0x2c)) {
        int growAmount = CSF_AT(int, self, 0x34);
        [[maybe_unused]] void* growArrayVtable = CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(reinterpret_cast<S_0041e5f0*>(&self->field_0028), nullptr, growAmount);
    }

    uint32_t oldCount = self->field_0030;
    int* slot = reinterpret_cast<int*>(self->field_0028) + oldCount;
    result = oldCount + 1;
    self->field_0030 = result;
    *slot = a2;
    return result;
}

HOOK(0x00475800, S_0044d360_RegisterSceneObjects, "auto");
