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

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(int, __stdcall, AiAlert_ApplyCoverAttack, 0x004ABF50, (int a1, uint32_t* a2));
CSF_FUNC(int, __fastcall, AiCore_UpdateTargetData, 0x004D1730, (S_0048c9f0* self, void* , int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Required by the packet's vtable list; the actual invocation is a direct call.
static const auto g_TaskTargetBufferVtable = CSF_VTBL(0x0047AE10);

static int __stdcall AiFncMacroOrder_SubmitCoverAttackRequest(
    C_0085e7b8* self, float duration, int argument3, int argument4)
{
    int linkedObject = GetLinkedObjectValue_4DB5C0(self, nullptr);
    uintptr_t ownerAddress = linkedObject
        ? static_cast<uintptr_t>(linkedObject) - 4
        : 0;

    S_0047ae10* task = CSF_AT(
        S_0047ae10*, reinterpret_cast<void*>(ownerAddress), 0x718);

    int targetDataAddress = GetLinkedObjectField10(self, nullptr) + 0x94;
    char* targetBuffer = AiCore_GetTaskTargetNameBuffer(task, nullptr);
    AiCore_UpdateTargetData(
        reinterpret_cast<S_0048c9f0*>(targetBuffer), nullptr, targetDataAddress);

    alignas(8) uint8_t requestBytes[0x10]{};
    CSF_AT(uint8_t, requestBytes, 0x0) = 1;
    CSF_AT(uint8_t, requestBytes, 0x1) = 0;
    // Unordered comparisons take the same branch as values at or below zero.
    CSF_AT(int, requestBytes, 0x4) = duration > g_fZero ? 4 : 8;
    CSF_AT(int, requestBytes, 0xC) = argument4;
    CSF_AT(int, requestBytes, 0x8) = argument3;

    linkedObject = GetLinkedObjectValue_4DB5C0(self, nullptr);
    ownerAddress = linkedObject
        ? static_cast<uintptr_t>(linkedObject) - 4
        : 0;
    UnnamedApply_4DC180(self, nullptr, static_cast<int>(ownerAddress + 0x3D8));

    linkedObject = GetLinkedObjectValue_4DB5C0(self, nullptr);
    ownerAddress = linkedObject
        ? static_cast<uintptr_t>(linkedObject) - 4
        : 0;

    void* stateProvider = GetCachedObjectProvider(self, nullptr);
    CSF_AT(uint8_t, reinterpret_cast<void*>(ownerAddress), 0x400) = 1;
    CoverAttack_SetState_4DC150(
        stateProvider, nullptr, static_cast<int>(ownerAddress + 0x3DC));

    void* alertProvider = GetCachedObjectProvider(stateProvider, nullptr);
    return AiAlert_ApplyCoverAttack(
        static_cast<int>(reinterpret_cast<uintptr_t>(alertProvider)),
        reinterpret_cast<uint32_t*>(requestBytes));
}

HOOK(0x004B1E10, AiFncMacroOrder_SubmitCoverAttackRequest, "auto");
