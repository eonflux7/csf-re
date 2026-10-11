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

// Eligibility test for a clump (returns a byte bool, tested with `test al, al`).
CSF_FUNC(char, __cdecl, GfxScene_IsClumpEligible, 0x006C0420, (int a1));

// Grows the clump array's backing storage (__thiscall, one stack argument, ret 4).
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0,
         (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// The original's call site references this vtable; keep the binding in the file.
[[maybe_unused]] static const void* const kClumpArrayVtable = CSF_VTBL(0x0041E5F0);

// Appends clump `clump` to the array `list` when it is eligible; always returns `clump`.
static int GfxScene_AppendClumpIfEligible(int clump, S_0041e5f0* list) {
    if (GfxScene_IsClumpEligible(clump)) {
        if (list->field_0008 >= list->field_0004) {
            S_0041e5f0_GrowArrayCapacity(list, nullptr, list->field_000c);
        }
        uint32_t index = list->field_0008;
        int* slot = reinterpret_cast<int*>(list->field_0000 + 4 * index);
        list->field_0008 = index + 1;
        *slot = clump;
    }
    return clump;
}

HOOK(0x006CCD30, GfxScene_AppendClumpIfEligible, "auto");
