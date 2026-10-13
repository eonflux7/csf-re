#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, RangeList_ContainsValue, 0x0069B320, (void* self, void* , uint32_t value));
CSF_FUNC(char, __fastcall, S_0069b1c0_TryReleaseBlock, 0x0069B590, (S_0069b1c0* self, void* , unsigned int address));
CSF_FUNC(void*, __cdecl, _realloc, 0x007B1ED4, (void* Block, uint32_t Size));

using DebugOutputFunction = void (__stdcall*)(const char*);
CSF_GLOBAL(DebugOutputFunction, g_OutputDebugStringA, 0x0082D0B4);

static uint32_t* __fastcall C_0085eac0_ReallocateBlock(
    C_0085eac0* self, void* /*edx*/, uint32_t* block,
    size_t size, int allocationArg, int allocationContext)
{
    if (!size)
        return nullptr;

    using AllocateFunction = uint32_t* (__fastcall*)(
        C_0085eac0*, void*, size_t, int, int);
    using MemoryDeltaFunction = uint32_t (__fastcall*)(C_0085eac0*, void*);

    uint32_t* result = nullptr;
    const uint32_t blockAddress = reinterpret_cast<uint32_t>(block);

    if (self->field_0118 &&
        RangeList_ContainsValue(reinterpret_cast<uint8_t*>(self) + 0x8,
                                nullptr, blockAddress))
    {
        if (size <= 4)
        {
            result = block;
        }
        else
        {
            result = reinterpret_cast<AllocateFunction>(self->vtbl[0xC / 4])(
                self, nullptr, size, allocationArg, allocationContext);
            result[0] = block[0];
            S_0069b1c0_TryReleaseBlock(
                reinterpret_cast<S_0069b1c0*>(
                    reinterpret_cast<uint8_t*>(self) + 0x8),
                nullptr, blockAddress);
        }
    }

    if (self->field_0118)
    {
        if (RangeList_ContainsValue(reinterpret_cast<uint8_t*>(self) + 0x4C,
                                    nullptr, blockAddress))
        {
            if (size <= 8)
            {
                result = block;
            }
            else
            {
                result = reinterpret_cast<AllocateFunction>(self->vtbl[0xC / 4])(
                    self, nullptr, size, allocationArg, allocationContext);
                result[0] = block[0];
                result[1] = block[1];
                S_0069b1c0_TryReleaseBlock(
                    reinterpret_cast<S_0069b1c0*>(
                        reinterpret_cast<uint8_t*>(self) + 0x4C),
                    nullptr, blockAddress);
            }
        }

        if (self->field_0118)
        {
            if (!result &&
                RangeList_ContainsValue(reinterpret_cast<uint8_t*>(self) + 0x90,
                                        nullptr, blockAddress))
            {
                if (size <= 0x10)
                {
                    result = block;
                }
                else
                {
                    result = reinterpret_cast<AllocateFunction>(self->vtbl[0xC / 4])(
                        self, nullptr, size, allocationArg, allocationContext);
                    result[0] = block[0];
                    result[1] = block[1];
                    result[2] = block[2];
                    result[3] = block[3];
                    S_0069b1c0_TryReleaseBlock(
                        reinterpret_cast<S_0069b1c0*>(
                            reinterpret_cast<uint8_t*>(self) + 0x90),
                        nullptr, blockAddress);
                }
            }

            if (self->field_0118)
            {
                if (result)
                    return result;

                if (RangeList_ContainsValue(
                        reinterpret_cast<uint8_t*>(self) + 0xD4,
                        nullptr, blockAddress))
                {
                    if (size <= 0x20)
                    {
                        result = block;
                    }
                    else
                    {
                        result = reinterpret_cast<AllocateFunction>(self->vtbl[0xC / 4])(
                            self, nullptr, size, allocationArg, allocationContext);
                        std::memcpy(result, block, 0x20);
                        S_0069b1c0_TryReleaseBlock(
                            reinterpret_cast<S_0069b1c0*>(
                                reinterpret_cast<uint8_t*>(self) + 0xD4),
                            nullptr, blockAddress);
                    }
                }
            }
        }
    }

    if (!result)
    {
        result = static_cast<uint32_t*>(_realloc(block, size));
        if (!result)
        {
            reinterpret_cast<MemoryDeltaFunction>(self->vtbl[0x2C / 4])(
                self, nullptr);
            g_OutputDebugStringA(CSF_STR(0x0085EB10, "\n\n OUT OF MEMORY \n\n"));
        }
    }

    return result;
}

HOOK(0x0069E050, C_0085eac0_ReallocateBlock, "auto");
