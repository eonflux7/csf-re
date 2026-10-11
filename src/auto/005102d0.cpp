#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, sub_50E9D0, 0x0050E9D0, ());
CSF_FUNC(uint32_t*, __fastcall, sub_51B480, 0x0051B480, (void* self, void* edx, int a2));

// Constructs the weapon database: runs the base-class init, installs this class's
// vtable, then creates a self-linked sentinel node for the record list.
static uint32_t* __fastcall C_00840680_Construct(C_00840680* self, void* /*edx*/, int a2)
{
    // Base-class initialization (CWeaponDefTable part of the object), a2 forwarded.
    sub_51B480(self, nullptr, a2);

    // Install this class's vtable at offset 0.
    CSF_AT(void*, self, 0x0) = CSF_VTBL(0x00840680);

    // Allocate the list sentinel node.
    int sentinelValue = sub_50E9D0();
    uint8_t* sentinelNode = reinterpret_cast<uint8_t*>(static_cast<intptr_t>(sentinelValue));
    uint32_t sentinelAddr = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(sentinelNode));

    // List head pointer lives at self+0x30.
    CSF_AT(uint8_t*, self, 0x30) = sentinelNode;
    CSF_AT(uint8_t, sentinelNode, 0x11) = 1;

    // Self-link: next, prev and the third slot all point at the sentinel itself.
    CSF_AT(uint32_t, sentinelNode, 0x4) = sentinelAddr;
    CSF_AT(uint32_t, sentinelNode, 0x0) = sentinelAddr;
    CSF_AT(uint32_t, sentinelNode, 0x8) = sentinelAddr;

    // Count / auxiliary field at self+0x34 cleared.
    CSF_AT(uint32_t, self, 0x34) = 0;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x005102D0, C_00840680_Construct, "auto");
