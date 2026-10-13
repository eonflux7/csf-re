#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00835060_DispatchRecord, 0x004A8C30, (void* self, void* edx, int a2));

static uint32_t* __fastcall C_00835060_RefreshSkinTargetAndDispatch(
    C_00835060* self, void* /*edx*/, uint32_t* record)
{
    void* holder = reinterpret_cast<void*>(self->field_0014);
    C_0082dd78* skinOwner =
        reinterpret_cast<C_0082dd78*>(CSF_AT(uint32_t, holder, 0x4C));

    if (skinOwner)
    {
        // The virtual call has no stack arguments.
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, skinOwner, 0)[0x3C / 4])(skinOwner, nullptr);
    }

    if (*record != 44)
        C_00835060_DispatchRecord(
            self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(record)));

    return record;
}

HOOK(0x004A8AA0, C_00835060_RefreshSkinTargetAndDispatch, "auto");
