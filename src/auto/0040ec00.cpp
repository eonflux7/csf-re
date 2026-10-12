#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee at 0x0070ECB0: takes the linked object as `self` and returns a char state.
CSF_FUNC(char, __fastcall, ComputeLinkedObjectState, 0x0070ECB0, (void* self, void* edx));

// Reads the linked object pointer stored at +0x1C and tail-forwards it to the
// state computation. The original is a tail jmp, so edx is passed through unchanged.
static char __fastcall GetLinkedObjectState(void* self, void* edx)
{
    void* linkedObject = CSF_AT(void*, self, 0x1C);
    return ComputeLinkedObjectState(linkedObject, edx);
}

HOOK(0x0040EC00, GetLinkedObjectState, "auto");
