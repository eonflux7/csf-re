#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_00833834_Construct, 0x0048CF30, (C_00833834* self, void* , int a2));
CSF_FUNC(std::uint32_t*, __fastcall, C_00833908_Construct, 0x00490940, (C_00833908* self, void* , int objectId, int stateCode));
CSF_FUNC(int, __fastcall, C_00833b88_InitializeMode, 0x004925E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00833b88_RefreshState, 0x00492D70, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);
CSF_GLOBAL(float, g_DefaultCharacterValue, 0x00833C54);

static int __fastcall C_00833b88_Construct(
    C_00833b88* self, void* /*edx*/, int objectId, int stateCode)
{
    C_00833908_Construct(self, nullptr, objectId, stateCode);

    self->vtbl = CSF_VTBL(0x00833B88);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00833B08));
    CSF_AT(void*, self, 0x008) = CSF_VTBL(0x00833B04);
    self->field_000c = reinterpret_cast<uint32_t>(CSF_VTBL(0x00833AF8));
    CSF_AT(void*, self, 0x010) = CSF_VTBL(0x00833AF0);
    self->field_0014 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00833AEC));

    self->field_018c = 0;
    self->field_0190 = 0;
    self->field_0194 = 0;
    self->field_0198 = 0;
    self->field_019c = 0;
    self->field_01a0 = 0;
    self->field_01ac = 0;
    self->field_01b0 = 0;
    CSF_AT(void*, self, 0x1B4) = CSF_VTBL(0x0082D448);
    self->field_01b8 = -1;
    self->field_01bc = -1;
    self->field_01c8 = 0;
    self->field_01cc = 0;
    self->field_01d0 = 0;
    self->field_01e4 = 1;
    self->field_01e8 = -1;
    self->field_01ec = 100;
    self->field_01f0 = 0;
    self->field_01c4 = 0;

    C_00833b88_InitializeMode(self, nullptr, 0);

    float initialValue = CSF_AT(
        float, reinterpret_cast<void*>(self->field_002c), 0xE4);
    if (!(initialValue > g_fZero))
        initialValue = self->field_01c4 ? g_MaxPercent : g_DefaultCharacterValue;

    const uint32_t pendingFirst = self->field_01d8;
    self->field_01d4 = initialValue;
    void* secondaryInterface = &self->field_0004;
    if (pendingFirst)
    {
        void* target = CSF_AT(void**, secondaryInterface, 0)[0x34 / 4];
        self->field_01d8 = 0;
        reinterpret_cast<void(__fastcall*)(void*, void*)>(target)(
            secondaryInterface, nullptr);
    }

    const bool pendingSecond = self->field_01e0 != 0;
    self->field_01dc = 0;
    if (pendingSecond)
    {
        void* target = CSF_AT(void**, secondaryInterface, 0)[0x38 / 4];
        self->field_01e0 = 0;
        reinterpret_cast<void(__fastcall*)(void*, void*)>(target)(
            secondaryInterface, nullptr);
    }

    self->field_0184 = 0;
    const float initialScale = 1.0f;
    std::memcpy(&self->field_0188, &initialScale, sizeof(initialScale));
    self->field_01c0 = 0;
    self->field_01f4 = -1;
    self->field_01f8 = -1;
    self->field_01fc = 0.0f;
    self->field_0200 = 0;

    auto* attachment = reinterpret_cast<C_00833834*>(
        CoreMemory_AllocUntagged(64));
    uint32_t* constructedAttachment = nullptr;
    if (attachment)
        constructedAttachment = C_00833834_Construct(
            attachment, nullptr, reinterpret_cast<int>(self));

    self->field_0204 = reinterpret_cast<uint32_t>(constructedAttachment);
    self->field_0208 = static_cast<uint16_t>(-1);

    const uint32_t clearedFlags = self->field_01f0 & static_cast<uint32_t>(-11);
    self->field_01f0 &= ~2u;
    self->field_01f0 = clearedFlags;
    self->field_01f0 = clearedFlags | 4;

    C_00833b88_RefreshState(self, nullptr);
    return reinterpret_cast<int>(self);
}

HOOK(0x00492F60, C_00833b88_Construct, "auto");
