#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void*));

CSF_GLOBAL(float, g_Const200_0f, 0x0082DDEC);

static int __stdcall AiCoverTasks_InitializeCachedTaskTarget(int task, int target)
{
    void* taskObject = reinterpret_cast<void*>(task);

    int linkedValue = GetLinkedObjectValue_4DB5C0(taskObject, nullptr);
    void* linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    CSF_AT(int, linkedObject, 0x69C) = target;

    linkedValue = GetLinkedObjectValue_4DB5C0(taskObject, nullptr);
    linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    void* position = GetObjectPositionVector(
        CSF_AT(void*, linkedObject, 0x2C), nullptr);

    const float positionY = CSF_AT(float, position, 0x4);
    const std::uint32_t positionX = CSF_AT(std::uint32_t, position, 0x0);
    const std::uint32_t positionZ = CSF_AT(std::uint32_t, position, 0x8);
    const float raisedY = positionY + g_Const200_0f;

    linkedValue = GetLinkedObjectValue_4DB5C0(taskObject, nullptr);
    linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    CSF_AT(std::uint32_t, linkedObject, 0x2B0) = 0;
    CSF_AT(std::uint32_t, linkedObject, 0x2B4) = 0;
    CSF_AT(int, linkedObject, 0x2A8) = 1;
    CSF_AT(std::uint32_t, linkedObject, 0x2B8) = positionX;
    CSF_AT(float, linkedObject, 0x2BC) = raisedY;
    CSF_AT(std::uint32_t, linkedObject, 0x2C0) = positionZ;

    linkedValue = GetLinkedObjectValue_4DB5C0(taskObject, nullptr);
    const int result = linkedValue ? linkedValue - 4 : 0;
    linkedObject = reinterpret_cast<void*>(result);
    CSF_AT(std::uint32_t, linkedObject, 0x2B0) = 1065353216;
    CSF_AT(std::uint32_t, linkedObject, 0x2B4) = 1065353216;
    return result;
}

HOOK(0x004AC170, AiCoverTasks_InitializeCachedTaskTarget, "auto");
