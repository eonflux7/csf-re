#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound callees (addresses from the packet).
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0,
         (S_0041e5f0* self, void* edx, int additionalCapacity));
CSF_FUNC(void, __fastcall, TAiState_RunBaseStep, 0x004DBEB0,
         (void* self, void* edx, int a2));

// Vtable referenced by this function's list-growth path (gate reference).
[[maybe_unused]] static void* const kGrowArrayVtbl = CSF_VTBL(0x0041E5F0);

static void __fastcall TAiState_RequeueChildAndStep(TAiState* self, void* /*edx*/, int a2)
{
    // Child object held at this+0x0C.
    void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x0C)));
    if (child)
    {
        void* childComponent =
            reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, child, 0x10)));
        if (childComponent)
        {
            // Virtual slot +0x30 on the component, passing the child as argument.
            auto detachFn = reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, childComponent, 0)[0x30 / 4]);
            detachFn(childComponent, nullptr, child);

            // Child may have been changed by the call: reload it.
            void* childAfter =
                reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x0C)));
            CSF_AT(uint32_t, childAfter, 0x10) = 0;

            // Growable pointer list owned at this+0x04 + 0x14.
            S_0041e5f0* list = reinterpret_cast<S_0041e5f0*>(
                static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x04)) + 0x14);

            if (list->field_0008 >= list->field_0004)
                S_0041e5f0_GrowArrayCapacity(list, nullptr, list->field_000c);

            uint32_t index = list->field_0008;
            uint32_t* slot = reinterpret_cast<uint32_t*>(
                static_cast<uintptr_t>(list->field_0000) + 4u * index);
            list->field_0008 = index + 1;
            *slot = CSF_AT(uint32_t, self, 0x0C);
        }
    }

    TAiState_RunBaseStep(self, nullptr, a2);

    // Re-read the child after the base step.
    void* last = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x0C)));
    if (last)
    {
        void* lastComponent =
            reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, last, 0x10)));
        if (lastComponent)
        {
            // Virtual slot +0x28 on the component, passing the child as argument.
            auto notifyFn = reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, lastComponent, 0)[0x28 / 4]);
            notifyFn(lastComponent, nullptr, last);
        }
    }
}

HOOK(0x004DBFE0, TAiState_RequeueChildAndStep, "auto");
