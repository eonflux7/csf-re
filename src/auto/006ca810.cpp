#include <cstdint>
#include "globals.h"
#include "hook.h"

// Calls the object's vtable slot 4 (byte offset 0x10) with the object as `this`.
// If the returned code is 0, 3 or 6, the controller is the sub-object at +0x20; otherwise null.
static int FxParticleManager_GetParticleController(int a1)
{
    void* particleManager = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));

    int stateCode = ((int(__fastcall*)(void*, void*))CSF_AT(void**, particleManager, 0)[4])(particleManager, nullptr);

    if (stateCode == 0 || stateCode == 3 || stateCode == 6)
        return a1 + 32;

    return 0;
}

HOOK(0x6CA810, FxParticleManager_GetParticleController, "auto");
