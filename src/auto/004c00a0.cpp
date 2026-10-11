#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));
CSF_FUNC(int, __fastcall, sub_4BC810, 0x004BC810, (void* self, void* edx, int a2, int a3));

// Pops the first 32-bit entry off the front of a dword queue (shifting the rest down),
// then forwards that entry to the dispatcher at this+0x6C with action code 23.
static bool __fastcall S_004bc930_PopFrontAndDispatch23(S_004bc930* self, void* /*edx*/, int /*a2*/)
{
    uint32_t remainingCount = self->field_00e4 - 1;
    int frontValue = *reinterpret_cast<int*>(static_cast<uintptr_t>(self->field_00dc));

    if (self->field_00e4 == 1)
    {
        self->field_00e4 = 0;
    }
    else
    {
        uint32_t queueBase = self->field_00dc;
        self->field_00e4 = remainingCount;
        _memcpy(reinterpret_cast<void*>(static_cast<uintptr_t>(queueBase)),
                reinterpret_cast<const void*>(static_cast<uintptr_t>(queueBase + 4)),
                static_cast<std::size_t>(4 * remainingCount));
    }

    sub_4BC810(reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x6C), nullptr, 23, frontValue);
    return true;
}

HOOK(0x4C00A0, S_004bc930_PopFrontAndDispatch23, "auto");
