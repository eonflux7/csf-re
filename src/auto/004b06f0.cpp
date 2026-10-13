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

CSF_FUNC(int, __stdcall, AiState_ForwardUpdateToChildTask, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, S_0048c9f0_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));

[[maybe_unused]] static void* const g_TargetHandleVtbl = CSF_VTBL(0x0048C9F0);

static int __stdcall TEstatico_CopyGuardTargetHandle(int a1, int a2)
{
    AiState_ForwardUpdateToChildTask(a1, a2);

    int result = IsField10Null(reinterpret_cast<void*>(a1), nullptr);
    if (!static_cast<uint8_t>(result))
    {
        // First linked-object lookup: its value becomes the base of the field block (value - 4).
        int linkedFirst = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(a1), nullptr);
        uint8_t* blockA = nullptr;
        if (linkedFirst)
            blockA = reinterpret_cast<uint8_t*>(linkedFirst - 4);

        // Second lookup: owner block used to reach the target object.
        int linkedSecond = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(a1), nullptr);
        uint8_t* blockB = nullptr;
        if (linkedSecond)
            blockB = reinterpret_cast<uint8_t*>(linkedSecond - 4);

        // Pick handle field: 0x628 normally, 0x610 when the signed word at 0x630 is negative.
        uint8_t* handleField = blockA + 0x628;
        if (CSF_AT(int, blockA, 0x630) < 0)
            handleField = blockA + 0x610;

        uint8_t* owner = CSF_AT(uint8_t*, blockB, 0x2C);
        S_0048c9f0* target = CSF_AT(S_0048c9f0*, owner, 0x204);

        // The original returns straight after this call: eax is whatever the callee left.
        return S_0048c9f0_CopyTargetHandle(target, nullptr, reinterpret_cast<int>(handleField));
    }
    return result;
}

HOOK(0x004B06F0, TEstatico_CopyGuardTargetHandle, "auto");


