#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_004D10F0_ResetFields, 0x004D10F0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetCoverTransition, 0x004D1100, (void* self, void* , int a2, int a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static int __fastcall TMovilAPuntoRastreo_InitializeCoverTransition(
    TMovilAPuntoRastreo* self, void* /*edx*/, TAiState* state)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    uintptr_t ownerAddress = linkedValue
        ? static_cast<uintptr_t>(linkedValue) - 4
        : 0;

    C_004D10F0_ResetFields(
        reinterpret_cast<void*>(ownerAddress + 0x690), nullptr);

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    ownerAddress = linkedValue
        ? static_cast<uintptr_t>(linkedValue) - 4
        : 0;

    void* firstGlobal = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));
    void* secondGlobal = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));

    float halfDuration =
        CSF_AT(float, secondGlobal, 0xA94) * g_HalfDurationScale;
    int fullDurationBits = CSF_AT(int, firstGlobal, 0xA94);
    int halfDurationBits;
    std::memcpy(&halfDurationBits, &halfDuration, sizeof(halfDurationBits));

    AiCoverAttackTasks_SetCoverTransition(
        reinterpret_cast<void*>(ownerAddress + 0x690), nullptr,
        halfDurationBits, fullDurationBits);

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    ownerAddress = linkedValue
        ? static_cast<uintptr_t>(linkedValue) - 4
        : 0;

    void* actor = CSF_AT(void*, reinterpret_cast<void*>(ownerAddress), 0x2C);
    CoverAttack_SetStance_492720(actor, nullptr, 2);

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    ownerAddress = linkedValue
        ? static_cast<uintptr_t>(linkedValue) - 4
        : 0;

    actor = CSF_AT(void*, reinterpret_cast<void*>(ownerAddress), 0x2C);
    ((void(__fastcall*)(void*, void*, int, int, int, int))
        CSF_AT(void**, actor, 0)[0x8C / 4])(
            actor, nullptr, 0, 0, -1, 0);

    return CoverAttack_SetState_4DC150(
        state, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(
            reinterpret_cast<uint8_t*>(self) + 4)));
}

HOOK(0x004AAEF0, TMovilAPuntoRastreo_InitializeCoverTransition, "auto");
