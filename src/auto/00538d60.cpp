#include <cstdint>
#include "globals.h"
#include "hook.h"

// int __cdecl sub_50D190()  -- returns the pointer to the global game state.
CSF_FUNC(int, __cdecl, GetGlobalState, 0x50D190, ());

// int __userpurge sub_538A50(int* a1@<ecx>, int a2@<esi>, float a3);  ret 4 (pops the float)
CSF_FUNC(int, __fastcall, CMenuLoading_SetupHalfParameter, 0x538A50, (int* a1, void* a2, float a3));

// 0x538D60: __usercall (a1@<ecx>, a2@<esi>), pops nothing.
// Applies a 0.5 half-parameter to the owning object (located 16 bytes before `self`),
// then dispatches virtual slot 3 (with argument 0) on the global state's inner object.
static int __fastcall CMenuLoading_ApplyHalfProgress(void* self, void* a2)
{
    // ecx = self - 0x10, esi = a2, push 0.5f
    CMenuLoading_SetupHalfParameter(
        reinterpret_cast<int*>(static_cast<uint8_t*>(self) - 0x10),
        a2,
        0.5f);

    int globalState = GetGlobalState();                                  // call 0x50D190
    int innerValue  = CSF_AT(int, reinterpret_cast<void*>(globalState), 0x10); // [eax+0x10]
    void* inner  = reinterpret_cast<void*>(innerValue);
    void** vtable = *reinterpret_cast<void***>(inner);                   // [ecx]
    return ((int (__fastcall*)(void*, void*, int))vtable[3])(inner, nullptr, 0); // call [eax+0xc]
}

HOOK(0x538D60, CMenuLoading_ApplyHalfProgress, "auto");
