#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

static int __stdcall AiFncStatic_InitializeGuardState(void* context)
{
    int linkedObject = GetLinkedObjectValue_4DB5C0(context, nullptr);
    void* owner = linkedObject
        ? reinterpret_cast<void*>(static_cast<std::uintptr_t>(linkedObject) - 4)
        : nullptr;

    CoverAttack_SetStance_492720(CSF_AT(void*, owner, 0x2C), nullptr, 3);

    linkedObject = GetLinkedObjectValue_4DB5C0(context, nullptr);
    owner = linkedObject
        ? reinterpret_cast<void*>(static_cast<std::uintptr_t>(linkedObject) - 4)
        : nullptr;

    void* character = CSF_AT(void*, owner, 0x2C);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, character, 0)[0x8C / 4])(
            character, nullptr, 0, 0, -1, 0);

    linkedObject = GetLinkedObjectValue_4DB5C0(context, nullptr);
    owner = linkedObject
        ? reinterpret_cast<void*>(static_cast<std::uintptr_t>(linkedObject) - 4)
        : nullptr;

    void* task = reinterpret_cast<void*>(
        reinterpret_cast<std::uintptr_t>(owner) + 0x2D4);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, task, 0)[0x28 / 4])(task, nullptr, context);

    linkedObject = GetLinkedObjectValue_4DB5C0(context, nullptr);
    owner = linkedObject
        ? reinterpret_cast<void*>(static_cast<std::uintptr_t>(linkedObject) - 4)
        : nullptr;

    CSF_AT(float, owner, 0x2E0) = 1.0f;
    CSF_AT(float, owner, 0x2E4) = 1.0f;

    linkedObject = GetLinkedObjectValue_4DB5C0(context, nullptr);
    owner = linkedObject
        ? reinterpret_cast<void*>(static_cast<std::uintptr_t>(linkedObject) - 4)
        : nullptr;

    if (CSF_AT(int, owner, 0x6A8) == -1)
    {
        linkedObject = GetLinkedObjectValue_4DB5C0(context, nullptr);
        const std::uintptr_t ownerAddress = linkedObject
            ? static_cast<std::uintptr_t>(linkedObject) - 4
            : 0;
        return UnnamedApply_4DC180(
            context, nullptr, static_cast<int>(ownerAddress + 0x23C));
    }

    linkedObject = GetLinkedObjectValue_4DB5C0(context, nullptr);
    const std::uintptr_t ownerAddress = linkedObject
        ? static_cast<std::uintptr_t>(linkedObject) - 4
        : 0;
    return UnnamedApply_4DC180(
        context, nullptr, static_cast<int>(ownerAddress + 0x418));
}

HOOK(0x004AFFF0, AiFncStatic_InitializeGuardState, "auto");
