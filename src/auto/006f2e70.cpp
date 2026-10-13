#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, GfxObject_UpdateAttachment, 0x006E50C0, (float* self, void* , float deltaTime));
CSF_FUNC(int, __fastcall, C_00862640_ApplyFrameIndex, 0x006F2CC0, (void* self, void* edx, int a2));

static int __fastcall C_00862640_AdvanceFrameTimer(
    C_00862640* self, void* /*edx*/, float deltaTime)
{
    // Preserve the full return register, although only its low byte is tested.
    int flagResult =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x34 / 4])(self, nullptr);
    if (static_cast<uint8_t>(flagResult) != 0)
        return flagResult;

    if (self->field_0048)
        GfxObject_UpdateAttachment(
            reinterpret_cast<float*>(self->field_0048), nullptr, deltaTime);

    double accumulated = static_cast<double>(deltaTime) + self->field_003c;
    self->field_003c = static_cast<float>(accumulated);

    // __ftol2 produces a signed 64-bit integer; this function uses its low dword.
    int64_t wholeSteps = static_cast<int64_t>(accumulated / self->field_0040);
    int32_t steps = static_cast<int32_t>(static_cast<uint32_t>(wholeSteps));
    if (steps <= 0)
        return steps;

    int32_t nextIndex = static_cast<int32_t>(
        self->field_0044 + static_cast<uint32_t>(steps));
    int loopResult = self->vHasBit1OfField0014();

    if (static_cast<uint8_t>(loopResult) != 0 ||
        nextIndex < CSF_AT(int32_t,
                          reinterpret_cast<void*>(self->field_0018), 0x70))
    {
        int32_t wrappedIndex =
            nextIndex % CSF_AT(int32_t,
                               reinterpret_cast<void*>(self->field_0018), 0x70);
        self->field_0044 = static_cast<uint32_t>(wrappedIndex);
        int result = C_00862640_ApplyFrameIndex(self, nullptr, wrappedIndex);
        self->field_003c = static_cast<float>(
            self->field_003c - static_cast<double>(steps) * self->field_0040);
        return result;
    }

    // This nominally void slot leaves the function's observable return in eax.
    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, self, 0)[0x2C / 4])(self, nullptr, 1);
}

HOOK(0x006F2E70, C_00862640_AdvanceFrameTimer, "auto");
