#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x00756A10, __cdecl, no args; returns the global value block pointer.
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// Original: 0x00536940, __thiscall; callee pops 8 bytes (float a2, char a3).
CSF_FUNC(int, __fastcall, sub_536940, 0x00536940, (void* self, void* edx, float a2, char a3));

static int __fastcall MenuControls_ApplyGlobalFloatToScene(S_00536940* self, void* /*edx*/)
{
    // Referenced by the original's call site (vtable of S_00536940).
    [[maybe_unused]] const void* const sceneVtable = CSF_VTBL(0x00536940);

    int globalValueBlock = GetGlobalValue_8BC1E0();
    float speedValue = CSF_AT(float, reinterpret_cast<void*>(static_cast<uintptr_t>(globalValueBlock)), 0x2264);
    return sub_536940(self, nullptr, speedValue, 0);
}

HOOK(0x536B70, MenuControls_ApplyGlobalFloatToScene, "auto");
