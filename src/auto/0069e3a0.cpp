#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// stdcall WaitForSingleObject(HANDLE, DWORD) reached through the import slot at 0x82D20C.
using WaitForSingleObjectFn = uint32_t(__stdcall*)(void*, uint32_t);
CSF_GLOBAL(WaitForSingleObjectFn, g_pfnWaitForSingleObject, 0x0082D20C);

// Returns true when the event handle is absent or the wait reports WAIT_OBJECT_0 (0).
static bool __fastcall C_0085eb30_WaitForEventHandle(C_0085eb30* self, void* /*edx*/, uint32_t dwMilliseconds)
{
    void* handle = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0004));
    if (!handle)
        return true;

    uint32_t timeoutMilliseconds = dwMilliseconds;
    if (!dwMilliseconds)
        timeoutMilliseconds = 0xFFFFFFFF;

    return g_pfnWaitForSingleObject(handle, timeoutMilliseconds) == 0;
}

HOOK(0x0069E3A0, C_0085eb30_WaitForEventHandle, "auto");
