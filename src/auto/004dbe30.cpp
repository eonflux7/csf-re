#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiState_PrepareSlot, 0x004DBCF0, (void* self, void* edx));

static int __fastcall C_0083dfa0_PrepareSlotAndDispatch(
    C_0083dfa0* self, void* /*edx*/, int value)
{
    using DispatchFn = int(__fastcall*)(void*, void*, C_00833908**, int);

    auto* slot = reinterpret_cast<C_00833908**>(
        reinterpret_cast<uint8_t*>(self) + 8);
    AiState_PrepareSlot(slot, nullptr);

    if (self) {
        // field_0004 is the embedded subobject's vtable pointer.
        auto* table = reinterpret_cast<void**>(self->field_0004);
        auto dispatch = reinterpret_cast<DispatchFn>(table[0x2C / 4]);
        return dispatch(&self->field_0004, nullptr, slot, value);
    }

    void* nullObject = nullptr;
    auto dispatch = reinterpret_cast<DispatchFn>(
        CSF_AT(void**, nullObject, 0)[0x2C / 4]);
    return dispatch(nullObject, nullptr, slot, value);
}

HOOK(0x004DBE30, C_0083dfa0_PrepareSlotAndDispatch, "auto");
