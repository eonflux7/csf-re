#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, NotifySceneByteChanged, 0x00444940, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, S_00467cc0_ExecuteAction, 0x00467BC0,
         (S_00467cc0* self, void* edx, int16_t* action, C_008313f8* definition));
CSF_FUNC(void*, __cdecl, GetGlobalBlock8AFDA0, 0x0046ED80, ());
CSF_FUNC(int, __stdcall, GetDwordField4, 0x0046ED90, (int a1));

static char __fastcall S_00467cc0_RunActions(
    S_00467cc0* self, void* /*edx*/, unsigned int firstAction)
{
    self->field_0060 = self->field_0008;
    self->field_0064 = self->field_000c;

    void* script = reinterpret_cast<void*>(self->field_0010);
    self->field_0082 = 1;
    self->field_0018 = 1;

    const uint32_t begin = CSF_AT(uint32_t, script, 0x30);
    uint32_t actionCount = 0;
    if (begin) {
        const uint32_t byteCount = CSF_AT(uint32_t, script, 0x34) - begin;
        actionCount = static_cast<uint32_t>(static_cast<int32_t>(byteCount) >> 3);
    }

    uint32_t executed = 0;
    self->field_001c = firstAction;
    bool completed = firstAction >= actionCount;

    while (!completed) {
        if (executed >= 0x3E8)
            break;

        int resolved = 0;
        if ((self->field_000c & 0x80000000) == 0) {
            resolved = CHandleTable_Resolve(
                g_pCurrentScene, nullptr,
                static_cast<int>(reinterpret_cast<uintptr_t>(
                    reinterpret_cast<uint8_t*>(self) + 4)));
        }

        if (resolved) {
            void* object = reinterpret_cast<void*>(static_cast<uint32_t>(resolved));
            if (!self->field_0080 ||
                (CSF_AT(uint8_t, object, 0x170) & 0x10) == 0)
                break;

            void* owner = reinterpret_cast<void*>(CSF_AT(uint32_t, object, 0x34));
            if (owner && (CSF_AT(uint8_t, owner, 0x18) & 2) == 0)
                break;
        } else if (!self->field_0080) {
            break;
        }

        const uint32_t actionAddress =
            CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010), 0x30) +
            8 * self->field_001c;
        int16_t* action = reinterpret_cast<int16_t*>(actionAddress);

        GetGlobalBlock8AFDA0();
        C_008313f8* definition = reinterpret_cast<C_008313f8*>(
            GetDwordField4(static_cast<int>(actionAddress)));

        if (!S_00467cc0_ExecuteAction(self, nullptr, action, definition) ||
            !self->field_0082)
            break;

        const uint32_t nextAction = self->field_001c + 1;
        self->field_001c = nextAction;
        ++executed;
        completed = nextAction >= actionCount;
    }

    if (executed >= 0x3E8) {
        self->field_0078 = 0;
        self->field_0018 = 12;
        return 0;
    }

    if (completed) {
        if (CSF_AT(uint8_t, self, 0x83)) {
            CSF_AT(uint8_t, self, 0x83) = 0;
            void* notificationTarget =
                reinterpret_cast<void*>(CSF_AT(uint32_t, g_pCurrentScene, 0xC0));
            NotifySceneByteChanged(notificationTarget, nullptr, 0);
        }
        self->field_0018 = 0;
    }

    return completed ? 1 : 0;
}

HOOK(0x00467CC0, S_00467cc0_RunActions, "auto");
