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

// Visitor callback type taken by RpClumpForAllAtomics (RpAtomic* atomic, void* data) -> RpAtomic*.
typedef int(__cdecl* AtomicVisitorFn)(void* atomic, void* data);

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));
CSF_FUNC(void, __fastcall, FinalizeAtomicVisitState, 0x006E5940, (void* self, void* edx, int a2));

// Visits every atomic of each non-null child clump in the table at field_0018 (count in field_001c).
static void __fastcall C_00860288_VisitChildClumpAtomics(C_00860288* self, void* /*edx*/, int a2)
{
    SceneLoader_NoOp();

    for (uint32_t i = 0; i < self->field_001c; ++i)
    {
        // Re-read the table pointer each iteration: callbacks may modify the object.
        uint32_t* children = reinterpret_cast<uint32_t*>(self->field_0018);
        void* child = reinterpret_cast<void*>(children[i]);
        if (child == nullptr)
            continue;

        // Child's virtual slot 4 (+0x10): returns the visit context value for this child.
        int childContext = ((int(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[4])(child, nullptr);

        // Stack state handed to the visitor by pointer: v6 @0, v7 @4, v8 @8.
        alignas(8) uint8_t visitState[0xC];
        CSF_AT(int, visitState, 0) = childContext;
        CSF_AT(int, visitState, 4) = 0;
        CSF_AT(uint8_t, visitState, 8) = (i == 0) ? 1 : 0;

        RpClumpForAllAtomics(a2, reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006B3280)), reinterpret_cast<int>(visitState));

        FinalizeAtomicVisitState(child, nullptr, CSF_AT(int, visitState, 4));
    }
}

HOOK(0x006B3BB0, C_00860288_VisitChildClumpAtomics, "auto");
