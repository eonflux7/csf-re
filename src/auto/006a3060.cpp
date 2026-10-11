#include "globals.h"
#include "hook.h"

// Callee bindings (by address)
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, LogWindow_ReleaseBuffer, 0x006A2F00, (void* self, void* edx));

// Log window reference counter and its heap-allocated state buffer
CSF_GLOBAL(int, g_LogWindowRefCount, 0x008BB410);
CSF_GLOBAL(void*, g_LogWindowBuffer, 0x008BB414);

// Drops one reference to the log window. When the last reference goes away and a
// state buffer exists, releases the buffer's internals, frees it, and clears the pointer.
static int __cdecl LogWindow_ReleaseRef() {
    --g_LogWindowRefCount;
    int result = g_LogWindowRefCount;
    if (g_LogWindowRefCount == 0) {
        if (g_LogWindowBuffer) {
            void* buffer = g_LogWindowBuffer;
            LogWindow_ReleaseBuffer(buffer, nullptr);
            Mem_Free(buffer);
            g_LogWindowBuffer = nullptr;
            return g_LogWindowRefCount;
        }
    }
    return result;
}

HOOK(0x006A3060, LogWindow_ReleaseRef, "auto");
