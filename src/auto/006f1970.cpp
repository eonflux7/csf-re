#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, CommitOverlayBuffer, 0x006EF050, (void* self, void* edx));
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

static int __fastcall C_00862490_ApplyFadeAlphaAndCommit(C_00862490* self, void* /*edx*/)
{
    // Base fade weight: field_003c * field_0038
    float fadeWeight = self->field_003c * self->field_0038;

    void* globalValue = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_6C5760()));
    float alphaFactor;

    if (CSF_AT(uint8_t, globalValue, 0x134)
        && self->field_0040 != 0
        && reinterpret_cast<bool (__fastcall*)(void*, void*)>(
               CSF_AT(void**, reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0040)), 0)[7])(
               reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0040)), nullptr))
    {
        void* effectObj = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0040));
        alphaFactor = reinterpret_cast<float (__fastcall*)(void*, void*)>(
                          CSF_AT(void**, effectObj, 0)[2])(effectObj, nullptr) * fadeWeight;
    }
    else
    {
        alphaFactor = fadeWeight;
    }

    // Scale and truncate to the alpha byte (signed conversion).
    float scaledAlpha = alphaFactor * g_ColorChannelScale;
    uint8_t alphaByte = static_cast<uint8_t>(static_cast<int>(scaledAlpha));

    // Keep the low 24 bits of the record's colour dword, replace the top byte with alpha.
    void* colorRecord = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0018));
    uint32_t colorWord = CSF_AT(uint32_t, colorRecord, 0x58);
    int packedColor = static_cast<int>((colorWord & 0x00FFFFFFu) | (static_cast<uint32_t>(alphaByte) << 24));

    // Set colour on the overlay object (vtbl slot 0x30), enable it (vtbl slot 0x34 with 1),
    // commit the buffer, then disable it (vtbl slot 0x34 with 0) and return that result.
    void* overlay = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0020));
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, overlay, 0)[12])(
        overlay, nullptr, packedColor);

    overlay = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0020));
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, overlay, 0)[13])(
        overlay, nullptr, 1);

    overlay = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0020));
    CommitOverlayBuffer(overlay, nullptr);

    overlay = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0020));
    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(CSF_AT(void**, overlay, 0)[13])(
        overlay, nullptr, 0);
}

HOOK(0x006F1970, C_00862490_ApplyFadeAlphaAndCommit, "auto");
