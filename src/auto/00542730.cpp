#include <cstdint>
#include "hook.h"
#include "globals.h"

// int __cdecl sub_73C020()
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x73C020, ());

// int __thiscall sub_73C030(void* this, int a2)
// Unknown callee: forwards the caller's value to the object returned by GetGlobalDword_73C020.
CSF_FUNC(int, __fastcall, GlobalDword_ApplyValue, 0x73C030, (void* self, void* edx, int value));

// 0x00542730 (23 bytes, ret 4): fetch the global object and hand it the caller's value.
static int __stdcall MenuMpCharacterSelect_DispatchValueToGlobal(int value)
{
    int global = GetGlobalDword_73C020();
    return GlobalDword_ApplyValue(reinterpret_cast<void*>(global), nullptr, value);
}

HOOK(0x542730, MenuMpCharacterSelect_DispatchValueToGlobal, "auto");
