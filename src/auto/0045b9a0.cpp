#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// 0x43E010 takes no arguments and is __cdecl (ret 0). The caller's pushed stack value is left in place.
CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
// 0x43F710 takes the object in ecx and one stack argument, and pops that argument (ret 4).
CSF_FUNC(int*, __fastcall, sub_43F710, 0x0043F710, (void* self, void* edx, int* a2));

static int __stdcall ResolveNestedFieldEntry(int a1, int a2, int a3)
{
    // ecx = *(a3); v4 = *(ecx + 4) + 4
    void** holder = reinterpret_cast<void**>(static_cast<uintptr_t>(a3));
    void* object = *holder;
    uint32_t fieldAddress = CSF_AT(uint32_t, object, 4) + 4;
    int* v4 = reinterpret_cast<int*>(static_cast<uintptr_t>(fieldAddress));

    // The original pushes v4 once, calls GetGlobal8afc7c (which leaves it on the stack),
    // then passes the global's result in ecx and calls sub_43F710, which pops v4.
    int globalValue = GetGlobal8afc7c();
    int* entry = sub_43F710(reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)),
                            nullptr, v4);

    return static_cast<int>(reinterpret_cast<uintptr_t>(entry));
}

HOOK(0x45B9A0, ResolveNestedFieldEntry, "auto");
