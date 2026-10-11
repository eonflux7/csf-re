#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Binds to CharTaskBase_Prepare, bound by address per the packet.
CSF_FUNC(int, __fastcall, CharTaskBase_Prepare, 0x004DB110, (void* self, void* edx));

// __thiscall method written as a __fastcall free function (ecx=this, edx unused).
// Original: ret 0 (no stack args popped).
static uint32_t* __fastcall TCharTask_PrepareAndClearFlagBit0(void* self, void* /*edx*/)
{
    CharTaskBase_Prepare(self, nullptr);

    // Owner pointer stored at this+0x08 (undeclared field for TCharTask).
    uint32_t owner = CSF_AT(uint32_t, self, 0x08);

    // Flags dword at owner+0x18: clear bit 0.
    uint32_t* flagsSlot = reinterpret_cast<uint32_t*>(owner + 0x18);
    uint32_t flags = *flagsSlot;
    *flagsSlot = flags & 0xFFFFFFFEu;

    return flagsSlot;
}

HOOK(0x004D7F30, TCharTask_PrepareAndClearFlagBit0, "auto");
