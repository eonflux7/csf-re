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

// Visitor slot 32 (vtbl+0x80): decides whether the walk is needed at all.
static uint8_t InvokeVisitorSlot32(void* visitor, uint8_t* state)
{
    return (*reinterpret_cast<uint8_t(__fastcall***)(void*, void*, int, void*)>(visitor))[0x80 / 4](visitor, nullptr, 0, state);
}

// Visitor slot 17 (vtbl+0x44): visits an object (this) with the state.
static int InvokeVisitorSlot17(void* visitor, void* subject, uint8_t* state)
{
    return (*reinterpret_cast<int(__fastcall***)(void*, void*, void*, void*)>(visitor))[0x44 / 4](visitor, nullptr, subject, state);
}

// Visitor slot 21 (vtbl+0x54): visits an embedded sub-object with the state.
static int InvokeVisitorSlot21(void* visitor, void* subject, uint8_t* state)
{
    return (*reinterpret_cast<int(__fastcall***)(void*, void*, void*, void*)>(visitor))[0x54 / 4](visitor, nullptr, subject, state);
}

// Visitor slot 31 (vtbl+0x7C): completion hook that receives the state.
static int InvokeVisitorSlot31(void* visitor, uint8_t* state)
{
    return (*reinterpret_cast<int(__fastcall***)(void*, void*, void*)>(visitor))[0x7C / 4](visitor, nullptr, state);
}

// One sub-object step: parks state+0x1C at zero, then runs visitor slot 21 on the
// sub-object only while the done byte is still 0.
static void RunChildStepSlot21(uint8_t* state, void* subject)
{
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (CSF_AT(uint8_t, state, 0) == 0) {
        ClearStepScratch(state);
        void* visitor = CSF_AT(void*, state, 4);
        const int stepResult = InvokeVisitorSlot21(visitor, subject, state);
        StoreStepVerdict(state, stepResult);
    }
}

// Walks a composite object through a visitor carried in the state record (state+0x04).
// Visitor slot 32 (vtbl+0x80) decides whether the walk is needed. The walk visits this
// object (slot 17, vtbl+0x44), then five embedded sub-objects (this+8, +0xC, +4, +0x10,
// +0x14) through slot 21 (vtbl+0x54). Each step runs only while the done byte is still 0.
// Visitor slot 31 (vtbl+0x7C) then gets the state as a completion hook.
static int __fastcall TaskStates_WalkVisitorOverFiveSubobjects(void* self, void* /*edx*/, uint8_t* state)
{
    // state+0x1C is parked in a local and written back before the final verdict.
    const uint32_t savedOwnerSlot = CSF_AT(uint32_t, state, 0x1C);

    bool skipWalk = CSF_AT(uint8_t, state, 0) == 0;
    if (skipWalk) {
        void* visitor = CSF_AT(void*, state, 4);
        skipWalk = (InvokeVisitorSlot32(visitor, state) == 0);
    }
    CSF_AT(uint8_t, state, 0) = skipWalk ? 1 : 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;

    // Visit this object (slot 17).
    if (!skipWalk) {
        ClearStepScratch(state);
        void* visitor = CSF_AT(void*, state, 4);
        const int visitResult = InvokeVisitorSlot17(visitor, self, state);
        StoreStepVerdict(state, visitResult);
    }

    // Embedded sub-objects, in the original's order (slot 21 each).
    RunChildStepSlot21(state, (uint8_t*)self + 0x8);
    RunChildStepSlot21(state, (uint8_t*)self + 0xC);
    RunChildStepSlot21(state, (uint8_t*)self + 0x4);
    RunChildStepSlot21(state, (uint8_t*)self + 0x10);
    RunChildStepSlot21(state, (uint8_t*)self + 0x14);

    CSF_AT(uint32_t, state, 0x1C) = savedOwnerSlot;
    if (CSF_AT(uint8_t, state, 0) != 0) {
        CSF_AT(uint8_t, state, 0) = 1;
        return 1;
    }

    // Completion hook: visitor slot 31 (vtbl+0x7C) receives the state.
    void* visitor = CSF_AT(void*, state, 4);
    const int completionResult = InvokeVisitorSlot31(visitor, state);
    if (static_cast<uint8_t>(completionResult) == 0) {
        CSF_AT(uint8_t, state, 0) = 1;
        return 1;
    }

    CSF_AT(uint8_t, state, 0) = 0;
    return completionResult;
}

HOOK(0x0041F200, TaskStates_WalkVisitorOverFiveSubobjects, "auto");
