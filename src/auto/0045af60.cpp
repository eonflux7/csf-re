#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(bool, __fastcall, sub_4687B0, 0x004687B0, (void* self, void* edx, int a2));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static __declspec(noinline) bool __stdcall EvaluateSubObjectQueryAndStoreFlag(void* a1, void* a2, void* a3)
{
    // Target record pointer taken from the first argument (+4).
    uint32_t targetAddress = CSF_AT(uint32_t, a1, 0x4);
    void* target = reinterpret_cast<void*>(targetAddress);

    // Argument chain: a3 -> *a3 -> (+4) -> (+4) value.
    void* argPtr = CSF_AT(void*, a3, 0);
    void* argNode = CSF_AT(void*, argPtr, 0x4);
    void* queryArg = CSF_AT(void*, argNode, 0x4);

    // Scene object's sub-object at +0xBC is the 'this' for the query.
    void* scene = g_pCurrentScene;
    void* self = CSF_AT(void*, scene, 0xBC);

    bool result = sub_4687B0(self, nullptr, reinterpret_cast<int>(queryArg));
    CSF_AT(uint8_t, target, 0x1) = result ? 1 : 0;
    return result;
}

HOOK(0x0045AF60, EvaluateSubObjectQueryAndStoreFlag, "auto");
