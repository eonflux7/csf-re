#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(bool, __fastcall, C_0083c280_EnterSecondPhase, 0x004D5520, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0083c280_ResetTask, 0x004D5840, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083c280_EnterThirdPhase, 0x004D5940, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0083c280_EnterFourthPhase, 0x004D5B50, (void* self, void* edx));

static char __fastcall C_0083c280_ChangeTaskState(
    C_0083c280* self, void* /*edx*/, int newState)
{
    switch (self->field_0070)
    {
    case 0:
    {
        void* owner = reinterpret_cast<void*>(self->field_0014);
        void* controller = CSF_AT(void*, owner, 0x44);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, controller, 0)[0xBC / 4])(
                controller, nullptr, 13);
        break;
    }
    case 1:
    {
        void* owner = reinterpret_cast<void*>(self->field_0014);
        void* controller = CSF_AT(void*, owner, 0x30);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0xA0 / 4])(
                controller, nullptr);
        break;
    }
    case 3:
        if (newState != 4)
            C_0083c280_ResetTask(self, nullptr);
        break;
    }

    self->field_0070 = static_cast<uint32_t>(newState);

    switch (newState)
    {
    case 1:
    {
        void* owner = reinterpret_cast<void*>(self->field_0014);
        void* controller = CSF_AT(void*, owner, 0x30);
        return reinterpret_cast<char(__fastcall*)(void*, void*, uint32_t*, int)>(
            CSF_AT(void**, controller, 0)[0xAC / 4])(
                controller, nullptr, &CSF_AT(uint32_t, self, 0x78), 1);
    }
    case 2:
        return static_cast<char>(C_0083c280_EnterSecondPhase(self, nullptr));
    case 3:
        return static_cast<char>(C_0083c280_EnterThirdPhase(self, nullptr));
    case 4:
        return C_0083c280_EnterFourthPhase(self, nullptr);
    case 6:
        ResetState(&self->field_001c, nullptr);
        self->field_0018 &= 0xFFFFFFFE;
        return static_cast<char>(self->field_0018);
    default:
        return static_cast<char>(static_cast<uint32_t>(newState) - 1);
    }
}

HOOK(0x004D5D90, C_0083c280_ChangeTaskState, "auto");
