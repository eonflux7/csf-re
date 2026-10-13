#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, GameHeap_AllocateBlock, 0x00403800, (int size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

[[maybe_unused]] static const uintptr_t g_StringGrowthExceptionInfo =
    CSF_ADDR_DATA(0x00886770);

static void* __fastcall C_0082d410_GrowStringBuffer(
    C_0082d410* self, void* /*edx*/, int requestedCapacity,
    unsigned int preservedLength)
{
    uint32_t newCapacity = static_cast<uint32_t>(requestedCapacity) | 0xF;

    if (newCapacity == 0xFFFFFFFF)
    {
        newCapacity = static_cast<uint32_t>(requestedCapacity);
    }
    else
    {
        const uint32_t oldCapacity = CSF_AT(uint32_t, self, 0x18);
        const uint32_t halfCapacity = oldCapacity >> 1;
        if (newCapacity / 3 < halfCapacity &&
            oldCapacity <= 0xFFFFFFFE - halfCapacity)
        {
            newCapacity = oldCapacity + halfCapacity;
        }
    }

    void* newBuffer = nullptr;
    try
    {
        newBuffer = reinterpret_cast<void*>(
            GameHeap_AllocateBlock(static_cast<int>(newCapacity + 1)));
    }
    catch (...)
    {
        newCapacity = static_cast<uint32_t>(requestedCapacity);
        try
        {
            newBuffer = reinterpret_cast<void*>(
                GameHeap_AllocateBlock(static_cast<int>(newCapacity + 1)));
        }
        catch (...)
        {
            if (CSF_AT(uint32_t, self, 0x18) >= 0x10)
                Mem_Free(reinterpret_cast<void*>(self->field_0004));

            CSF_AT(uint32_t, self, 0x18) = 0xF;
            CSF_AT(uint32_t, self, 0x14) = 0;
            reinterpret_cast<uint8_t*>(&self->field_0004)[0] = 0;
            RethrowCppException(nullptr, nullptr);
        }
    }

    if (preservedLength)
    {
        const void* source;
        if (CSF_AT(uint32_t, self, 0x18) < 0x10)
            source = &self->field_0004;
        else
            source = reinterpret_cast<void*>(self->field_0004);

        std::memcpy(newBuffer, source, preservedLength);
    }

    if (CSF_AT(uint32_t, self, 0x18) >= 0x10)
        Mem_Free(reinterpret_cast<void*>(self->field_0004));

    reinterpret_cast<uint8_t*>(&self->field_0004)[0] = 0;
    self->field_0004 = reinterpret_cast<uint32_t>(newBuffer);
    CSF_AT(uint32_t, self, 0x18) = newCapacity;
    CSF_AT(uint32_t, self, 0x14) = preservedLength;

    void* result = newBuffer;
    if (newCapacity < 0x10)
        result = &self->field_0004;

    static_cast<uint8_t*>(result)[preservedLength] = 0;
    return result;
}

HOOK(0x00403900, C_0082d410_GrowStringBuffer, "auto");
