#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

static int __stdcall AiAlert_UpdateAndResetLinkedTask(int character, int updateArgument)
{
    AiCoverAttackTasks_UpdateCharacterTasks(character, updateArgument);

    void* self = reinterpret_cast<void*>(character);
    int result = IsField10Null(self, nullptr);
    if (static_cast<std::uint8_t>(result) != 0)
        return result;

    result = GetLinkedObjectValue_4DB5C0(self, nullptr);
    result = result ? result - 4 : 0;
    bool reset = CSF_AT(std::uint8_t, reinterpret_cast<void*>(result), 0x684) != 0;

    if (!reset)
    {
        result = GetLinkedObjectValue_4DB5C0(self, nullptr);
        result = result ? result - 4 : 0;
        if (CSF_AT(int, reinterpret_cast<void*>(result), 0x608) != -1 ||
            CSF_AT(int, reinterpret_cast<void*>(result), 0x60C) != -1)
        {
            result = GetLinkedObjectValue_4DB5C0(self, nullptr);
            result = result ? result - 4 : 0;
            reset = (CSF_AT(std::uint8_t, reinterpret_cast<void*>(result), 0x680) & 1) == 0;
        }
    }

    if (reset)
    {
        int linkedField = GetLinkedObjectField10(self, nullptr);
        return AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedField + 4);
    }
    return result;
}

HOOK(0x004B4240, AiAlert_UpdateAndResetLinkedTask, "auto");
