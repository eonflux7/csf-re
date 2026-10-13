#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void, __fastcall, AiCore_UpdateLinkedTask, 0x004B1420, (void* self, void* edx, int a3, int a4, char a5));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));

static void __stdcall TFncMacroOrder_UpdateCharacterTasks(C_0085e760* character, int updateArg)
{
    AiCoverAttackTasks_UpdateCharacterTasks(
        reinterpret_cast<int>(character), updateArg);

    if (static_cast<std::uint8_t>(IsField10Null(character, nullptr)) == 0)
    {
        const int resolvedEntry = ResolveEntryLookupValue(character, nullptr);
        const int linkedObject = GetLinkedObjectField10(character, nullptr);
        AiCore_UpdateLinkedTask(reinterpret_cast<void*>(linkedObject), nullptr,
            resolvedEntry, updateArg, static_cast<char>(1));
    }
}

HOOK(0x004B1C20, TFncMacroOrder_UpdateCharacterTasks, "auto");
