#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// 0x44ACE0 reads its single argument from the stack and does not pop it (ret 0),
// so it is bound as __cdecl. The caller side does not clean up here either.
CSF_FUNC(int, __cdecl, GetGlobalValue_44ACE0, 0x0044ACE0, (int value));
// 0x44BAA0 takes the object in ecx and one stack argument, and pops that argument (ret 4).
CSF_FUNC(uint32_t*, __fastcall, sub_44BAA0, 0x0044BAA0, (void* self, void* edx, int a2));

static int __stdcall ResolveHandleFieldEntry(int a1, int a2, int a3)
{
    // ecx = *(a3); v4 = *(ecx + 4) + 4
    void* holder = reinterpret_cast<void*>(a3);
    void* object = *reinterpret_cast<void**>(holder);
    int offsetValue = CSF_AT(int, object, 4) + 4;

    // The original pushes v4 once, calls GetGlobalValue (which leaves it on the stack),
    // then calls sub_44BAA0, which pops it. Passing it again as the stack argument
    // gives the same arguments to both callees.
    int globalValue = GetGlobalValue_44ACE0(offsetValue);
    uint32_t* entry = sub_44BAA0(reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)),
                                 nullptr, offsetValue);

    return static_cast<int>(reinterpret_cast<uintptr_t>(entry));
}

HOOK(0x45B270, ResolveHandleFieldEntry, "auto");
