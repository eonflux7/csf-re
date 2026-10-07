#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

using ReleaseMutexFunction = int (__stdcall*)(void*);
CSF_GLOBAL(ReleaseMutexFunction, g_ReleaseMutex, 0x82D23C);

static int __fastcall C_0085eb30_ReleaseMutexIfSet(
    C_0085eb30* self, void* /*edx*/)
{
    void* mutexHandle = reinterpret_cast<void*>(self->field_0004);
    if (mutexHandle)
        return g_ReleaseMutex(mutexHandle);
    return 0;
}

HOOK(0x69E3D0, C_0085eb30_ReleaseMutexIfSet, "auto");
