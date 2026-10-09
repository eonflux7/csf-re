#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x0043E010: cdecl, no arguments, returns an int handle used as `self` below.
CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());

// 0x0043F6A0: __fastcall (self in ecx), one stack argument, `ret 4`. Returns a float value.
CSF_FUNC(double, __fastcall, EvaluateFloatForSubobject, 0x0043F6A0, (void* self, void* edx, int* a2));

// 0x00461720: __stdcall, ret 12. Computes a float from a global handle and a nested pointer chain,
// then stores it at +4 of the object referenced by a1+4.
static void __stdcall StoreComputedFloatInSubobject(int a1, int a2, int a3)
{
    // Object whose +4 field receives the result.
    uint32_t targetObject = *CSF_AT(uint32_t*, a1, 0x4);

    // Argument chain: *a3 -> p; p+4 -> q; argument = q+4.
    uint32_t firstLevel = *(uint32_t*)a3;
    uint32_t secondLevel = *CSF_AT(uint32_t*, firstLevel, 0x4);
    int* chainArg = (int*)(uintptr_t)(secondLevel + 4);

    GetGlobal8afc7c();
    int globalHandle = GetGlobal8afc7c();
    double computed = EvaluateFloatForSubobject((void*)(intptr_t)globalHandle, nullptr, chainArg);

    *CSF_AT(float*, targetObject, 0x4) = (float)computed;
}

HOOK(0x00461720, StoreComputedFloatInSubobject, "auto");
