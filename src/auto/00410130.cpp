#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, sub_40FD80, 0x0040FD80, (uint32_t a1, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

// Body of the original catch(...) handler (the `ret` 0 funclet): retry with the exact
// requested capacity; if that fails too, release the string and rethrow.
static void* __cdecl C_0082d410_RetryAllocateExact(C_0082d410* self, uint32_t requestedCapacity)
{
    try
    {
        return reinterpret_cast<void*>(static_cast<uintptr_t>(sub_40FD80(requestedCapacity + 1, 0)));
    }
    catch (...)
    {
        if (CSF_AT(uint32_t, self, 0x18) >= 0x10 && self->field_0004)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0004));

        CSF_AT(uint32_t, self, 0x18) = 0xF;
        CSF_AT(uint32_t, self, 0x14) = 0;
        *reinterpret_cast<uint8_t*>(&self->field_0004) = 0;
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }
}

static void* __fastcall C_0082d410_ReallocateBuffer(C_0082d410* self, void* /*edx*/, int requestedCapacity, unsigned int preservedLength)
{
    uint32_t capacity = static_cast<uint32_t>(requestedCapacity) | 0xF;
    if (capacity == 0xFFFFFFFF)
    {
        capacity = static_cast<uint32_t>(requestedCapacity);
    }
    else
    {
        const uint32_t oldCapacity = CSF_AT(uint32_t, self, 0x18);
        const uint32_t halfCapacity = oldCapacity >> 1;
        if (capacity / 3 < halfCapacity && oldCapacity <= 0xFFFFFFFEu - halfCapacity)
            capacity = oldCapacity + halfCapacity;
    }

    void* allocation;
    try
    {
        allocation = reinterpret_cast<void*>(static_cast<uintptr_t>(sub_40FD80(capacity + 1, 0)));
    }
    catch (...)
    {
        capacity = static_cast<uint32_t>(requestedCapacity);
        allocation = C_0082d410_RetryAllocateExact(self, capacity);
    }

    if (preservedLength)
    {
        const void* source;
        if (CSF_AT(uint32_t, self, 0x18) < 0x10)
            source = &self->field_0004;
        else
            source = reinterpret_cast<void*>(self->field_0004);

        std::memcpy(allocation, source, preservedLength);
    }

    if (CSF_AT(uint32_t, self, 0x18) >= 0x10 && self->field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0004));

    *reinterpret_cast<uint8_t*>(&self->field_0004) = 0;
    self->field_0004 = reinterpret_cast<uintptr_t>(allocation);
    CSF_AT(uint32_t, self, 0x18) = capacity;
    CSF_AT(uint32_t, self, 0x14) = preservedLength;

    void* result = allocation;
    if (capacity < 0x10)
        result = &self->field_0004;
    static_cast<uint8_t*>(result)[preservedLength] = 0;
    return result;
}

HOOK(0x00410130, C_0082d410_ReallocateBuffer, "auto");
