#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(char, __fastcall, sub_42FA10, 0x0042FA10, (void* self, void* edx, int arg0, int a3, int a4, int a5));

static uint8_t* AddressToPointer(uint32_t address) {
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(address));
}

// Address of the character data of a legacy string node: the inline buffer (node+4)
// while the capacity at node+0x18 is below 16, otherwise the heap pointer stored at node+4.
static uint32_t ResolveStringData(uint32_t node) {
    if (CSF_AT(uint32_t, AddressToPointer(node), 0x18) < 0x10u)
        return node + 4;
    return CSF_AT(uint32_t, AddressToPointer(node), 4);
}

static int __fastcall DispatchNamedEntryToGlobal(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t* a4) {
    // Resolve the four string arguments from the parameter block (a4[0..2]) and the source node (a4[3]).
    uint32_t sourceNode = CSF_AT(uint32_t, AddressToPointer(a4[3]), 4);
    uint32_t firstData = ResolveStringData(CSF_AT(uint32_t, AddressToPointer(a4[2]), 4) + 4);
    uint32_t secondNode = CSF_AT(uint32_t, AddressToPointer(a4[1]), 4);
    uint32_t secondData = ResolveStringData(secondNode + 4);
    uint32_t thirdData = ResolveStringData(CSF_AT(uint32_t, AddressToPointer(a4[0]), 4) + 4);
    uint32_t sourceValue = CSF_AT(uint32_t, AddressToPointer(sourceNode), 4);

    void* handler = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobal8afc78()));
    sub_42FA10(handler, nullptr,
               static_cast<int>(thirdData),
               static_cast<int>(secondData),
               static_cast<int>(firstData),
               static_cast<int>(sourceValue));

    CSF_AT(int32_t, self, 0x18) = 12;
    CSF_AT(uint8_t, self, 0x82) = 0;

    // The original re-reads a4[3] after the call.
    uint32_t returnedNode = CSF_AT(uint32_t, AddressToPointer(a4[3]), 4);
    CSF_AT(uint32_t, self, 0x78) = CSF_AT(uint32_t, AddressToPointer(returnedNode), 4);
    return static_cast<int>(returnedNode);
}

HOOK(0x00461800, DispatchNamedEntryToGlobal, "auto");
