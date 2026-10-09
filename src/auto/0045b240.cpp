#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobal8B0558, 0x004F4920, ());
CSF_FUNC(int, __fastcall, ProcessGlobalKeyPair, 0x004F4990, (void* self, void* edx, int a2, int a3));

// Reads the two stack-passed values (v4 from a3[0], v5 from a3[1]), each taken
// through the chain obj -> obj+4 -> obj+4, then forwards them to ProcessGlobalKeyPair
// with the object returned by GetGlobal8B0558 as `this`.
static int __stdcall DispatchNestedKeyPairWithGlobal(int a1, int a2, uint32_t* a3)
{
    uint32_t* firstEntry = reinterpret_cast<uint32_t*>(a3[1]);
    uint32_t* firstInner = reinterpret_cast<uint32_t*>(firstEntry[1]);
    uint32_t v5 = firstInner[1];

    uint32_t* secondEntry = reinterpret_cast<uint32_t*>(a3[0]);
    uint32_t* secondInner = reinterpret_cast<uint32_t*>(secondEntry[1]);
    uint32_t v4 = secondInner[1];

    void* resolved = reinterpret_cast<void*>(GetGlobal8B0558());
    return ProcessGlobalKeyPair(resolved, nullptr, static_cast<int>(v4), static_cast<int>(v5));
}

HOOK(0x0045B240, DispatchNestedKeyPairWithGlobal, "auto");
