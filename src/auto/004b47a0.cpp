#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void, __stdcall, AiDistracted_UpdateResolvedEntry, 0x004B3D30, (int arg0, float a3, float a4));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

static int __stdcall AiAlert_UpdateLinkedTasks(C_0085e760* character, float elapsed)
{
    int elapsedBits;
    std::memcpy(&elapsedBits, &elapsed, sizeof(elapsedBits));
    AiCoverAttackTasks_UpdateCharacterTasks(reinterpret_cast<int>(character), elapsedBits);

    int result = IsField10Null(character, nullptr);
    if (static_cast<std::uint8_t>(result) != 0)
        return result;

    auto* resolvedEntry =
        reinterpret_cast<C_0085e760*>(ResolveEntryLookupValue(character, nullptr));
    GetLinkedObjectField10(character, nullptr);

    int nestedEntry = ResolveEntryLookupValue(resolvedEntry, nullptr);
    GetLinkedObjectField10(resolvedEntry, nullptr);
    AiDistracted_UpdateResolvedEntry(nestedEntry, elapsed, elapsed);

    int linkedObject = GetLinkedObjectValue_4DB5C0(character, nullptr);
    result = linkedObject ? linkedObject - 4 : 0;

    if (CSF_AT(int, reinterpret_cast<void*>(result), 0x608) != -1 ||
        CSF_AT(int, reinterpret_cast<void*>(result), 0x60C) != -1)
    {
        linkedObject = GetLinkedObjectValue_4DB5C0(character, nullptr);
        result = linkedObject ? linkedObject - 4 : 0;

        if ((CSF_AT(std::uint8_t, reinterpret_cast<void*>(result), 0x680) & 1) == 0)
        {
            int linkedTask = GetLinkedObjectField10(character, nullptr);
            return AiCoverAttackTasks_ResetLinkedTaskState(character, nullptr, linkedTask + 4);
        }
    }

    return result;
}

HOOK(0x004B47A0, AiAlert_UpdateLinkedTasks, "auto");
