#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Base-class prologue shared by TCharTask-derived tasks (called with ecx = this).
CSF_FUNC(int, __fastcall, CharTaskBase_Prepare, 0x004DB110, (void* self, void* edx));

// TCharTask::MarkChildFlag2 (0x004D8220): runs the base prepare step, then sets bit 0x2
// in the flags dword (+0x18) of the object referenced by this->+0x08.
// Returns a pointer to that flags dword.
static int* __fastcall TCharTask_MarkChildFlag2(void* self, void* /*edx*/) {
    CharTaskBase_Prepare(self, nullptr);

    uint32_t childAddr = CSF_AT(uint32_t, self, 0x08);
    void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(childAddr));

    int* flagsPtr = reinterpret_cast<int*>(static_cast<uintptr_t>(childAddr + 0x18));
    int flags = CSF_AT(int, child, 0x18);
    *flagsPtr = flags | 2;
    return flagsPtr;
}

HOOK(0x004D8220, TCharTask_MarkChildFlag2, "auto");
