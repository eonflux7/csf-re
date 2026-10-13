#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

// Resets the linked order's task state (when the owner's state reports not-ready or
// the state's result flags lack bit 1), refreshes the cached provider, and returns
// the linked object's field. Returns the state's result pointer when it is valid.
static uint32_t* __stdcall AiOrder_UpdateLinkedOrderState(C_0085e760* object, int unusedSecondArg)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(object, nullptr);

    uint8_t* linkBase = nullptr;
    if (linkedValue)
        linkBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedValue - 4));

    void* ownerObject = CSF_AT(void*, linkBase, 0x2C);
    void* stateObject = CSF_AT(void*, ownerObject, 0x30);

    bool stateReady = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, stateObject, 0)[0xB8 / 4])(stateObject, nullptr);

    uint32_t* result = nullptr;
    if (!stateReady) {
        result = ((uint32_t * (__fastcall*)(void*, void*, void*))CSF_AT(void**, stateObject, 0)[0xBC / 4])(
            stateObject, nullptr, &object);
        if ((*result & 2) != 0)
            return result;
    }

    int stateField10 = GetLinkedObjectField10(object, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(object, nullptr, stateField10 + 4);

    int entryValue = ResolveEntryLookupValue(object, nullptr);
    auto* cacheEntry = reinterpret_cast<C_0085e7b8*>(static_cast<uintptr_t>(entryValue));
    GetCachedObjectProvider(cacheEntry, nullptr);

    return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(GetLinkedObjectField10(object, nullptr)));
}

HOOK(0x4ACF80, AiOrder_UpdateLinkedOrderState, "auto");
