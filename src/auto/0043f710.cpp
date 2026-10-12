#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x0043F2A0: __thiscall lookup over the container embedded at this+4.
// Takes the arg address and value as two stack args (ret 8); returns the
// address of a slot holding the found node pointer.
CSF_FUNC(void**, __fastcall, Campaign_TreeLowerBoundSlot, 0x0043F2A0,
         (void* self, void* edx, void** a2, unsigned int* a3));

// 0x0043EFC0: __thiscall insert/rebalance helper for the same container (ret 8).
CSF_FUNC(int*, __fastcall, Campaign_TreeInsertAt, 0x0043EFC0,
         (uint32_t* a1, int a2, int* a3, int a4));

// 0x0043F710 (__thiscall, one stack arg, ret 4): finds the entry for a2 in the
// container at this+4. If the found node differs from the sentinel/end pointer
// stored at this+8, forwards to the insert helper and returns its result;
// otherwise returns the found node.
static int* __fastcall Campaign_FindOrInsertEntry(void* self, void* /*edx*/, int* a2)
{
    void* containerSelf = static_cast<uint8_t*>(self) + 4;

    void** foundSlot = Campaign_TreeLowerBoundSlot(containerSelf, nullptr,
                                                   reinterpret_cast<void**>(&a2),
                                                   reinterpret_cast<unsigned int*>(a2));
    int* result = reinterpret_cast<int*>(*foundSlot);

    if (result != CSF_AT(int*, self, 0x8))
    {
        // edx is not set by the original before this call; pass 0.
        return Campaign_TreeInsertAt(reinterpret_cast<uint32_t*>(containerSelf), 0,
                                     reinterpret_cast<int*>(&a2), reinterpret_cast<int>(result));
    }
    return result;
}

HOOK(0x0043F710, Campaign_FindOrInsertEntry, "auto");
