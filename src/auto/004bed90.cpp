#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, TaskCharAction_48CFE0, 0x0048CFE0, (void* self, void* edx, int a2, char a3, char a4));
CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, UnnamedSetRangeParams_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

static int __stdcall AiCore_PrepareLinkedCharacterAction(int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);

    int linkedValue = GetLinkedObjectValue_4DB5C0(context, nullptr);
    uint32_t ownerAddress = linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0;
    void* character = CSF_AT(void*, reinterpret_cast<void*>(ownerAddress), 0x2C);
    CoverAttack_SetStance_492720(character, nullptr, 1);

    linkedValue = GetLinkedObjectValue_4DB5C0(context, nullptr);
    ownerAddress = linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0;
    character = CSF_AT(void*, reinterpret_cast<void*>(ownerAddress), 0x2C);
    ((void(__fastcall*)(void*, void*, int, int, int, int))
        CSF_AT(void**, character, 0)[0x8C / 4])
        (character, nullptr, 0, 0, -1, 0);

    linkedValue = GetLinkedObjectValue_4DB5C0(context, nullptr);
    ownerAddress = linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0;
    UnnamedApply_4DC180(context, nullptr, static_cast<int>(ownerAddress + 0x440));

    linkedValue = GetLinkedObjectValue_4DB5C0(context, nullptr);
    ownerAddress = linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0;
    UnnamedSetRangeParams_4BC3A0(
        reinterpret_cast<void*>(ownerAddress + 0x50C), nullptr, 3.0f, 6.0f);

    linkedValue = GetLinkedObjectValue_4DB5C0(context, nullptr);
    const uint32_t actionOwnerAddress =
        linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0;

    linkedValue = GetLinkedObjectValue_4DB5C0(context, nullptr);
    ownerAddress = linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0;
    character = CSF_AT(void*, reinterpret_cast<void*>(ownerAddress), 0x2C);
    void* task = CSF_AT(void*, character, 0x204);

    return TaskCharAction_48CFE0(
        task, nullptr, static_cast<int>(actionOwnerAddress), 5, 1);
}

HOOK(0x004BED90, AiCore_PrepareLinkedCharacterAction, "auto");
