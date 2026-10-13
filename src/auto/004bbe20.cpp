#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(void, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, float angle));
CSF_FUNC(unsigned int, __fastcall, AiDistracted_UpdateGuardWaypoint, 0x004B9A80, (void* self, void* , int* position));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(char, __fastcall, C_0083e0d8_FindNearestWaypoint, 0x0075CB30, (C_0083e0d8* self, void* , int groupId, float* position, uint32_t* result));
CSF_FUNC(double, __fastcall, C_0086c288_GetFacingAngleRadians, 0x0075D0A0, (C_0086c288* self, void* , uint32_t* a2));

CSF_GLOBAL(float, g_PayloadScale, 0x0083543C);

static int __stdcall AiDistracted_FaceNearestGuardWaypoint(
    int context, int /*unused2*/, int /*unused3*/)
{
    // Retain the vtable binding identified by the packet.
    (void)CSF_VTBL(0x0047AE10);

    void* state = reinterpret_cast<void*>(context);
    void* world = reinterpret_cast<void*>(GetValue8B0554());
    C_0083e0d8* waypoints = CSF_AT(C_0083e0d8*, world, 0x1FD8);

    int linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    void* guard = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;

    void* waypointState =
        reinterpret_cast<void*>(GetLinkedObjectField10(state, nullptr));
    AiDistracted_UpdateGuardWaypoint(
        guard, nullptr,
        reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(waypointState) + 28));

    waypointState =
        reinterpret_cast<void*>(GetLinkedObjectField10(state, nullptr));
    if (CSF_AT(uint8_t, waypointState, 0x28))
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
        guard = linkedValue
            ? reinterpret_cast<uint8_t*>(linkedValue) - 4
            : nullptr;

        S_0047ae10* task = CSF_AT(S_0047ae10*, guard, 0x718);
        if (*AiCore_GetTaskTargetNameBuffer(task, nullptr) == '\0')
        {
            alignas(8) uint8_t waypointBytes[0x8];
            uint32_t* waypoint = reinterpret_cast<uint32_t*>(waypointBytes);
            waypoint[0] = static_cast<uint32_t>(-1);
            waypoint[1] = static_cast<uint32_t>(-1);

            waypointState =
                reinterpret_cast<void*>(GetLinkedObjectField10(state, nullptr));
            if (C_0083e0d8_FindNearestWaypoint(
                    waypoints, nullptr, 3,
                    reinterpret_cast<float*>(
                        reinterpret_cast<uint8_t*>(waypointState) + 28),
                    waypoint))
            {
                float angle = static_cast<float>(
                    C_0086c288_GetFacingAngleRadians(
                        reinterpret_cast<C_0086c288*>(waypoints),
                        nullptr, waypoint));

                linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
                guard = linkedValue
                    ? reinterpret_cast<uint8_t*>(linkedValue) - 4
                    : nullptr;

                float payload = angle * g_PayloadScale;
                DispatchSceneObjectWithPayload(
                    CSF_AT(void*, guard, 0x2C), nullptr, payload);
            }
        }
    }

    return Dispatch_NoLinkedTarget(state, nullptr, 0);
}

HOOK(0x004BBE20, AiDistracted_FaceNearestGuardWaypoint, "auto");
