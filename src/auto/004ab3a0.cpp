#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Character_SetActionMode, 0x00492760, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

static int __stdcall AiMoveTasks_ResetLinkedCharacterAction(void* task)
{
    int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    std::uint8_t* owner = linked
        ? reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(linked)) - 4
        : nullptr;
    CoverAttack_SetStance_492720(CSF_AT(void*, owner, 0x2C), nullptr, 3);

    linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    owner = linked
        ? reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(linked)) - 4
        : nullptr;
    void* character = CSF_AT(void*, owner, 0x2C);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, character, 0)[0x8C / 4])(
            character, nullptr, 0, 0, -1, 0);

    linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    owner = linked
        ? reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(linked)) - 4
        : nullptr;
    character = CSF_AT(void*, owner, 0x2C);
    const int actionMode = CSF_AT(std::uint32_t, character, 0x1C4) ? 0 : 1;

    linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    if (linked) {
        character = CSF_AT(
            void*, reinterpret_cast<void*>(static_cast<std::uintptr_t>(linked)), 0x28);
    } else {
        character = CSF_AT(void*, static_cast<void*>(nullptr), 0x2C);
    }
    return Character_SetActionMode(character, nullptr, actionMode);
}

HOOK(0x004AB3A0, AiMoveTasks_ResetLinkedCharacterAction, "auto");
