#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Allocates the sentinel (head) node of the waypoint graph's ordered container.
CSF_FUNC(uint32_t*, __cdecl, GraphNode_AllocateSentinel, 0x0075F000, ());

// Constructor of the waypoint graph container. Takes the vtable pointer as its
// single stack argument (ret 4), stores it in the first dword, resets the
// bookkeeping fields, then builds a self-linked sentinel node at +0x14 / +0x18.
// Returns this.
static void* __fastcall C_0086c288_Construct(C_0086c288* self, void* /*edx*/, int vtableAddress)
{
    CSF_AT(uint32_t, self, 0x00) = static_cast<uint32_t>(vtableAddress);
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 10;

    uint32_t* sentinel = GraphNode_AllocateSentinel();
    CSF_AT(uint32_t, self, 0x18) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(sentinel));

    uint8_t* node = reinterpret_cast<uint8_t*>(sentinel);
    uint32_t nodeAddress = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(node));

    CSF_AT(uint8_t, node, 0x31) = 1;
    CSF_AT(uint32_t, node, 4) = nodeAddress;
    CSF_AT(uint32_t, node, 0) = nodeAddress;
    CSF_AT(uint32_t, node, 8) = nodeAddress;

    CSF_AT(uint32_t, self, 0x1C) = 0;

    return reinterpret_cast<void*>(self);
}

HOOK(0x0075FBF0, C_0086c288_Construct, "auto");
