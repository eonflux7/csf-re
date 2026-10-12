#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, MenuMpCharSelect_SetPreviewAndInit, 0x00542640, (int* self, void* /*edx*/, char a2, int a3));

// Referenced by the original's call target at 0x00542640 (packet lists it as its vtable).
// Only the address is bound here; no runtime access is generated from the disassembly.
static void* const s_MenuMpCharSelectVtblRef = CSF_VTBL(0x00542640);

static int __fastcall MpSpawnPoints_SetGameMode(void* self, void* /*edx*/, uint32_t a3)
{
    // Global state object; its field at +0x74 holds an object whose vtable slot 3 (+12) is invoked when non-null.
    uintptr_t globalStateAddr = static_cast<uintptr_t>(GetGlobalState_50D190());
    void* globalState = reinterpret_cast<void*>(globalStateAddr);
    void* pendingObj = CSF_AT(void*, globalState, 0x74);
    if (pendingObj) {
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, pendingObj, 0)[3])(pendingObj, nullptr);
    }

    // Preview character object at this+0x14C (332); initialize preview with mode 1 and a3.
    void* previewChar = CSF_AT(void*, self, 0x14C);
    int result = MenuMpCharSelect_SetPreviewAndInit(
        static_cast<int*>(previewChar), nullptr, 1, static_cast<int>(a3));

    // Flag byte at this+0x17E (382) set to 1.
    CSF_AT(uint8_t, self, 0x17E) = 1;

    return result;
}

HOOK(0x00536600, MpSpawnPoints_SetGameMode, "auto");
