#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees bound by address. Names for the two unnamed callees are chosen here.
CSF_FUNC(void**, __fastcall, S_00434780_EraseTailRange, 0x00434780,
         (void* self, void* edx, void** countPtr, void* firstRemoved, void* endPtr));
CSF_FUNC(int, __fastcall, S_00434780_InsertCopiesAt, 0x00435290,
         (void* self, void* edx, void* insertPos, unsigned int count, void* valuePtr));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Element stride is 48 bytes. Signed division, as in the original.
static uint32_t ElementCountBetween(uint32_t firstElem, uint32_t endElem)
{
    return static_cast<uint32_t>(static_cast<int>(endElem - firstElem) / 48);
}

// Reinterprets a raw count/integer result as the pointer-typed return value.
static C_0082d410 **AsResultPointer(std::uintptr_t raw)
{
    return reinterpret_cast<C_0082d410 **>(raw);
}

// Resizes the 48-byte element sequence to requestedCount elements.
// Stack arguments total 52 bytes (ret 0x34).
static C_0082d410 **__fastcall S_00434780_ResizeWithValue(
    S_00434780 *self, void * /*edx*/,
    unsigned int requestedCount,
    C_0082d410 fillValue,
    int /*a4*/, int /*a5*/, int /*a6*/,
    unsigned int fillValueCapacity,
    int /*a8*/, int /*a9*/, int /*a10*/, int /*a11*/, int /*a12*/)
{
    // Packet-listed vtable reference. Address 0x43600C is a call to 0x434780,
    // not a vtable store, so nothing is emitted here.
    (void)CSF_VTBL(0x00434780);

    // Cached begin pointer. The original keeps it in a register across calls.
    const uint32_t firstElem = self->field_0004;
    C_0082d410 **result;

    if (firstElem != 0)
        result = AsResultPointer(ElementCountBetween(firstElem, self->field_0008));
    else
        result = nullptr;

    if (reinterpret_cast<std::uintptr_t>(result) >= requestedCount)
    {
        if (firstElem != 0)
        {
            result = AsResultPointer(ElementCountBetween(firstElem, self->field_0008));
            if (requestedCount < reinterpret_cast<std::uintptr_t>(result))
            {
                void *firstRemoved = reinterpret_cast<void *>(firstElem + 48u * requestedCount);
                result = reinterpret_cast<C_0082d410 **>(
                    S_00434780_EraseTailRange(self, nullptr,
                                              reinterpret_cast<void **>(&requestedCount),
                                              firstRemoved,
                                              reinterpret_cast<void *>(self->field_0008)));
            }
        }
    }
    else
    {
        const uint32_t currentCount =
            (firstElem != 0) ? ElementCountBetween(firstElem, self->field_0008) : 0u;
        result = reinterpret_cast<C_0082d410 **>(
            S_00434780_InsertCopiesAt(self, nullptr,
                                      reinterpret_cast<void *>(self->field_0008),
                                      requestedCount - currentCount,
                                      &fillValue));
    }

    // When the value's buffer is heap-allocated (capacity >= 16), free it.
    // Note: result is overwritten with fillValue.field_0004 here, as in the original.
    if (fillValueCapacity >= 0x10u)
    {
        result = reinterpret_cast<C_0082d410 **>(fillValue.field_0004);
        if (result != nullptr)
            return reinterpret_cast<C_0082d410 **>(
                Mem_FreeDebug(reinterpret_cast<void *>(result)));
    }

    return result;
}

HOOK(0x00435F80, S_00434780_ResizeWithValue, "auto");

