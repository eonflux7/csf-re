#include <cstdint>
#include "globals.h"
#include "hook.h"

// Clears the scratch bytes at state+0x15/+0x16 before a visitor step.
static void ClearStepScratch(uint8_t* state)
{
    CSF_AT(uint8_t, state, 0x15) = 0;
    CSF_AT(uint8_t, state, 0x16) = 0;
}

// Stores a step verdict in the done byte: 1 when the visitor returned -1, otherwise 0.
static void StoreStepVerdict(uint8_t* state, int stepResult)
{
    CSF_AT(uint8_t, state, 0) = (stepResult == -1) ? 1 : 0;
}

// Walks a composite object through a visitor carried in the state record (state+0x04).
// Visitor slot 32 (vtbl+0x80) decides whether the walk is needed. The walk visits this
// object (slot 22, vtbl+0x58) and then five embedded sub-objects (this+4, +0x15, +0x10,
// +0x16, +0x14) through slots 14/17/18/17/17. Each step runs only while the done byte
// is still 0. Visitor slot 31 (vtbl+0x7C) then gets the state as a completion hook.
static int __fastcall TraverseVisitorOverSubobjects(void* self, void* /*edx*/, uint8_t* state)
{
    // state+0x1C is parked in a local and written back before the final verdict.
    const uint32_t savedOwnerSlot = CSF_AT(uint32_t, state, 0x1C);

    bool skipTraversal = CSF_AT(uint8_t, state, 0) == 0;
    if (skipTraversal) {
        void* visitor = CSF_AT(void*, state, 4);
        const uint8_t needsTraversal =
            (*reinterpret_cast<uint8_t(__fastcall***)(void*, void*, int, void*)>(visitor))[0x80 / 4](visitor, nullptr, 0, state);
        skipTraversal = (needsTraversal == 0);
    }
    CSF_AT(uint8_t, state, 0) = skipTraversal ? 1 : 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;

    // Visit this object (slot 22).
    if (!skipTraversal) {
        ClearStepScratch(state);
        void* visitor = CSF_AT(void*, state, 4);
        const int visitResult =
            (*reinterpret_cast<int(__fastcall***)(void*, void*, void*, void*)>(visitor))[0x58 / 4](visitor, nullptr, self, state);
        StoreStepVerdict(state, visitResult);
    }

    // Sub-object at this+4 (slot 14).
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (CSF_AT(uint8_t, state, 0) == 0) {
        ClearStepScratch(state);
        void* visitor = CSF_AT(void*, state, 4);
        const int stepResult =
            (*reinterpret_cast<int(__fastcall***)(void*, void*, void*, void*)>(visitor))[0x38 / 4](visitor, nullptr, (uint8_t*)self + 4, state);
        StoreStepVerdict(state, stepResult);
    }

    // Sub-object at this+0x15 (slot 17).
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (CSF_AT(uint8_t, state, 0) == 0) {
        ClearStepScratch(state);
        void* visitor = CSF_AT(void*, state, 4);
        const int stepResult =
            (*reinterpret_cast<int(__fastcall***)(void*, void*, void*, void*)>(visitor))[0x44 / 4](visitor, nullptr, (uint8_t*)self + 0x15, state);
        StoreStepVerdict(state, stepResult);
    }

    // Sub-object at this+0x10 (slot 18).
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (CSF_AT(uint8_t, state, 0) == 0) {
        ClearStepScratch(state);
        void* visitor = CSF_AT(void*, state, 4);
        const int stepResult =
            (*reinterpret_cast<int(__fastcall***)(void*, void*, void*, void*)>(visitor))[0x48 / 4](visitor, nullptr, (uint8_t*)self + 0x10, state);
        StoreStepVerdict(state, stepResult);
    }

    // Sub-object at this+0x16 (slot 17).
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (CSF_AT(uint8_t, state, 0) == 0) {
        ClearStepScratch(state);
        void* visitor = CSF_AT(void*, state, 4);
        const int stepResult =
            (*reinterpret_cast<int(__fastcall***)(void*, void*, void*, void*)>(visitor))[0x44 / 4](visitor, nullptr, (uint8_t*)self + 0x16, state);
        StoreStepVerdict(state, stepResult);
    }

    // Sub-object at this+0x14 (slot 17).
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (CSF_AT(uint8_t, state, 0) == 0) {
        ClearStepScratch(state);
        void* visitor = CSF_AT(void*, state, 4);
        const int stepResult =
            (*reinterpret_cast<int(__fastcall***)(void*, void*, void*, void*)>(visitor))[0x44 / 4](visitor, nullptr, (uint8_t*)self + 0x14, state);
        StoreStepVerdict(state, stepResult);
    }

    CSF_AT(uint32_t, state, 0x1C) = savedOwnerSlot;
    if (CSF_AT(uint8_t, state, 0) != 0) {
        CSF_AT(uint8_t, state, 0) = 1;
        return 1;
    }

    // Completion hook: visitor slot 31 (vtbl+0x7C) receives the state.
    void* visitor = CSF_AT(void*, state, 4);
    const int completionResult =
        (*reinterpret_cast<int(__fastcall***)(void*, void*, void*)>(visitor))[0x7C / 4](visitor, nullptr, state);
    if (static_cast<uint8_t>(completionResult) == 0) {
        CSF_AT(uint8_t, state, 0) = 1;
        return 1;
    }

    CSF_AT(uint8_t, state, 0) = 0;
    return completionResult;
}

HOOK(0x00464380, TraverseVisitorOverSubobjects, "auto");
