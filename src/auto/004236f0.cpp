#include <cstdint>
#include "globals.h"
#include <cstring>

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4DB0F0, 0x004DB0F0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_422FA0, 0x00422FA0, (void* self, void* edx, float a2));
CSF_FUNC(void**, __fastcall, sub_423190, 0x00423190, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4DB440, 0x004DB440, (void* self, void* edx, int a2));

// Reinterprets the raw stack dword as a float (the original passes it as *(float*)&a2).
static float RawDwordToFloat(int raw) {
    float value;
    std::memcpy(&value, &raw, sizeof(value));
    return value;
}

// Runs the pre-step, tests the float-gated check, and if it fails resets the
// state object and schedules the follow-up using the stored offset + 104.
static char __fastcall TCharTask_RunStateStep(TCharTask* self, void* /*edx*/, int a2) {
    sub_4DB0F0(self, nullptr, a2);

    char result = sub_422FA0(reinterpret_cast<S_00421120*>(self), nullptr,
                             RawDwordToFloat(a2));
    if (result == 0) {
        sub_423190(reinterpret_cast<C_00834fa8*>(self), nullptr);
        int stateOffset = CSF_AT(int, self, 0x0C) + 0x68;
        return static_cast<char>(
            sub_4DB440(reinterpret_cast<C_00834fa8*>(self), nullptr, stateOffset));
    }
    return result;
}

HOOK(0x004236F0, TCharTask_RunStateStep, "auto");
