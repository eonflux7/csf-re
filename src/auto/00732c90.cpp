#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static bool __fastcall C_008670f0_ExceedsParentBounds(
    C_008670f0* self, void* /*edx*/)
{
    void* parent = reinterpret_cast<void*>(self->field_0004);
    int limit = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, parent, 0)[0x2C / 4])(parent, nullptr);

    if (static_cast<int32_t>(self->field_0098) > limit)
        return true;
    if ((self->field_00a0 & 0x80000000) != 0)
        return true;

    parent = reinterpret_cast<void*>(self->field_0004);
    limit = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, parent, 0)[0x30 / 4])(parent, nullptr);

    if (static_cast<int32_t>(self->field_009c) > limit)
        return true;
    return (self->field_00a4 & 0x80000000) != 0;
}

HOOK(0x00732C90, C_008670f0_ExceedsParentBounds, "auto");
