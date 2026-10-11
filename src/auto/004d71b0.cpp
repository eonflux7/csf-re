#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetCoverTransition, 0x004D1100, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, TaskCharAction_48CFE0, 0x0048CFE0, (void* self, void* edx, int a2, char a3, char a4));
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

// The original passes the address of an S_0048c9f0 object (whose vtable is this) to
// AiAttackTasks_CopyTargetHandle; this function does not store the vtable itself.
[[maybe_unused]] static const auto s_targetHandleVtbl = CSF_VTBL(0x0048C9F0);

static int __fastcall C_0082f9f8_PrepareAndStartMovement(C_0082f9f8* self, void* /*edx*/) {
    // Flag byte at owner+0x33C set to 1.
    *(reinterpret_cast<uint8_t*>(self->field_0008) + 828) = 1;

    // Global value lookup; its result points to a block holding the cover transition at +0xAE8.
    int globalValue = GetGlobalValue_8BC1E0();
    uint32_t coverTransition = CSF_AT(uint32_t, reinterpret_cast<void*>(globalValue), 0xAE8);
    AiCoverAttackTasks_SetCoverTransition(reinterpret_cast<uint8_t*>(self->field_0008) + 0x330,
                                          nullptr, 0, static_cast<int>(coverTransition));

    // First attack-task call: charTask = owner->field_0014->[0x204].
    void* charTask = CSF_AT(void*, CSF_AT(void*, reinterpret_cast<void*>(self->field_0008), 0x14), 0x204);
    TaskCharAction_48CFE0(charTask, nullptr, static_cast<int>(self->field_0008), 1, 1);

    // Re-read the owner chain after the call, as the original does.
    charTask = CSF_AT(void*, CSF_AT(void*, reinterpret_cast<void*>(self->field_0008), 0x14), 0x204);
    AiAttackTasks_CopyTargetHandle(charTask, nullptr, static_cast<int>(self->field_000c + 120));

    return CharTask_StartMovement(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(&self->field_0014)));
}

HOOK(0x004D71B0, C_0082f9f8_PrepareAndStartMovement, "auto");
