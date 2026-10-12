#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));

static int __fastcall AiDistracted_InitializeStateTransition(
    void* self, void* /*edx*/, int /*unused*/, int state)
{
    alignas(8) uint8_t record[0x34];
    auto* aiState = static_cast<uint8_t*>(self) + 8;

    CSF_AT(void**, record, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(void**, record, 0x14) = CSF_VTBL(0x0082D448);

    CSF_AT(int, self, 0x25C) = 0;
    CSF_AT(int, self, 0x2A8) = 0;
    CSF_AT(int, self, 0x2D8) = 0;
    CSF_AT(int, self, 0x2F8) = 0;
    CSF_AT(int, self, 0x314) = 0;
    CSF_AT(int, self, 0x330) = 0;
    CSF_AT(int, self, 0x368) = 0;
    CSF_AT(int, self, 0x38C) = 0;
    CSF_AT(int, self, 0x3B8) = 0;

    CSF_AT(void**, record, 0x00) = CSF_VTBL(0x0082D478);
    CSF_AT(int, record, 0x0C) = -1;
    CSF_AT(int, record, 0x10) = -1;
    CSF_AT(int, record, 0x18) = -1;
    CSF_AT(int, record, 0x1C) = -1;
    CSF_AT(int, record, 0x04) = 25;
    CSF_AT(int, record, 0x20) = 0;
    CSF_AT(int, record, 0x24) = 0;
    CSF_AT(int, record, 0x2C) = 0;
    CSF_AT(int, record, 0x28) = 0;
    CSF_AT(char, record, 0x30) = 0;

    int linkedValue = GetLinkedObjectValue_4DB5C0(aiState, nullptr);
    auto* linkedObject = reinterpret_cast<uint8_t*>(
        linkedValue ? static_cast<uintptr_t>(linkedValue) - 4 : 0);
    void* component = CSF_AT(void*, linkedObject, 0x2C);
    CSF_AT(int, record, 0x0C) = CSF_AT(int, component, 0x20);
    CSF_AT(int, record, 0x10) = CSF_AT(int, component, 0x24);

    linkedValue = GetLinkedObjectValue_4DB5C0(aiState, nullptr);
    linkedObject = reinterpret_cast<uint8_t*>(
        linkedValue ? static_cast<uintptr_t>(linkedValue) - 4 : 0);
    if (CSF_AT(int, linkedObject, 0x608) != -1 ||
        CSF_AT(int, linkedObject, 0x60C) != -1)
    {
        CSF_AT(char, record, 0x30) = 1;
    }

    uint8_t* nextState = nullptr;
    switch (state)
    {
    case 0:
    {
        CSF_AT(int, record, 0x24) = CSF_AT(int, self, 0x688);
        linkedValue = GetLinkedObjectValue_4DB5C0(aiState, nullptr);
        linkedObject = reinterpret_cast<uint8_t*>(
            linkedValue ? static_cast<uintptr_t>(linkedValue) - 4 : 0);
        component = CSF_AT(void*, linkedObject, 0x2C);
        void* secondaryObject = static_cast<uint8_t*>(component) + 4;
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, secondaryObject, 0)[0x18 / 4])(
                secondaryObject, nullptr);
        nextState = static_cast<uint8_t*>(self) + 0x30;
        break;
    }
    case 2:
        CSF_AT(int, record, 0x24) = 0;
        nextState = static_cast<uint8_t*>(self) + 0x3C;
        break;
    case 3:
        CSF_AT(int, record, 0x24) = 1;
        nextState = static_cast<uint8_t*>(self) + 0x50;
        break;
    case 4:
        CSF_AT(int, record, 0x24) = 2;
        nextState = static_cast<uint8_t*>(self) + 0x84;
        break;
    case 5:
        CSF_AT(int, record, 0x24) = 3;
        nextState = static_cast<uint8_t*>(self) + 0x10C;
        break;
    case 6:
        CSF_AT(int, record, 0x24) = CSF_AT(int, self, 0x688);
        nextState = static_cast<uint8_t*>(self) + 0x19C;
        break;
    default:
        break;
    }

    if (nextState)
    {
        CoverAttack_SetState_4DC150(
            aiState, nullptr, reinterpret_cast<int>(nextState));
    }

    void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
    return static_cast<int>(
        DispatchRecordToGlobalObject(dispatcher, nullptr, record, 0));
}

HOOK(0x004B9C30, AiDistracted_InitializeStateTransition, "auto");
