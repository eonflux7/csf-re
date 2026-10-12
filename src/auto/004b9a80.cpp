#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_004af750_EvaluateGuardWaypoint, 0x004AF750, (S_004af750* self, void* , float* position));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(unsigned int, __fastcall, DispatchWithArgBlock, 0x004DD060, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(char, __fastcall, C_0083e0d8_FindNearestWaypoint, 0x0075CB30, (C_0083e0d8* self, void* , int groupId, float* position, uint32_t* result));

static unsigned int __fastcall AiDistracted_UpdateGuardWaypoint(
    void* self, void* /*edx*/, int* position)
{
    // The packet also labels this direct callee address as a vtable.
    (void)CSF_VTBL(0x004AF750);

    alignas(8) uint8_t requestBytes[0x24];
    auto* request = reinterpret_cast<uint32_t*>(requestBytes);

    auto* context = reinterpret_cast<void*>(GetValue8B0554());
    auto* waypointGraph = CSF_AT(C_0083e0d8*, context, 0x1FD8);
    auto* savedPosition = static_cast<uint8_t*>(self) + 0x6B4;

    CSF_AT(int, savedPosition, 0) = position[0];
    CSF_AT(int, savedPosition, 4) = position[1];
    CSF_AT(int, savedPosition, 8) = position[2];

    auto* linkedObject = static_cast<uint8_t*>(self) + 8;
    request[7] = reinterpret_cast<uint32_t>(waypointGraph);
    request[8] = reinterpret_cast<uint32_t>(savedPosition);

    int linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    uintptr_t waypointResultObject =
        linkedValue ? static_cast<uint32_t>(linkedValue) - 4u : 0;

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    uintptr_t waypointPositionObject =
        linkedValue ? static_cast<uint32_t>(linkedValue) - 4u : 0;

    C_0083e0d8_FindNearestWaypoint(
        waypointGraph, nullptr, 3,
        reinterpret_cast<float*>(waypointPositionObject + 0x6B4),
        reinterpret_cast<uint32_t*>(waypointResultObject + 0x6C0));

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    uintptr_t evaluationPositionObject =
        linkedValue ? static_cast<uint32_t>(linkedValue) - 4u : 0;

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    auto* guard = reinterpret_cast<S_004af750*>(
        linkedValue ? static_cast<uint32_t>(linkedValue) - 4u : 0);

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    auto* evaluationResultObject = reinterpret_cast<void*>(
        linkedValue ? static_cast<uint32_t>(linkedValue) - 4u : 0);

    int evaluation = S_004af750_EvaluateGuardWaypoint(
        guard, nullptr,
        reinterpret_cast<float*>(evaluationPositionObject + 0x6B4));
    CSF_AT(int, evaluationResultObject, 0x6C8) = evaluation;

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    auto* guardObject = reinterpret_cast<void*>(
        linkedValue ? static_cast<uint32_t>(linkedValue) - 4u : 0);
    auto* owner = CSF_AT(uint8_t*, guardObject, 0x2C);
    auto* ownerIdentity = owner + 0x1C;

    request[0] = 1;
    request[1] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    request[2] = CSF_AT(uint32_t, ownerIdentity, 4);
    uint32_t identityPart = CSF_AT(uint32_t, ownerIdentity, 8);
    request[4] = CSF_AT(uint32_t, savedPosition, 0);
    request[3] = identityPart;
    request[5] = CSF_AT(uint32_t, savedPosition, 4);
    request[6] = CSF_AT(uint32_t, savedPosition, 8);

    return DispatchWithArgBlock(&waypointGraph->field_0044, nullptr, request);
}

HOOK(0x004B9A80, AiDistracted_UpdateGuardWaypoint, "auto");
