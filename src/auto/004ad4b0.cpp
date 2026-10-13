#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(char, __fastcall, C_0082f9f8_QueueAction, 0x004A8710, (C_0082f9f8* self, void* , int a2));

static void* QueuePointerFromAddress(uint32_t address)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(address));
}

static int __stdcall QueueLinkedObjectAction(int linkedObject)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(static_cast<uintptr_t>(linkedObject)), nullptr);
    uint32_t ownerBase = linkedValue ? static_cast<uint32_t>(linkedValue - 4) : 0u;

    uint32_t ownerPart = CSF_AT(uint32_t, QueuePointerFromAddress(ownerBase), 0x2C);
    uint32_t ownerSubPart = CSF_AT(uint32_t, QueuePointerFromAddress(ownerPart), 0x34);
    uint32_t queueAddress = CSF_AT(uint32_t, QueuePointerFromAddress(ownerSubPart), 0x310);
    C_0082f9f8* queue = reinterpret_cast<C_0082f9f8*>(static_cast<uintptr_t>(queueAddress));

    int linkedField10 = GetLinkedObjectField10(reinterpret_cast<void*>(static_cast<uintptr_t>(linkedObject)), nullptr);
    return static_cast<int>(C_0082f9f8_QueueAction(queue, nullptr, linkedField10 + 0x14));
}

HOOK(0x004AD4B0, QueueLinkedObjectAction, "auto");
