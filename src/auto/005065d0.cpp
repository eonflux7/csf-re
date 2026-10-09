#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

using SceneLoaderThreadProc = uint32_t (__stdcall*)(void*);
using CreateThreadImportFn = void* (__stdcall*)(
    void* securityAttributes,
    uint32_t stackSize,
    SceneLoaderThreadProc startRoutine,
    void* parameter,
    uint32_t creationFlags,
    uint32_t* threadId);

CSF_GLOBAL(CreateThreadImportFn, g_CreateThreadImport, 0x0082D1A0);

static void* __fastcall CSceneLoader_StartBackgroundThread(C_0083f5d8* self, void* /*edx*/)
{
    void* threadHandle = g_CreateThreadImport(
        nullptr,
        0,
        reinterpret_cast<SceneLoaderThreadProc>(CSF_ADDR_FN(0x005065C0)),
        self,
        0,
        &CSF_AT(uint32_t, self, 0x114));

    CSF_AT(uint32_t, self, 0x110) =
        static_cast<uint32_t>(reinterpret_cast<uintptr_t>(threadHandle));
    CSF_AT(uint8_t, self, 0xB5) = 1;
    return threadHandle;
}

HOOK(0x005065D0, CSceneLoader_StartBackgroundThread, "auto");
