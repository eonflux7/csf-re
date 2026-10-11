#include "globals.h"
#include "hook.h"

// Stdcall CloseHandle pointer stored in the game's data section.
using CloseHandleFn = int(__stdcall*)(void* handle);
CSF_GLOBAL(CloseHandleFn, g_pfnCloseHandle, 0x82D0E0);

// Closes a thread handle through the stored CloseHandle pointer.
// Caller is cdecl (pops nothing); the stdcall callee pops its own argument.
static void __cdecl Audio_CloseThreadHandle(void* hObject) {
    g_pfnCloseHandle(hObject);
}

HOOK(0x70E010, Audio_CloseThreadHandle, "auto");
