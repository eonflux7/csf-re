#include <cstdint>
#include "hook.h"
#include "globals.h"

// int __cdecl GetGlobalValue_521BB0()
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x521BB0, ());

// void __thiscall sub_5231D0(int this, int a2)
// Unknown callee: applies the given value to the object returned by GetGlobalValue_521BB0.
CSF_FUNC(void, __fastcall, GlobalSetValue_5231D0, 0x5231D0, (int self, void* edx, int value));

// 0x00541940 (23 bytes, ret 4): fetch the global object and hand it the caller's value.
static void __stdcall MenuMpStats_DispatchValueToGlobal(int value)
{
    int global = GetGlobalValue_521BB0();
    GlobalSetValue_5231D0(global, nullptr, value);
}

HOOK(0x541940, MenuMpStats_DispatchValueToGlobal, "auto");
