#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(void, __stdcall, AiCore_AdjustPositionToSceneSurface, 0x004AA6D0, (S_004e0c20* a1));
CSF_FUNC(int, __fastcall, AiCore_CopyCoverAttackState, 0x004D16C0, (void* self, void* , uint32_t* source));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

[[maybe_unused]] static const auto kTaskTargetNameVtable = CSF_VTBL(0x0047AE10);

static int __stdcall AiAlertStates_ConfigureLinkedObject(
    int arg0, uint32_t* coordinates)
{
    void* link = reinterpret_cast<void*>(arg0);

    int linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
    uintptr_t coverObject = linkedValue
        ? static_cast<uintptr_t>(linkedValue) - 4
        : 0;

    linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
    uintptr_t targetObject = linkedValue
        ? static_cast<uintptr_t>(linkedValue) - 4
        : 0;

    auto* targetNameOwner = CSF_AT(
        S_0047ae10*, reinterpret_cast<void*>(targetObject), 0x718);
    auto* coverState = reinterpret_cast<uint32_t*>(coverObject + 0x64C);

    char* targetName = AiCore_GetTaskTargetNameBuffer(targetNameOwner, nullptr);
    AiCore_CopyCoverAttackState(targetName, nullptr, coverState);

    alignas(8) uint8_t positionBytes[0xC];
    S_004e0c20& position =
        *reinterpret_cast<S_004e0c20*>(positionBytes);

    uint32_t x = coordinates[0];
    uint32_t y = coordinates[1];
    uint32_t z = coordinates[2];
    CSF_AT(uint32_t, &position, 0x0) = x;
    CSF_AT(uint32_t, &position, 0x4) = y;
    position.field_0008 = z;

    GetLinkedObjectValue_4DB5C0(link, nullptr);
    AiCore_AdjustPositionToSceneSurface(&position);

    linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
    uintptr_t linkedObject = linkedValue
        ? static_cast<uintptr_t>(linkedValue) - 4
        : 0;

    void* owner = CSF_AT(void*, reinterpret_cast<void*>(linkedObject), 0x2C);
    void* positionController = CSF_AT(void*, owner, 0x30);

    return ((int(__fastcall*)(void*, void*, S_004e0c20*, int))
        CSF_AT(void**, positionController, 0)[0xAC / 4])(
            positionController, nullptr, &position, 0);
}

HOOK(0x004B2030, AiAlertStates_ConfigureLinkedObject, "auto");
