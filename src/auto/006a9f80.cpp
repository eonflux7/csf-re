#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original vtable reference: the first call in this function is to sub_6A9BE0 with
// the S_006a9be0 vtable address in the original's ctor/dtor chain. No store is emitted here.
[[maybe_unused]] static const void* const kS006a9be0VtblRef = CSF_VTBL(0x006A9BE0);

CSF_FUNC(void, __fastcall, sub_6A9BE0, 0x006A9BE0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, sub_6508A0, 0x006508A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_652810, 0x00652810, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, NetSession_GetNodeAllocator, 0x00646FF0, ());
CSF_FUNC(void, __cdecl, NetSession_FreeListNode, 0x006470E0, (int a1, int a2, int a3));

// Releases the two node handles held by this object back to the NetSession node allocator.
static void __fastcall S_006a9be0_ReleaseNodes(S_006a9be0* self, void* /*edx*/)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    sub_6A9BE0(self, nullptr);

    // First sub-object at +0x18: release its handle at +0x1C.
    sub_6508A0(base + 0x18, nullptr);
    uint32_t firstNode = CSF_AT(uint32_t, base, 0x1C);
    uint32_t firstAllocator = reinterpret_cast<uintptr_t>(NetSession_GetNodeAllocator());
    NetSession_FreeListNode(static_cast<int>(firstAllocator), static_cast<int>(firstNode), 7);

    // Second sub-object at +0x8: release its handle at +0xC.
    CSF_AT(uint32_t, base, 0x1C) = 0;
    sub_652810(base + 0x8, nullptr);
    uint32_t secondNode = CSF_AT(uint32_t, base, 0xC);
    uint32_t secondAllocator = reinterpret_cast<uintptr_t>(NetSession_GetNodeAllocator());
    NetSession_FreeListNode(static_cast<int>(secondAllocator), static_cast<int>(secondNode), 7);

    CSF_AT(uint32_t, base, 0xC) = 0;
}

HOOK(0x006A9F80, S_006a9be0_ReleaseNodes, "auto");
