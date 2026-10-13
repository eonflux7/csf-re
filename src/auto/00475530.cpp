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

// Releases both pointer tables of a C_00831698 instance: each non-null element
// gets its slot-0 virtual call with argument 1, then the table block is freed.
static int __fastcall C_00831698_ResetAndFreeLists(C_00831698* self, void* /*edx*/) {
    uint32_t i = 0;
    for (i = 0; i < self->field_000c; ++i) {
        uint32_t element = reinterpret_cast<uint32_t*>(self->field_0004)[i];
        if (element != 0) {
            void* obj = reinterpret_cast<void*>(element);
            auto destroy = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, obj, 0)[0]);
            destroy(obj, nullptr, 1);
            reinterpret_cast<uint32_t*>(self->field_0004)[i] = 0;
        }
    }

    uint32_t firstTable = self->field_0004;
    self->field_0008 = 0;
    self->field_000c = 0;
    if (firstTable != 0) {
        CoreMemory_Free(reinterpret_cast<void*>(firstTable));
        self->field_0004 = 0;
    }

    uint32_t j = 0;
    for (j = 0; j < self->field_001c; ++j) {
        uint32_t element = reinterpret_cast<uint32_t*>(self->field_0014)[j];
        if (element != 0) {
            void* obj = reinterpret_cast<void*>(element);
            auto destroy = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, obj, 0)[0]);
            destroy(obj, nullptr, 1);
            reinterpret_cast<uint32_t*>(self->field_0014)[j] = 0;
        }
    }

    uint32_t secondTable = self->field_0014;
    self->field_0018 = 0;
    self->field_001c = 0;
    int result = static_cast<int>(secondTable);
    if (secondTable != 0) {
        result = CoreMemory_Free(reinterpret_cast<void*>(secondTable));
        self->field_0014 = 0;
    }
    return result;
}

HOOK(0x00475530, C_00831698_ResetAndFreeLists, "auto");
