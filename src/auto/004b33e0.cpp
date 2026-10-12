#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));

static char __fastcall AiCoverAttackTasks_SetCoverMode(
    void* self, void* /*edx*/, char coverMode)
{
    if (coverMode)
        return coverMode;

    char* targetName = AiCore_GetTaskTargetNameBuffer(
        CSF_AT(S_0047ae10*, self, 0x718), nullptr);
    ResetState(targetName, nullptr);

    void* attackOwner = CSF_AT(void*, self, 0x2C);
    alignas(8) std::uint8_t targetHandleBytes[0xC];
    CSF_AT(std::int32_t, targetHandleBytes, 0x4) = -1;
    CSF_AT(std::int32_t, targetHandleBytes, 0x8) = -1;
    CSF_AT(void**, targetHandleBytes, 0) = CSF_VTBL(0x0082D448);

    return static_cast<char>(AiAttackTasks_CopyTargetHandle(
        CSF_AT(void*, attackOwner, 0x204), nullptr,
        reinterpret_cast<int>(targetHandleBytes)));
}

HOOK(0x004B33E0, AiCoverAttackTasks_SetCoverMode, "auto");
