#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __cdecl, IsGlobal8AFC78NonZero, 0x0042D6E0, ());
CSF_FUNC(int, __fastcall, CMissionScript_Clear, 0x0046B1F0, (CMissionScript* self, void*));
CSF_FUNC(int, __fastcall, ReleaseBufferAndClearCounters, 0x004755C0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0083e124_ConsumeFlagAndNotify, 0x004DEAA0, (C_0083e124* self, void*));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static int __fastcall CScene_CleanupCurrentScene(CScene* self, void* /*edx*/)
{
    auto* optionalSubsystem =
        reinterpret_cast<C_0083e124*>(self->m_optionalSubsys);
    if (optionalSubsystem)
        C_0083e124_ConsumeFlagAndNotify(optionalSubsystem, nullptr);

    CMissionScript_Clear(
        reinterpret_cast<CMissionScript*>(self->m_subsys46ada0), nullptr);
    CMissionScript_Clear(
        CSF_AT(CMissionScript*, reinterpret_cast<void*>(self->field_00c0), 0x004),
        nullptr);
    ReleaseBufferAndClearCounters(
        reinterpret_cast<void*>(self->m_subsys475720), nullptr);

    void* buffer = reinterpret_cast<void*>(self->field_0170);
    self->field_0174 = 0;
    self->field_0178 = 0;
    if (buffer)
    {
        CoreMemory_Free(buffer);
        self->field_0170 = 0;
    }

    buffer = reinterpret_cast<void*>(self->field_00e8);
    self->field_00ec = 0;
    self->field_00f0 = 0;
    if (buffer)
    {
        CoreMemory_Free(buffer);
        self->field_00e8 = 0;
    }

    const int result = IsGlobal8AFC78NonZero();
    if (static_cast<uint8_t>(result))
    {
        const int32_t globalValue = GetGlobal8afc78();
        SceneLoader_NoOp();
        return globalValue;
    }
    return result;
}

HOOK(0x0044FB50, CScene_CleanupCurrentScene, "auto");
