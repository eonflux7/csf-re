// Reimplementation of 0x004BF4D0 (ai_core, "-- Fin Debug IA --").
// Resolves the linked object for `a1` (twice), applies a setup call, grows the
// debug-entry array if it is full, appends code 31 and sets its range params.

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

// Callee bindings (one name and signature per address).
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0,
         (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));
CSF_FUNC(int, __fastcall, UnnamedSetRangeParams_4BC3A0, 0x004BC3A0,
         (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0,
         (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180,
         (void* self, void* edx, int a2));

// Vtable of the debug-entry array class; referenced by the original's call site.
[[maybe_unused]] static void* const kDebugEntryListVtable = CSF_VTBL(0x0041E5F0);

// Resolves the object linked to `owner`, returning its base (linked - 4) or 0.
static uintptr_t ResolveLinkedBase(void* owner) {
    int linked = GetLinkedObjectValue_4DB5C0(owner, nullptr);
    return linked ? static_cast<uintptr_t>(linked - 4) : 0;
}

static int __stdcall AiCore_PushFinDebugIaEntry(int a1) {
    void* owner = reinterpret_cast<void*>(a1);

    uintptr_t firstBase = ResolveLinkedBase(owner);
    UnnamedApply_4DC180(owner, nullptr, static_cast<int>(firstBase + 1088));

    uintptr_t base = ResolveLinkedBase(owner);
    auto* list = reinterpret_cast<S_0041e5f0*>(base + 1292);
    uint8_t* baseBytes = reinterpret_cast<uint8_t*>(base);

    // Unsigned compare per the original's jb.
    if (CSF_AT(uint32_t, baseBytes, 0x514) >= CSF_AT(uint32_t, baseBytes, 0x510)) {
        S_0041e5f0_GrowArrayCapacity(list, nullptr,
                                     static_cast<int>(CSF_AT(uint32_t, baseBytes, 0x518)));
    }

    uint32_t index = list->field_0008;
    uint32_t* slots = reinterpret_cast<uint32_t*>(list->field_0000);
    list->field_0008 = index + 1;
    slots[index] = 31;

    return UnnamedSetRangeParams_4BC3A0(list, nullptr, 1.0f, 3.0f);
}

HOOK(0x4BF4D0, AiCore_PushFinDebugIaEntry, "auto");
