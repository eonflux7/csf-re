#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Callees, bound by address.
CSF_FUNC(int, __fastcall, sub_755E90, 0x00755E90, (int a1, int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));

// Constructor for C_00869cd0: sets vtable, initializes embedded sub-object at +8,
// allocates a 16-byte zeroed block stored at +4, assigns "_Etereo_" into the
// sub-object at +0xC, and clears the field at +0x28.
static char* __fastcall C_00869cd0_Construct(char* self, void* /*edx*/)
{
    // Vtable store (base-class/derived vtable for C_00869cd0).
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00869CD0);

    // Initialize the embedded sub-object at self+8.
    sub_755E90(static_cast<int>(reinterpret_cast<std::uintptr_t>(self + 8)), 0);

    // Allocate a 16-byte block from the game heap and zero its three dwords after the first.
    void* block = Mem_Alloc(16);
    if (block != nullptr) {
        CSF_AT(int, block, 4) = 0;
        CSF_AT(int, block, 8) = 0;
        CSF_AT(int, block, 0xC) = 0;
    } else {
        block = nullptr;
    }
    CSF_AT(void*, self, 4) = block;

    // Assign the "_Etereo_" string (8 chars) into the sub-object at self+0xC.
    // The original passes a non-const pointer to the constant; cast away const to match the callee's signature.
    AssignStringRange_429B60(self + 0xC, nullptr, const_cast<char*>(CSF_STR(0x00869CC4, "_Etereo_")), 8);

    CSF_AT(int, self, 0x28) = 0;
    return self;
}

HOOK(0x007562D0, C_00869cd0_Construct, "auto");
