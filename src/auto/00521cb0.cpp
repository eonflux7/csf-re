#include "hook.h"
#include "globals.h"

// Callee: GetGlobalState, __cdecl, no arguments, returns the global state pointer.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

// Stdcall dispatcher: if the object's predicate (slot 23) holds, forwards the first
// argument to slot 20; otherwise forwards the global state's +0xA0 value to slot 20.
static int __stdcall SoundManager_DispatchWithGlobalFallback(int a1, int a2) {
    void* obj = reinterpret_cast<void*>(a2);

    bool predicateHolds = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[0x5C / 4])(obj, nullptr);
    if (predicateHolds) {
        return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[0x50 / 4])(obj, nullptr, a1);
    }

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    int fallbackValue = CSF_AT(int, globalState, 0xA0);
    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[0x50 / 4])(obj, nullptr, fallbackValue);
}

HOOK(0x521CB0, SoundManager_DispatchWithGlobalFallback, "auto");
