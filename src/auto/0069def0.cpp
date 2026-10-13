#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0069afe0_ProcessSectionTable, 0x0069B460, (S_0069afe0* self, void*));
CSF_FUNC(void*, __cdecl, _malloc, 0x007B1EC2, (uint32_t Size));

using DebugOutputFunction = void (__stdcall*)(const char*);
CSF_GLOBAL(DebugOutputFunction, g_OutputDebugStringA, 0x0082D0B4);

static void* __fastcall C_0085eac0_AllocateInner(
    C_0085eac0* self, void* /*edx*/, size_t size, int /*a3*/, int /*a4*/)
{
    if (!size)
        return _malloc(0);

    void* allocation = nullptr;
    if (self->field_0118 && size <= 0x20)
    {
        if (size <= 4)
        {
            allocation = reinterpret_cast<void*>(
                S_0069afe0_ProcessSectionTable(
                    reinterpret_cast<S_0069afe0*>(
                        reinterpret_cast<uint8_t*>(self) + 0x8),
                    nullptr));
        }

        if (!allocation && size <= 8)
        {
            allocation = reinterpret_cast<void*>(
                S_0069afe0_ProcessSectionTable(
                    reinterpret_cast<S_0069afe0*>(
                        reinterpret_cast<uint8_t*>(self) + 0x4C),
                    nullptr));
        }

        if (!allocation)
        {
            auto* pool = reinterpret_cast<S_0069afe0*>(
                reinterpret_cast<uint8_t*>(self) +
                (size > 0x10 ? 0xD4 : 0x90));
            allocation = reinterpret_cast<void*>(
                S_0069afe0_ProcessSectionTable(pool, nullptr));
        }
    }

    if (!allocation)
    {
        allocation = _malloc(size);
        if (!allocation)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*)>(
                self->vtbl[0x2C / 4])(self, nullptr);
            g_OutputDebugStringA(
                CSF_STR(0x0085EB10, "\n\n OUT OF MEMORY \n\n"));
        }
    }

    return allocation;
}

HOOK(0x0069DEF0, C_0085eac0_AllocateInner, "auto");
