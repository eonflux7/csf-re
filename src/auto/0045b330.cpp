#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, DefaultDestination_Get, 0x004FE4A0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, SubmitFlaggedPositionCommand, 0x0044D020, (void* a1, int a2, int a3, int* a4));

static uint8_t* PointerFromValue(uint32_t value)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(value));
}

static uint32_t DwordAtOffset(uint8_t* base, size_t offset)
{
    return *reinterpret_cast<uint32_t*>(base + offset);
}

static void* __stdcall PostFlaggedPositionOrder(int /*a1*/, int /*a2*/, uint32_t* a3)
{
    int flagWord = 0;
    int xFromFirstFloat = 0;

    // LOBYTE(a4) = 0; v11 = 0;
    reinterpret_cast<uint8_t*>(&flagWord)[0] = 0;
    xFromFirstFloat = 0;

    // Flag byte: *(BYTE*)(*(a3[1] + 4) + 1)
    uint8_t flagByte = *(PointerFromValue(DwordAtOffset(PointerFromValue(a3[1]), 4)) + 1);
    reinterpret_cast<uint8_t*>(&flagWord)[0] = flagByte;

    // First float: *(float*)(*(a3[2] + 4) + 4)
    float firstFloat = *reinterpret_cast<float*>(
        PointerFromValue(DwordAtOffset(PointerFromValue(a3[2]), 4)) + 4);
    xFromFirstFloat = static_cast<int>(firstFloat);

    // Second float: *(float*)(*(a3[0] + 4) + 4)
    float secondFloat = *reinterpret_cast<float*>(
        PointerFromValue(DwordAtOffset(PointerFromValue(a3[0]), 4)) + 4);
    int yFromSecondFloat = static_cast<int>(secondFloat);

    int globalValue = GetGlobalValue_8B0568();
    void* destination = DefaultDestination_Get(
        reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr);

    // edx is not set by the original before the call (v7 is never assigned), so pass 0.
    return SubmitFlaggedPositionCommand(destination, 0, yFromSecondFloat, &flagWord);
}

HOOK(0x0045B330, PostFlaggedPositionOrder, "auto");
