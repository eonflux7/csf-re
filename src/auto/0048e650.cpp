#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static hook::Tail __fastcall C_00833908_NotifyLinkedObjects(
    void* object, const uint32_t* /*args*/)
{
    auto* self = static_cast<C_00833908*>(object);
    int result = 0;

    void* linked = reinterpret_cast<void*>(self->field_004c);
    if (linked)
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, linked, 0)[0x34 / 4])(linked, nullptr);
    }

    linked = reinterpret_cast<void*>(self->field_0030);
    if (linked)
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, linked, 0)[0xa0 / 4])(linked, nullptr);
    }

    linked = reinterpret_cast<void*>(self->field_0034);
    if (linked)
        return hook::tail_jump(
            linked, (*reinterpret_cast<void***>(linked))[0xac / 4]);

    return hook::tail_ret(result);
}

HOOK_TAIL(0x0048E650, C_00833908_NotifyLinkedObjects, "auto");
