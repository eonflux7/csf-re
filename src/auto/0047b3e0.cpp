#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CharTaskBase_Prepare, 0x004DB110, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_DecrementCounterAt30, 0x0048CAC0, (void* self, void* , int));
CSF_FUNC(char, __fastcall, CounterSlot_Reset, 0x0048C9D0, (void* self, void* edx, char a2));

// Reads the owner task stored in this->field_0008 (re-read at each use, as the original does).
static uint8_t* TFunctionRecargar_OwnerTask(TFunctionRecargar* self)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0008));
}

// Prepares the task, releases one counter slot, resets the counter slot,
// then forwards to the owner's virtual slot 15 (offset 0x3C) with this task as argument.
static int __fastcall TFunctionRecargar_StartReload(TFunctionRecargar* self, void* /*edx*/)
{
    CharTaskBase_Prepare(self, nullptr);

    uint8_t* owner = TFunctionRecargar_OwnerTask(self);
    uint32_t ctx = CSF_AT(uint32_t, owner, 0x14);
    uint32_t counter = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(ctx)), 0x204);
    C_DecrementCounterAt30(reinterpret_cast<void*>(static_cast<uintptr_t>(counter)), nullptr,
                           static_cast<int>(reinterpret_cast<uintptr_t>(self)));

    owner = TFunctionRecargar_OwnerTask(self);
    ctx = CSF_AT(uint32_t, owner, 0x14);
    counter = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(ctx)), 0x204);
    CounterSlot_Reset(reinterpret_cast<void*>(static_cast<uintptr_t>(counter)), nullptr, static_cast<char>(-1));

    owner = TFunctionRecargar_OwnerTask(self);
    ctx = CSF_AT(uint32_t, owner, 0x14);
    uint32_t target = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(ctx)), 0x48);
    void* targetObj = reinterpret_cast<void*>(static_cast<uintptr_t>(target));
    uint32_t taskArg = self->field_0008;

    return ((int(__fastcall*)(void*, void*, uint32_t))(CSF_AT(void**, targetObj, 0)[15]))(
        targetObj, nullptr, taskArg);
}

HOOK(0x0047B3E0, TFunctionRecargar_StartReload, "auto");
