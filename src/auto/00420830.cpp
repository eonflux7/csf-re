#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HasNonzeroLinkedValueDifferentFrom, 0x0048E680, (void* self, void* , int comparisonValue));
CSF_FUNC(int, __fastcall, TaskState_ProcessDefault, 0x004DB0F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, TaskState_InitializeOnce, 0x004DB140, (void* self, void* edx, int a2));

static char __fastcall C_0082f9f8_EnsureInitAndProcessState(C_0082f9f8* self, void* /*edx*/, int a2)
{
    if (!self->field_0010) {
        int subobjectAddress = static_cast<int>(reinterpret_cast<uintptr_t>((uint8_t*)self + 0x20));
        TaskState_InitializeOnce(self, nullptr, subobjectAddress);
    }

    char result = static_cast<char>(self->field_02b8);
    if (!result) {
        result = static_cast<char>(HasNonzeroLinkedValueDifferentFrom(
            reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0014)), nullptr, reinterpret_cast<int>(self)));
        if (!result) {
            return static_cast<char>(TaskState_ProcessDefault(self, nullptr, a2));
        }
    }
    return result;
}

HOOK(0x00420830, C_0082f9f8_EnsureInitAndProcessState, "auto");
