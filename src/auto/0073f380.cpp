#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, SessionDumpTeardown, 0x0073EEA0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_GLOBAL(uint32_t, g_dword_8BC098, 0x008BC098);

// Releases the global session dump object if it exists: runs its teardown routine,
// frees the block through the game heap, and clears the global pointer.
static int __cdecl ReleaseSessionDumpBuffer()
{
    if (g_dword_8BC098 == 0)
        return 0;

    void* dumpObject = reinterpret_cast<void*>(static_cast<uintptr_t>(g_dword_8BC098));
    SessionDumpTeardown(dumpObject, nullptr);
    int freeResult = Mem_Free(dumpObject);
    g_dword_8BC098 = 0;
    return freeResult;
}

HOOK(0x73F380, ReleaseSessionDumpBuffer, "auto");
