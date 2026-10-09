#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(char, __fastcall, SceneControl_ApplyRecordByte, 0x0044F8C0, (void* self, void* edx, char a2));

// Walks **a3 -> +4 -> +1 to fetch a record byte and forwards it, with the current scene, to SceneControl_ApplyRecordByte.
static char __stdcall ScriptCommands_ForwardSceneRecordByteToControl(int a1, int a2, int a3)
{
    (void)a1;
    (void)a2;

    uintptr_t argRecord = *reinterpret_cast<uintptr_t*>(static_cast<uintptr_t>(a3));
    uintptr_t chainNode = *reinterpret_cast<uintptr_t*>(argRecord + 4);
    void* currentScene = g_pCurrentScene;
    char recordByte = *reinterpret_cast<char*>(chainNode + 1);

    return SceneControl_ApplyRecordByte(currentScene, nullptr, recordByte);
}

HOOK(0x45BA40, ScriptCommands_ForwardSceneRecordByteToControl, "auto");
