#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static bool __fastcall C_00833908_IsLinkedObjectActiveAndUnblocked(
    C_00833908* self, void* /*edx*/)
{
    void* linkedObject = reinterpret_cast<void*>(self->field_0048);
    if (!linkedObject)
        return false;

    if (!reinterpret_cast<bool (__fastcall*)(void*, void*)>(
            CSF_AT(void**, linkedObject, 0)[0x34 / 4])(linkedObject, nullptr))
        return false;

    return CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x1) == 0;
}

HOOK(0x0048F4C0, C_00833908_IsLinkedObjectActiveAndUnblocked, "auto");
