#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_00833908_InitializeAndForward(
    C_00833908* self, void* /*edx*/, int initializationData, int argument)
{
    self->vSlot50(reinterpret_cast<int*>(initializationData));
    void* innerObject = reinterpret_cast<void*>(self->vGetField016c());
    void* destination = reinterpret_cast<void*>(
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, innerObject, 0)[0x18 / 4])(innerObject, nullptr));

    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, destination, 0)[0x4C / 4])(
            destination, nullptr, argument);
}

HOOK(0x0048E620, C_00833908_InitializeAndForward, "auto");
