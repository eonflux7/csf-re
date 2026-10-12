#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, AiDistracted_UpdateLinkedState, 0x004B3430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_004D10F0_ResetFields, 0x004D10F0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

static int __stdcall AiAlert_ResetLinkedState(void* alert)
{
    uint32_t linkedValue = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(alert, nullptr));
    uintptr_t actorAddress = linkedValue ? linkedValue - 4 : 0;
    CSF_AT(uint32_t, reinterpret_cast<void*>(actorAddress), 0x6DC) = 0;

    linkedValue = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(alert, nullptr));
    actorAddress = linkedValue ? linkedValue - 4 : 0;
    C_004D10F0_ResetFields(
        reinterpret_cast<void*>(actorAddress + 0x690), nullptr);

    linkedValue = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(alert, nullptr));
    actorAddress = linkedValue ? linkedValue - 4 : 0;
    CoverAttack_SetStance_492720(
        CSF_AT(void*, reinterpret_cast<void*>(actorAddress), 0x2C),
        nullptr, 3);

    linkedValue = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(alert, nullptr));
    actorAddress = linkedValue ? linkedValue - 4 : 0;
    void* controller =
        CSF_AT(void*, reinterpret_cast<void*>(actorAddress), 0x2C);
    ((void(__fastcall*)(void*, void*, int, int, int, int))
        CSF_AT(void**, controller, 0)[0x8C / 4])(
            controller, nullptr, 0, 0, -1, 0);

    linkedValue = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(alert, nullptr));
    actorAddress = linkedValue ? linkedValue - 4 : 0;
    AiDistracted_UpdateLinkedState(
        reinterpret_cast<void*>(actorAddress), nullptr);

    linkedValue = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(alert, nullptr));
    actorAddress = linkedValue ? linkedValue - 4 : 0;
    return UnnamedApply_4DC180(
        alert, nullptr, static_cast<int>(actorAddress + 0x418));
}

HOOK(0x004B5030, AiAlert_ResetLinkedState, "auto");
