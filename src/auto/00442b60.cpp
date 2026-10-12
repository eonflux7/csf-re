#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveSkinObjectFromArg, 0x00681FC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));

static C_0085e760* __cdecl DynamicObj_DispatchSlot30ForObjectKind(int objectKind, char* arg)
{
    C_0085e760* result = reinterpret_cast<C_0085e760*>(objectKind);

    if (objectKind != 0) {
        if (objectKind <= 2 || objectKind > 6) {
            return result;
        }
        int globalValue = GetGlobalValue_4FBDE0();
        result = reinterpret_cast<C_0085e760*>(
            GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr));
    } else {
        int globalValue = GetGlobalValue_4FBDE0();
        result = reinterpret_cast<C_0085e760*>(
            ResolveSkinObjectFromArg(reinterpret_cast<void*>(globalValue), nullptr));
    }

    if (result) {
        result = reinterpret_cast<C_0085e760*>(
            ResolveEntryLookupValue(result, nullptr));
        if (result) {
            using Slot30Fn = C_0085e760* (__fastcall*)(void*, void*, char*);
            Slot30Fn slot30 = reinterpret_cast<Slot30Fn>(CSF_AT(void**, result, 0)[0x30 / 4]);
            return slot30(result, nullptr, arg);
        }
    }
    return result;
}

HOOK(0x00442B60, DynamicObj_DispatchSlot30ForObjectKind, "auto");
