#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AccessorState_ResolveSlot, 0x006C8EE0, (void* self, void* edx, int a2));

static int __fastcall C_008316d8_ApplyEntryState(
    C_008316d8* self, void* /*edx*/, int /*unused*/)
{
    const int entryValue = self->vResolveEntryLookupValue();
    if (!entryValue)
        return entryValue;

    void* entry = reinterpret_cast<void*>(entryValue);
    void* global = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(global, nullptr));

    void* state = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, accessor, 0)[0x14 / 4])(accessor, nullptr);

    const int slot = AccessorState_ResolveSlot(
        state, nullptr, static_cast<int>(self->field_0044));

    alignas(4) uint8_t parameters[0x20] = {};
    CSF_AT(float, parameters, 0x00) = self->field_0048;
    CSF_AT(uint32_t, parameters, 0x04) = 0;
    CSF_AT(uint32_t, parameters, 0x08) = 0;
    CSF_AT(uint8_t, parameters, 0x0C) = 0;
    CSF_AT(uint8_t, parameters, 0x0D) = 0;
    CSF_AT(uint8_t, parameters, 0x0E) = 0;
    CSF_AT(float, parameters, 0x10) = 1.0f;
    CSF_AT(float, parameters, 0x14) = 1.0f;
    CSF_AT(float, parameters, 0x18) = 1.0f;
    CSF_AT(uint8_t, parameters, 0x1C) = 0;
    CSF_AT(uint8_t, parameters, 0x1D) = 1;

    void* receiver = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, entry, 0)[0x68 / 4])(entry, nullptr);

    return ((int(__fastcall*)(void*, void*, int, int, void*))
        CSF_AT(void**, receiver, 0)[0x14 / 4])(
            receiver, nullptr, 0, slot, parameters);
}

HOOK(0x00476390, C_008316d8_ApplyEntryState, "auto");
