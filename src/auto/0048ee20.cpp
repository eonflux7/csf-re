#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static uint32_t* __fastcall C_00833908_CopyResolvedVector(
    C_00833908* self, void* /*edx*/, uint32_t* destination)
{
    void* inner = reinterpret_cast<void*>(self->vGetField016c());
    void* provider =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, inner, 0)[0x18 / 4])(inner, nullptr);
    uint32_t* source =
        reinterpret_cast<uint32_t* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, provider, 0)[0x10 / 4])(provider, nullptr);

    destination[0] = source[0];
    destination[1] = source[1];
    destination[2] = source[2];
    return destination;
}

HOOK(0x0048EE20, C_00833908_CopyResolvedVector, "auto");
