#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));
CSF_FUNC(unsigned int, __fastcall, RefreshAtomicPipelineState, 0x006D2270, (void* self, void* /*edx*/));

static void __fastcall C_008631c8_PrepareAtomicForPipeline(C_00860e68* self, void* /*edx*/, int a2)
{
    if (a2 == 0)
        return;

    // Search the id list (array at +0x88, count at +0x90) for a2.
    uint32_t count = self->field_0090;
    int* slots = reinterpret_cast<int*>(self->field_0088);
    bool found = false;
    for (uint32_t i = 0; i < count; ++i) {
        if (slots[i] == a2) {
            found = true;
            break;
        }
    }

    if (!found) {
        // Grow the array when count reaches capacity (+0x8C); growth step is at +0x94.
        if (self->field_0090 >= CSF_AT(uint32_t, self, 0x8C)) {
            S_0041e5f0_GrowArrayCapacity(reinterpret_cast<S_0041e5f0*>(&self->field_0088),
                                         nullptr,
                                         static_cast<int>(CSF_AT(uint32_t, self, 0x94)));
        }

        uint32_t index = self->field_0090;
        int* array = reinterpret_cast<int*>(self->field_0088);
        array[index] = a2;
        self->field_0090 = index + 1;
        self->field_0098 = static_cast<uint8_t>(self->field_0090 > 1 ? 1 : 0);
    }

    RefreshAtomicPipelineState(self, nullptr);
}

HOOK(0x006D2310, C_008631c8_PrepareAtomicForPipeline, "auto");
