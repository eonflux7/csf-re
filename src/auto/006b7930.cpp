#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpWorldForAllWorldSectors, 0x0058CFF0, (int a1, int (__cdecl* a2)(int*, int), int a3));
CSF_FUNC(char, __fastcall, S_006b6cf0_InitializeWorld, 0x006B6CF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_006b6cf0_RefreshWorld, 0x006B7630, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ParticleInner_Update, 0x006C4330, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30, (C_0085e7b8* self, void*));
CSF_FUNC(int, __fastcall, GfxSkin_AppendObjectRecord, 0x006C8390, (void* self, void*, int objIndex));

[[maybe_unused]] static const auto g_PacketWorldVtable = CSF_VTBL(0x006B6CF0);

static char __fastcall S_006b6cf0_RenderWorld(
    S_006b6cf0* self, void* /*edx*/, std::uint8_t renderMode)
{
    if (!self->field_0010)
        return 0;

    const bool modeChanged = self->field_0025 != renderMode;
    if (modeChanged)
        self->field_0025 = renderMode;

    if (self->field_0024)
    {
        if (modeChanged)
            S_006b6cf0_RefreshWorld(self, nullptr);
    }
    else
    {
        S_006b6cf0_InitializeWorld(self, nullptr);
        RpWorldForAllWorldSectors(
            static_cast<int>(self->field_0010),
            reinterpret_cast<int (__cdecl*)(int*, int)>(
                CSF_ADDR_FN(0x006B7310)),
            reinterpret_cast<int>(self));
        self->field_0024 = 1;
    }

    void* renderer = CSF_AT(
        void*, reinterpret_cast<void*>(GetGlobalValue_6C5760()), 0xF0);

    void* globals = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    GfxSkin_AppendObjectRecord(CSF_AT(void*, globals, 0x4), nullptr, 3);

    std::uint32_t recordOffset = 0;
    for (std::uint32_t index = 0; index < self->field_001c; ++index)
    {
        void* record =
            reinterpret_cast<std::uint8_t*>(self->field_0014) + recordOffset;

        if ((reinterpret_cast<bool (__fastcall*)(void*, void*, void*, int, int)>(
                CSF_AT(void**, renderer, 0)[0x6C / 4]))(
                renderer, nullptr, record, 0, 2))
        {
            ParticleInner_Update(
                CSF_AT(void*,
                    reinterpret_cast<std::uint8_t*>(self->field_0014) + recordOffset,
                    0x10),
                nullptr);
            ParticleInner_Update(
                CSF_AT(void*,
                    reinterpret_cast<std::uint8_t*>(self->field_0014) + recordOffset,
                    0x14),
                nullptr);
        }

        recordOffset += 28;
    }

    globals = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    return GlobalHud_Commit_6C7C30(
        CSF_AT(C_0085e7b8*, globals, 0x4), nullptr);
}

HOOK(0x006B7930, S_006b6cf0_RenderWorld, "auto");
