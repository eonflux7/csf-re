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

CSF_FUNC(bool, __fastcall, EvaluateCallbackPredicate, 0x00472820, (void* self, void* edx, void* a2, int a3));

static bool __stdcall WriteCallbackResultFlag(int a1, int a2, int a3)
{
    void* argBlock = reinterpret_cast<void*>(a3);

    // Original reads a1->field_4 before the call and writes through it afterwards.
    uint32_t flagOwnerBase = CSF_AT(uint32_t, reinterpret_cast<void*>(a1), 0x04);

    uint32_t thirdArgValue = CSF_AT(uint32_t, argBlock, 0x08);
    uint32_t secondArgValue = CSF_AT(uint32_t, argBlock, 0x00);
    void* stateObject = reinterpret_cast<void*>(CSF_AT(uint32_t, argBlock, 0x04));
    void* selfPointer = reinterpret_cast<void*>(CSF_AT(uint32_t, stateObject, 0x04));

    bool result = EvaluateCallbackPredicate(
        selfPointer,
        reinterpret_cast<void*>(thirdArgValue),
        reinterpret_cast<void*>(secondArgValue),
        static_cast<int>(thirdArgValue));

    uint8_t* flagByte = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(flagOwnerBase) + 1);
    *flagByte = static_cast<uint8_t>(result);

    return result;
}

HOOK(0x0045B000, WriteCallbackResultFlag, "auto");
