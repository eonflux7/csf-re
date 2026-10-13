#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

static char __stdcall C_0085e760_HandleLinkedTaskRequest16(C_0085e760* self)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    void* linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    void* owner = CSF_AT(void*, linkedObject, 0x2C);
    void* statusObject = CSF_AT(void*, owner, 0x44);

    if (((bool(__fastcall*)(void*, void*))
             CSF_AT(void**, statusObject, 0)[0x90 / 4])(statusObject, nullptr))
    {
        int entry = ResolveEntryLookupValue(self, nullptr);
        void* taskState = reinterpret_cast<void*>(GetLinkedObjectField10(self, nullptr));

        if (CSF_AT(uint8_t, taskState, 0xB0))
        {
            int entryLinkedValue =
                GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(entry), nullptr);
            void* entryLinkedObject =
                reinterpret_cast<void*>(entryLinkedValue ? entryLinkedValue - 4 : 0);
            void* taskObject = CSF_AT(void*, entryLinkedObject, 0x718);

            ((void(__fastcall*)(void*, void*))
                 CSF_AT(void**, taskObject, 0)[0xA0 / 4])(taskObject, nullptr);
        }

        CSF_AT(uint8_t, reinterpret_cast<void*>(GetLinkedObjectField10(self, nullptr)), 0xB0) = 0;
        CSF_AT(uint8_t, reinterpret_cast<void*>(GetLinkedObjectField10(self, nullptr)), 0xB1) = 0;
        return 0;
    }

    int taskState = GetLinkedObjectField10(self, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, taskState + 0x1C);
    return 1;
}

HOOK(0x004AC8D0, C_0085e760_HandleLinkedTaskRequest16, "auto");
