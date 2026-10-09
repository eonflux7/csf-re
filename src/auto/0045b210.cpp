#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobal8B0558, 0x004F4920, ());
CSF_FUNC(int, __fastcall, sub_4F4970, 0x004F4970, (void* self, void* edx, int a2, int a3));

// Reads the two stack-passed values (v4 from a3[0], v5 from a3[1]), each taken
// through the chain obj -> obj+4 -> obj+4, then dispatches to sub_4F4970.
static int __stdcall DispatchNestedKeyPair(int a1, int a2, uint32_t* a3)
{
    uint32_t* firstEntry = reinterpret_cast<uint32_t*>(a3[1]);
    uint32_t* firstInner = reinterpret_cast<uint32_t*>(firstEntry[1]);
    uint32_t v5 = firstInner[1];

    uint32_t* secondEntry = reinterpret_cast<uint32_t*>(a3[0]);
    uint32_t* secondInner = reinterpret_cast<uint32_t*>(secondEntry[1]);
    uint32_t v4 = secondInner[1];

    void* resolved = reinterpret_cast<void*>(GetGlobal8B0558());
    return sub_4F4970(resolved, nullptr, static_cast<int>(v4), static_cast<int>(v5));
}

HOOK(0x0045B210, DispatchNestedKeyPair, "auto");
