#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

// The allocator (0x484070 -> Mem_AllocDebug(2 * count, 0, 0)) may throw; it is called through a
// plain C++ function pointer so the try blocks around it are kept by the compiler.
typedef int (__stdcall* WideBufferAllocFn)(int count, int hint);

static void* __fastcall S_004f21d0_GrowBuffer(
    S_004f21d0* self, void* /*edx*/, int requestedCapacity, int preservedLength)
{
    const WideBufferAllocFn allocateWideBuffer = (WideBufferAllocFn)CSF_ADDR_FN(0x00484070);

    uint32_t capacity = static_cast<uint32_t>(requestedCapacity) | 7;

    if (capacity > 0xFFFFFFFE)
    {
        capacity = static_cast<uint32_t>(requestedCapacity);
    }
    else
    {
        const uint32_t oldCapacity = self->field_0018;
        const uint32_t halfCapacity = oldCapacity >> 1;
        if (capacity / 3 < halfCapacity &&
            oldCapacity <= 0xFFFFFFFE - halfCapacity)
        {
            capacity = oldCapacity + halfCapacity;
        }
    }

    void* buffer;
    try
    {
        buffer = reinterpret_cast<void*>(
            allocateWideBuffer(static_cast<int>(capacity + 1), 0));
    }
    catch (...)
    {
        capacity = static_cast<uint32_t>(requestedCapacity);
        try
        {
            buffer = reinterpret_cast<void*>(
                allocateWideBuffer(static_cast<int>(capacity + 1), 0));
        }
        catch (...)
        {
            if (self->field_0018 >= 8 &&
                CSF_AT(void*, self, 0x04) != nullptr)
            {
                Mem_FreeDebug(CSF_AT(void*, self, 0x04));
            }
            self->field_0018 = 7;
            self->field_0014 = 0;
            self->field_0004 = 0;
            RethrowCppException(nullptr, nullptr);
        }
    }

    const uint32_t length = static_cast<uint32_t>(preservedLength);
    if (length != 0)
    {
        const void* source = self->field_0018 < 8
            ? static_cast<const void*>(&self->field_0004)
            : CSF_AT(void*, self, 0x04);
        std::memcpy(buffer, source, 2 * length);
    }

    if (self->field_0018 >= 8 &&
        CSF_AT(void*, self, 0x04) != nullptr)
    {
        Mem_FreeDebug(CSF_AT(void*, self, 0x04));
    }

    self->field_0004 = 0;
    CSF_AT(void*, self, 0x04) = buffer;
    self->field_0018 = capacity;
    self->field_0014 = length;

    void* result = capacity < 8
        ? static_cast<void*>(&self->field_0004)
        : buffer;
    static_cast<uint16_t*>(result)[length] = 0;
    return result;
}

HOOK(0x00486550, S_004f21d0_GrowBuffer, "auto");
