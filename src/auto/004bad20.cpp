#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(void, __stdcall, AiCore_AdjustPositionToSceneSurface, 0x004AA6D0, (S_004e0c20* a1));
CSF_FUNC(int, __fastcall, AiCore_IsTargetPositionValid, 0x004AEDD0, (S_004aa310* self, void* , int a2));
CSF_FUNC(char, __fastcall, S_004b0200_FindReachableOffset, 0x004B0300, (S_004b0200* self, void* , int actor, int positionAddress));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(uint32_t, __fastcall, S_004dca10_DispatchWithArgBlock, 0x004DD060, (S_004dca10* self, void* , uint32_t* args));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

static S_004aa310* AiDistracted_GetLinkedBase(void* actor)
{
    const int linked = GetLinkedObjectValue_4DB5C0(actor, nullptr);
    return linked
        ? reinterpret_cast<S_004aa310*>(static_cast<uint32_t>(linked) - 4)
        : nullptr;
}

static int __fastcall S_004b0200_ApplyLinkedPosition(
    S_004b0200* self, void* /*edx*/, int actor,
    char findReachableOffset, uint32_t* requestedPosition,
    int positionOption, char secondaryFlag)
{
    self->field_0015 = findReachableOffset;
    self->field_0014 = secondaryFlag;

    uint32_t* position = &self->field_0004;
    self->field_0004 = requestedPosition[0];
    std::memcpy(&self->field_0008, requestedPosition + 1, sizeof(uint32_t));
    CSF_AT(uint32_t, self, 0x0C) = requestedPosition[2];
    self->field_0010 = positionOption;

    void* actorObject = reinterpret_cast<void*>(actor);
    S_004aa310* linkedBase = AiDistracted_GetLinkedBase(actorObject);
    void* entity = CSF_AT(void*, linkedBase, 0x2C);

    bool resetState = CSF_AT(int, entity, 0x1E0) == 1;
    if (!resetState)
    {
        linkedBase = AiDistracted_GetLinkedBase(actorObject);
        entity = CSF_AT(void*, linkedBase, 0x2C);
        resetState = CSF_AT(int, entity, 0x1E0) == 2;
    }

    if (resetState)
    {
        linkedBase = AiDistracted_GetLinkedBase(actorObject);
        entity = CSF_AT(void*, linkedBase, 0x2C);
        if (CSF_AT(int, entity, 0x1C4) == 1)
        {
            linkedBase = AiDistracted_GetLinkedBase(actorObject);
            entity = CSF_AT(void*, linkedBase, 0x2C);
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, entity, 0)[0xB4 / 4])(entity, nullptr, 0);
        }
    }

    S_004aa310* positionOwner = AiDistracted_GetLinkedBase(actorObject);
    S_004aa310* validationOwner = AiDistracted_GetLinkedBase(actorObject);
    int currentPosition = GetObjectPositionVector(
        CSF_AT(void*, positionOwner, 0x2C), nullptr);
    int result = AiCore_IsTargetPositionValid(
        validationOwner, nullptr, currentPosition);
    if (static_cast<uint8_t>(result) == 0)
        return result;

    if (self->field_0015)
    {
        S_004b0200_FindReachableOffset(
            self, nullptr, actor, reinterpret_cast<int>(position));
    }

    validationOwner = AiDistracted_GetLinkedBase(actorObject);
    result = AiCore_IsTargetPositionValid(
        validationOwner, nullptr, reinterpret_cast<int>(position));
    if (static_cast<uint8_t>(result) == 0)
        return result;

    AiDistracted_GetLinkedBase(actorObject);
    AiCore_AdjustPositionToSceneSurface(
        reinterpret_cast<S_004e0c20*>(position));

    linkedBase = AiDistracted_GetLinkedBase(actorObject);
    entity = CSF_AT(void*, linkedBase, 0x2C);
    uint8_t* entityDescriptor = static_cast<uint8_t*>(entity) + 0x1C;

    alignas(8) uint8_t argumentBytes[0x1C];
    uint32_t* arguments = reinterpret_cast<uint32_t*>(argumentBytes);
    arguments[0] = 2;
    arguments[1] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    arguments[2] = CSF_AT(uint32_t, entityDescriptor, 0x04);
    arguments[3] = CSF_AT(uint32_t, entityDescriptor, 0x08);
    arguments[4] = self->field_0004;
    std::memcpy(arguments + 5, &self->field_0008, sizeof(uint32_t));
    arguments[6] = CSF_AT(uint32_t, self, 0x0C);

    void* manager = reinterpret_cast<void*>(GetValue8B0554());
    void* dispatcherOwner = CSF_AT(void*, manager, 0x1FD8);
    S_004dca10_DispatchWithArgBlock(
        reinterpret_cast<S_004dca10*>(
            static_cast<uint8_t*>(dispatcherOwner) + 0x54),
        nullptr, arguments);

    linkedBase = AiDistracted_GetLinkedBase(actorObject);
    entity = CSF_AT(void*, linkedBase, 0x2C);
    void* movement = CSF_AT(void*, entity, 0x30);
    return ((int(__fastcall*)(void*, void*, uint32_t*, int))
        CSF_AT(void**, movement, 0)[0xAC / 4])(
            movement, nullptr, position, 0);
}

HOOK(0x004BAD20, S_004b0200_ApplyLinkedPosition, "auto");
