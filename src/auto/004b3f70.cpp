#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(bool, __fastcall, AiAlert_IsLinkedAlertReady, 0x004AD810, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

static int __stdcall AiAlert_ResetSelectedLinkedTask(int a1, int /*a2*/, int /*a3*/)
{
    void* self = reinterpret_cast<void*>(a1);
    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    int object = linkedValue ? linkedValue - 4 : 0;

    bool selectAlternate =
        CSF_AT(int, reinterpret_cast<void*>(object), 0x608) == -1 &&
        CSF_AT(int, reinterpret_cast<void*>(object), 0x60C) == -1;

    if (!selectAlternate)
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
        object = linkedValue ? linkedValue - 4 : 0;
        if (CSF_AT(int, reinterpret_cast<void*>(object), 0x608) != -1)
        {
            linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
            object = linkedValue ? linkedValue - 4 : 0;
            selectAlternate = AiAlert_IsLinkedAlertReady(
                reinterpret_cast<void*>(object + 0x604), nullptr);
        }
    }

    int linkedTask = GetLinkedObjectField10(self, nullptr);
    return AiCoverAttackTasks_ResetLinkedTaskState(
        self, nullptr, linkedTask + (selectAlternate ? 12 : 8));
}

HOOK(0x004B3F70, AiAlert_ResetSelectedLinkedTask, "auto");
