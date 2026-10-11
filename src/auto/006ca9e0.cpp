#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_6D5560, 0x006D5560, (void* self, void* edx, char* String2, int a3));

// Submits String2 to the cached object provider with a3 = -1 when a2 is set, +1 otherwise.
static int __stdcall GfxScene_SubmitClumpAddRemove(void* String2, char a2)
{
    int globalInt = GetGlobalInt();
    int accessor = GlobalInt_ResolveAccessor(reinterpret_cast<void*>(globalInt), nullptr);
    void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(accessor), nullptr);

    if (a2)
        return sub_6D5560(provider, nullptr, reinterpret_cast<char*>(String2), -1);
    else
        return sub_6D5560(provider, nullptr, reinterpret_cast<char*>(String2), 1);
}

HOOK(0x6CA9E0, GfxScene_SubmitClumpAddRemove, "auto");
