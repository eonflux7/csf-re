#include <cstdint>
#include "hook.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, GetGlobalState, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, MpCharacterSelect_ResolveSpawnIndex, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __cdecl, MpCharacterSelect_ApplySpawnIndex, 0x005500F0, (int spawnIndex));
CSF_FUNC(int, __cdecl, MpCharacterSelect_RefreshPreviewCamera, 0x00555DD0, (int previewObject));

// __thiscall(this, a2, a3), callee pops 8 bytes.
static int __fastcall MenuMpCharacterSelect_RefreshCameraPreview(void* self, void* /*edx*/, int /*a2*/, int /*a3*/)
{
    int globalState = GetGlobalState();
    int result = globalState;

    void* stateObject = reinterpret_cast<void*>(static_cast<uintptr_t>(globalState));
    if (CSF_AT(uint8_t, stateObject, 0xD0))
    {
        int globalIndex = GetGlobalInt();
        int spawnIndex = MpCharacterSelect_ResolveSpawnIndex(
            reinterpret_cast<void*>(static_cast<uintptr_t>(globalIndex)), nullptr);
        MpCharacterSelect_ApplySpawnIndex(spawnIndex);

        uint32_t ownerPointer = CSF_AT(uint32_t, self, 0);
        void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(ownerPointer));
        result = MpCharacterSelect_RefreshPreviewCamera(
            static_cast<int>(CSF_AT(uint32_t, owner, 24)));

        CSF_AT(uint8_t, self, 0xA) = 0;
        CSF_AT(uint32_t, self, 0xC) = 0xBF800000u;
    }
    return result;
}

HOOK(0x00542510, MenuMpCharacterSelect_RefreshCameraPreview, "auto");
