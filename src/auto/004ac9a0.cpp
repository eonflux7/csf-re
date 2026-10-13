#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(void, __stdcall, AiCore_AdjustPositionToSceneSurface, 0x004AA6D0, (S_004e0c20* a1));
CSF_FUNC(int, __fastcall, AiCore_UpdateTargetData, 0x004D1730, (S_0048c9f0* self, void* , int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));

static int __stdcall AiOrder_UpdateTargetAndPosition(void* order)
{
    // Preserve the packet's address-only vtable annotation.
    (void)CSF_VTBL(0x0047AE10);

    int linkedValue = GetLinkedObjectValue_4DB5C0(order, nullptr);
    auto* linkedObject = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;

    auto* taskTarget = CSF_AT(S_0047ae10*, linkedObject, 0x718);
    int targetData = GetLinkedObjectField10(order, nullptr) + 148;
    char* targetBuffer = AiCore_GetTaskTargetNameBuffer(taskTarget, nullptr);
    AiCore_UpdateTargetData(
        reinterpret_cast<S_0048c9f0*>(targetBuffer), nullptr, targetData);

    GetLinkedObjectValue_4DB5C0(order, nullptr);
    int positionData = GetLinkedObjectField10(order, nullptr) + 160;
    AiCore_AdjustPositionToSceneSurface(
        reinterpret_cast<S_004e0c20*>(positionData));

    linkedValue = GetLinkedObjectValue_4DB5C0(order, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;

    void* linkedComponent = CSF_AT(void*, linkedObject, 0x2C);
    void* positionHandler = CSF_AT(void*, linkedComponent, 0x30);
    void** handlerVtable = CSF_AT(void**, positionHandler, 0);

    positionData = GetLinkedObjectField10(order, nullptr) + 160;
    return reinterpret_cast<int (__fastcall*)(void*, void*, int, int)>(
        handlerVtable[0xAC / 4])(positionHandler, nullptr, positionData, 0);
}

HOOK(0x004AC9A0, AiOrder_UpdateTargetAndPosition, "auto");
