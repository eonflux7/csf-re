#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, GfxObject_DestroyEmbeddedState, 0x006BF180, (C_00860630* self, void*));
CSF_FUNC(char, __fastcall, GfxFrame_InitRotationDegreesFromMatrix, 0x006BF380, (void* self, void* , int matrixBase));
CSF_FUNC(int, __fastcall, GfxObject_ConstructEmbeddedState, 0x006BFD00, (C_00860700* self, void*));
CSF_FUNC(int, __cdecl, GfxFrames_GetProviderFrameObject, 0x006C0500, (int clump));
CSF_FUNC(int*, __cdecl, GfxFrames_TranslateWrappedAttachment, 0x006C06B0, (int objectIndex, float offsetU, float offsetV));
CSF_FUNC(std::uint32_t, __fastcall, GfxAnimInterpolator_AdvanceAnimation, 0x007086D0, (void* self, void* , float elapsedTime, int , float activity));

static std::uint32_t __fastcall GfxScene_UpdateListedObject(
    void* self, void* /*edx*/, float elapsedTime)
{
    if (CSF_AT(void*, self, 0x0C))
    {
        alignas(8) std::uint8_t stateBytes[0x20];
        auto* state = reinterpret_cast<C_00860700*>(stateBytes);

        GfxObject_ConstructEmbeddedState(state, nullptr);

        int provider = GfxFrames_GetProviderFrameObject(
            CSF_AT(int, self, 0x04));
        GfxFrame_InitRotationDegreesFromMatrix(
            state, nullptr,
            CSF_AT(int, reinterpret_cast<void*>(provider), 0x04));

        GfxAnimInterpolator_AdvanceAnimation(
            CSF_AT(void*, self, 0x0C), nullptr, elapsedTime,
            reinterpret_cast<int>(state), 100.0f);

        void* listedObject = CSF_AT(void*, self, 0x0C);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, listedObject, 0)[0x24 / 4])(
                listedObject, nullptr, 0);

        GfxObject_DestroyEmbeddedState(
            reinterpret_cast<C_00860630*>(stateBytes), nullptr);
    }

    std::uint32_t count = CSF_AT(std::uint32_t, self, 0x1C);
    if (count)
    {
        std::uint32_t index = 0;
        std::uint32_t byteOffset = 0;
        do
        {
            auto* attachments = CSF_AT(std::uint8_t*, self, 0x14);
            auto* attachment = attachments + byteOffset;

            float offsetV = elapsedTime * CSF_AT(float, attachment, 0x08);
            int objectIndex = CSF_AT(int, attachment, 0x00);
            float offsetU = elapsedTime * CSF_AT(float, attachment, 0x04);

            GfxFrames_TranslateWrappedAttachment(objectIndex, offsetU, offsetV);

            count = CSF_AT(std::uint32_t, self, 0x1C);
            ++index;
            byteOffset += 12;
        } while (index < count);
    }
    return count;
}

HOOK(0x006F7490, GfxScene_UpdateListedObject, "auto");
