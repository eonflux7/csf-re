#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Virtual slot +0x08 (slot index 2) of the sub-object type, called with (a2, a3).
using ChainedSlot2Fn = char(__fastcall*)(void* self, void* edx, int a2, int a3);

static char __fastcall C_0083fdf4_RunChainedSlot2Calls(C_0083fdf4* self, void* /*edx*/, int a2, int a3)
{
    char result = 0;

    // First sub-object: always called when present.
    void* firstChild = reinterpret_cast<void*>(self->field_0004);
    if (firstChild) {
        ChainedSlot2Fn fn = reinterpret_cast<ChainedSlot2Fn>(CSF_AT(void**, firstChild, 0)[2]);
        result = fn(firstChild, nullptr, a2, a3);
    }

    // Second sub-object: called only while the previous result is nonzero.
    void* secondChild = reinterpret_cast<void*>(self->field_0008);
    if (secondChild && result) {
        ChainedSlot2Fn fn = reinterpret_cast<ChainedSlot2Fn>(CSF_AT(void**, secondChild, 0)[2]);
        result = fn(secondChild, nullptr, a2, a3);
    }

    // Third sub-object: called only while the previous result is nonzero.
    void* thirdChild = reinterpret_cast<void*>(self->field_000c);
    if (thirdChild && result) {
        ChainedSlot2Fn fn = reinterpret_cast<ChainedSlot2Fn>(CSF_AT(void**, thirdChild, 0)[2]);
        result = fn(thirdChild, nullptr, a2, a3);
    }

    // Fourth sub-object: its result is the function's return value.
    void* fourthChild = reinterpret_cast<void*>(self->field_0010);
    if (fourthChild) {
        if (result) {
            ChainedSlot2Fn fn = reinterpret_cast<ChainedSlot2Fn>(CSF_AT(void**, fourthChild, 0)[2]);
            return fn(fourthChild, nullptr, a2, a3);
        }
    }

    return result;
}

HOOK(0x0050BCB0, C_0083fdf4_RunChainedSlot2Calls, "auto");
