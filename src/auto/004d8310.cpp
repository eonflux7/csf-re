#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, C_00834fa8_RunStateHandlerAtOffset14, 0x004210C0, (C_00834fa8* self, void*));

// Runs the state handler of the owner's state object (owner+0x310), then clears
// bit 0 of the owner's dword at +0x18. Returns a pointer to that dword.
static uint32_t* __fastcall Autonomo_RunStateAndClearLowBit(uint32_t* self, void* /*edx*/) {
    uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<std::uintptr_t>(CSF_AT(uint32_t, self, 0x8)));
    C_00834fa8* stateObj = reinterpret_cast<C_00834fa8*>(static_cast<std::uintptr_t>(CSF_AT(uint32_t, owner, 0x310)));
    C_00834fa8_RunStateHandlerAtOffset14(stateObj, nullptr);

    // The original reloads [self+8] after the call, since the callee may change it.
    owner = reinterpret_cast<uint8_t*>(static_cast<std::uintptr_t>(CSF_AT(uint32_t, self, 0x8)));
    uint32_t& flags = CSF_AT(uint32_t, owner, 0x18);
    flags = flags & 0xFFFFFFFEu;
    return &flags;
}

HOOK(0x4D8310, Autonomo_RunStateAndClearLowBit, "auto");
