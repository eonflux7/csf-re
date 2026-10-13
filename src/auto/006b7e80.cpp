#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Bind of the tree locate/insert helper (__fastcall: ecx=self, edx=a2, 3 stack args -> ret 12).
CSF_FUNC(uint32_t*, __fastcall, MapTree_LocateOrInsertNode, 0x006B7D20,
         (int a1, int a2, uint32_t* a3, uint32_t* a4, uint32_t* a5));

// Looks up the value slot for the key *keyPtr in a balanced map tree whose header is self.
// Returns a pointer to the slot's payload (node + 16 bytes), creating the node if absent.
static int __fastcall MapTree_LocateValueSlot(uint32_t* self, void* /*edx*/, int* keyPtr) {
    // header = *(this + 1); candidate = header[1] (root)
    uint32_t* header = reinterpret_cast<uint32_t*>(self[1]);
    uint32_t* node = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, header, 4));

    // Descend until the nil sentinel (byte +0x15 set).
    while (CSF_AT(uint8_t, node, 0x15) == 0) {
        if (CSF_AT(uint32_t, node, 0xC) >= static_cast<uint32_t>(*keyPtr)) {
            header = node;
            node = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, node, 0));
        } else {
            node = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, node, 8));
        }
    }

    // Found an exact-or-lower match inside the tree.
    if (header != reinterpret_cast<uint32_t*>(self[1]) &&
        static_cast<uint32_t>(*keyPtr) >= CSF_AT(uint32_t, header, 0xC)) {
        return static_cast<int>(reinterpret_cast<uintptr_t>(header + 4));
    }

    // Not found: build the key pair on the stack and let the helper insert it.
    uint32_t keyPair[2];
    keyPair[0] = static_cast<uint32_t>(*keyPtr);
    keyPair[1] = 0;

    uint32_t* paramSlot = reinterpret_cast<uint32_t*>(&keyPtr);
    uint32_t* slot = MapTree_LocateOrInsertNode(
        static_cast<int>(reinterpret_cast<uintptr_t>(self)),
        static_cast<int>(reinterpret_cast<uintptr_t>(&keyPtr)),
        paramSlot,
        header,
        keyPair);
    return static_cast<int>(*slot + 16);
}

HOOK(0x006B7E80, MapTree_LocateValueSlot, "auto");
