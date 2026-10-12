#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original: Mem_Alloc (__cdecl), returns a 52-byte block for a vertex record.
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
// Original: sub_6C4490 (__thiscall, ecx=this), initializes the newly allocated record.
CSF_FUNC(int, __fastcall, sub_6C4490, 0x006C4490, (void* self, void* edx));
// Original: sub_6C35C0 (__thiscall, ret 8), called with (10, 10) after the record is stored.
CSF_FUNC(int, __fastcall, sub_6C35C0, 0x006C35C0, (void* self, void* edx, int a2, int a3));

// Returns the vertex record stored at this+0xCC, creating it on first use.
static int __fastcall GfxObjectInstances_GetVertexRecord(void* self, void* /*edx*/)
{
    if (CSF_AT(int, self, 0xCC) == 0) {
        void* allocated = Mem_Alloc(0x34);
        int recordHandle = 0;
        if (allocated != nullptr) {
            recordHandle = sub_6C4490(allocated, nullptr);
        }
        CSF_AT(int, self, 0xCC) = recordHandle;
        sub_6C35C0(reinterpret_cast<void*>(static_cast<std::uintptr_t>(recordHandle)), nullptr, 10, 10);
    }
    return CSF_AT(int, self, 0xCC);
}

HOOK(0x006D9D40, GfxObjectInstances_GetVertexRecord, "auto");
