#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Original callee at 0x451100: __thiscall, no stack args, result unused here.
CSF_FUNC(char, __fastcall, RefreshS004503e0State, 0x00451100, (void* self, void* edx));

// Calls the state refresh on this, then returns the address of the embedded
// sub-object at offset 0x74 (lea, not a load).
static char* __fastcall GetEmbeddedBlockAt74(S_004503e0* self, void* /*edx*/)
{
    RefreshS004503e0State(self, nullptr);
    return reinterpret_cast<char*>(self) + 0x74;
}

HOOK(0x00452F10, GetEmbeddedBlockAt74, "auto");
