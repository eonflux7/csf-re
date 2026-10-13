#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0041c550_IsField8CNull, 0x0041C550, (void* self, void*));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(uint32_t*, __fastcall, C_0083c280_ResetTask, 0x004D5840, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083c280_ContinueTask, 0x004D5940, (void* self, void* edx));

static int __fastcall C_0083c280_TransitionAfterTargetCheck(
    C_0083c280* self, void* /*edx*/, int /*unused*/)
{
    const bool targetable =
        reinterpret_cast<bool (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xD8 / 4])(self, nullptr);

    if (targetable)
    {
        void* owner = reinterpret_cast<void*>(self->field_0014);
        const int result = C_0041c550_IsField8CNull(
            CSF_AT(void*, owner, 0x4C), nullptr);
        if (static_cast<uint8_t>(result) == 0)
            return result;
    }

    switch (self->field_0070)
    {
    case 0:
    {
        void* owner = reinterpret_cast<void*>(self->field_0014);
        void* controller = CSF_AT(void*, owner, 0x44);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, controller, 0)[0xBC / 4])(
                controller, nullptr, 13);
        break;
    }
    case 1:
    {
        void* owner = reinterpret_cast<void*>(self->field_0014);
        void* controller = CSF_AT(void*, owner, 0x30);
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0xA0 / 4])(
                controller, nullptr);
        break;
    }
    case 3:
        C_0083c280_ResetTask(self, nullptr);
        break;
    }

    if (targetable)
    {
        self->field_0070 = 3;
        return C_0083c280_ContinueTask(self, nullptr);
    }

    self->field_0070 = 6;
    const int result = ResetState(&self->field_001c, nullptr);
    self->field_0018 &= ~1u;
    return result;
}

HOOK(0x004D5E60, C_0083c280_TransitionAfterTargetCheck, "auto");
