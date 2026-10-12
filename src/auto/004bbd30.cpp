#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AttackTasks_SetMode, 0x0047B210, (void* self, void* , int mode));
CSF_FUNC(int, __fastcall, AiCore_ApplyLinkedPosition, 0x004BAD20, (void* self, void* edx, int a2, char a3, uint32_t* a4, int a5, char a6));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, DispatchWithArgBlock, 0x004DD060, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __stdcall AiDistracted_ApplyLinkedPosition(C_0085e7b8* self)
{
    void* dispatcherOwner = CSF_AT(
        void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FD8);

    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    uint8_t* linkedObject = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;
    void* positionOwner = CSF_AT(void*, linkedObject, 0x2C);

    int linkedField = GetLinkedObjectField10(self, nullptr);

    alignas(8) uint8_t argumentBytes[0x1C];
    uint32_t* arguments = reinterpret_cast<uint32_t*>(argumentBytes);
    arguments[0] = 2;
    arguments[1] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    arguments[2] = CSF_AT(uint32_t, positionOwner, 0x20);
    arguments[3] = CSF_AT(uint32_t, positionOwner, 0x24);

    uint32_t* position = reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(linkedField) + 0x1C);
    arguments[4] = position[0];
    arguments[5] = position[1];
    arguments[6] = position[2];

    DispatchWithArgBlock(
        reinterpret_cast<uint8_t*>(dispatcherOwner) + 0x44,
        nullptr, arguments);

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;
    AttackTasks_SetMode(CSF_AT(void*, linkedObject, 0x718), nullptr, 0);

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;
    UnnamedApply_4DC180(
        self, nullptr, reinterpret_cast<int>(linkedObject + 0x1B0));

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;

    linkedField = GetLinkedObjectField10(self, nullptr);
    position = reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(linkedField) + 0x1C);
    void* provider = GetCachedObjectProvider(self, nullptr);

    return AiCore_ApplyLinkedPosition(
        linkedObject + 0x1B0, nullptr,
        reinterpret_cast<int>(provider), 0, position, 0, 0);
}

HOOK(0x004BBD30, AiDistracted_ApplyLinkedPosition, "auto");
