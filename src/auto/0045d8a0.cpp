#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callees of the original function.
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __fastcall, sub_42DF60, 0x0042DF60, (void* self, void* edx, int a1, float a2));

// Reads the record chain from the nested reference and forwards one int and one float to the sink.
// The first two stack arguments of the original are unused.
static int __stdcall ForwardNestedRecordValueToSink(int /*unusedA*/, int /*unusedB*/, uint32_t* recordRef) {
    // *recordRef -> X; X+4 -> Y; Y+0x1C is the element count.
    uint32_t recordX = *recordRef;
    uint32_t recordY = CSF_AT(uint32_t, reinterpret_cast<void*>(recordX), 4);
    uint32_t elementCount = CSF_AT(uint32_t, reinterpret_cast<void*>(recordY), 0x1C);

    // Small counts use the inline slot at Y+8 (address); larger counts load the pointer stored there.
    uint32_t slotPointer = CSF_AT(uint32_t, reinterpret_cast<void*>(recordY), 8);
    int payloadPointer = (elementCount < 0x10)
        ? static_cast<int>(recordY + 8)
        : static_cast<int>(slotPointer);

    // recordRef[1] -> Z; Z+4 -> W; W+4 is the float value.
    uint32_t recordZ = recordRef[1];
    uint32_t recordW = CSF_AT(uint32_t, reinterpret_cast<void*>(recordZ), 4);
    float scaleValue = CSF_AT(float, reinterpret_cast<void*>(recordW), 4);

    // Values are loaded before this call, as in the original. The returned object becomes `this`.
    int32_t globalObject = GetGlobal8afc78();
    void* sinkSelf = reinterpret_cast<void*>(static_cast<uintptr_t>(globalObject));

    return sub_42DF60(sinkSelf, nullptr, payloadPointer, scaleValue);
}

HOOK(0x0045D8A0, ForwardNestedRecordValueToSink, "auto");
