#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(void, __fastcall, AiCore_UpdateLinkedTask, 0x004B1420, (void* self, void* edx, int a3, int a4, char a5));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, DispatchWithArgBlock, 0x004DD060, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __fastcall AiCore_CheckBoundsAndRefreshLinkedTask(
    void* self, void* /*edx*/, C_0085e760* argument, int updateArgument)
{
    // The virtual query may overwrite its argument, but subsequent operations
    // continue to use the original object.
    C_0085e760* const linkedObject = argument;

    int linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    void* linkedBase = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4 : nullptr;
    void* sceneObject = CSF_AT(void*, linkedBase, 0x2C);
    void* state = CSF_AT(void*, sceneObject, 0x30);

    bool reset = ((bool(__fastcall*)(void*, void*))
        CSF_AT(void**, state, 0)[0xB8 / 4])(state, nullptr);

    if (!reset)
    {
        void* flags = ((void*(__fastcall*)(void*, void*, C_0085e760**))
            CSF_AT(void**, state, 0)[0xBC / 4])(state, nullptr, &argument);
        reset = (CSF_AT(uint32_t, flags, 0) & 2) == 0;
    }

    if (!reset)
    {
        int entry = ResolveEntryLookupValue(linkedObject, nullptr);
        int task = GetLinkedObjectField10(linkedObject, nullptr);
        AiCore_UpdateLinkedTask(
            reinterpret_cast<void*>(task), nullptr, entry, updateArgument, 1);

        linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
        linkedBase = linkedValue
            ? reinterpret_cast<uint8_t*>(linkedValue) - 4 : nullptr;
        void* position = reinterpret_cast<void*>(GetObjectPositionVector(
            CSF_AT(void*, linkedBase, 0x2C), nullptr));

        if (CSF_AT(uint32_t, self, 0x1C) != 2139095039)
        {
            float dx = CSF_AT(float, position, 0) - CSF_AT(float, self, 0x04);
            float dy = CSF_AT(float, position, 4) - CSF_AT(float, self, 0x08);
            float dz = CSF_AT(float, position, 8) - CSF_AT(float, self, 0x0C);
            float radius = CSF_AT(float, self, 0x1C);
            reset = dz * dz + dy * dy + dx * dx > radius * radius;
        }

        if (!reset)
        {
            float dx = CSF_AT(float, self, 0x10) - CSF_AT(float, position, 0);
            float dy = CSF_AT(float, self, 0x14) - CSF_AT(float, position, 4);
            float dz = CSF_AT(float, self, 0x18) - CSF_AT(float, position, 8);
            float radius = CSF_AT(float, self, 0x20);
            reset = radius * radius > dz * dz + dy * dy + dx * dx;
        }
    }

    if (reset)
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
        void* firstBase = linkedValue
            ? reinterpret_cast<uint8_t*>(linkedValue) - 4 : nullptr;

        linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
        void* secondBase = linkedValue
            ? reinterpret_cast<uint8_t*>(linkedValue) - 4 : nullptr;

        void* positionOwner = CSF_AT(void*, firstBase, 0x2C);
        void* handleOwner = CSF_AT(void*, secondBase, 0x2C);
        void* position = reinterpret_cast<void*>(
            GetObjectPositionVector(positionOwner, nullptr));

        alignas(8) uint8_t eventBytes[0x1C];
        uint32_t* event = reinterpret_cast<uint32_t*>(eventBytes);
        event[0] = 1;
        event[1] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
        event[2] = CSF_AT(uint32_t, handleOwner, 0x20);
        event[3] = CSF_AT(uint32_t, handleOwner, 0x24);
        event[4] = CSF_AT(uint32_t, position, 0);
        event[5] = CSF_AT(uint32_t, position, 4);
        event[6] = CSF_AT(uint32_t, position, 8);

        void* manager = reinterpret_cast<void*>(GetValue8B0554());
        DispatchWithArgBlock(
            static_cast<uint8_t*>(CSF_AT(void*, manager, 0x1FD8)) + 0x54,
            nullptr, event);

        int task = GetLinkedObjectField10(linkedObject, nullptr);
        AiCoverAttackTasks_ResetLinkedTaskState(linkedObject, nullptr, task + 4);

        int entry = ResolveEntryLookupValue(linkedObject, nullptr);
        GetCachedObjectProvider(reinterpret_cast<void*>(entry), nullptr);
        GetLinkedObjectField10(linkedObject, nullptr);
    }

    int entry = ResolveEntryLookupValue(linkedObject, nullptr);
    int task = GetLinkedObjectField10(linkedObject, nullptr);
    AiCore_UpdateLinkedTask(
        reinterpret_cast<void*>(task), nullptr, entry, updateArgument, 1);

    // The final callee is void; the original has no defined return value.
    return 0;
}

HOOK(0x004BCCB0, AiCore_CheckBoundsAndRefreshLinkedTask, "auto");
