#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global storage slot written by the original (0x008BAFD0).
CSF_GLOBAL(void*, g_storedWindowHandle, 0x8BAFD0);

// Stores the incoming window handle into the global slot and returns it unchanged.
static void* __cdecl StoreWindowHandle(void* hWnd)
{
    g_storedWindowHandle = hWnd;
    return hWnd;
}

HOOK(0x69CDE0, StoreWindowHandle, "auto");
