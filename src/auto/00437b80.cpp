#include <cstdint>
#include <cstddef>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint64_t, __cdecl, ScaleHudSizePair, 0x0042F3B0, (int* sizePair));
CSF_FUNC(char, __fastcall, CGuiTacticalMap_TransformObjectPosition, 0x00436C90, (void* self, void* edx, float* a2));
CSF_FUNC(uint32_t*, __fastcall, CGuiTacticalMap_GetLabelPosition, 0x00436D30, (void* self, void* edx, uint32_t* a2, float* a3));
CSF_FUNC(void, __fastcall, CGuiTacticalMap_PositionObjectSprite, 0x004376E0, (void* self, void* edx, int* a2, float* a3, float a4));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(char, __fastcall, C_GetByte023C, 0x00495490, (char* self, void*));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, InitStringFromLiteral, 0x00692720, (void* returnSlot, const char* text, int count));

CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);
CSF_GLOBAL(float, g_TacticalMapPulsePi, 0x00830A0C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int* __fastcall CGuiTacticalMap_DrawObjectMarkers(
    CGuiTacticalMap* self, void* /*edx*/)
{
    alignas(8) uint8_t colorBytes[0x10];
    float* color = reinterpret_cast<float*>(colorBytes);

    void* sprite = CSF_AT(void*, reinterpret_cast<void*>(self->m_sprites), 0);
    float* originalColor =
        reinterpret_cast<float* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sprite, 0)[0x6C / 4])(sprite, nullptr);
    color[0] = originalColor[0];
    color[1] = originalColor[1];
    color[2] = originalColor[2];
    color[3] = originalColor[3];

    void* objectList = static_cast<uint8_t*>(g_pCurrentScene) + 0x60;
    const uint32_t objectCount = CSF_AT(uint32_t, objectList, 0x08);
    if (objectCount == 0)
        return reinterpret_cast<int*>(objectList);

    // The string occupies the adjacent -0x58 and -0x54 stack regions.
    alignas(8) uint8_t labelBytes[0x1C];
    alignas(8) uint8_t positionBytes[0x0C];
    alignas(8) uint8_t labelPositionBytes[0x08];
    alignas(8) uint8_t scaledPositionBytes[0x18];

    float* position = reinterpret_cast<float*>(positionBytes);
    uint32_t* labelPosition = reinterpret_cast<uint32_t*>(labelPositionBytes);
    int* scaledPosition = reinterpret_cast<int*>(scaledPositionBytes);

    for (uint32_t index = 0; index < objectCount; ++index)
    {
        auto* object = CSF_AT(C_008342e8*,
                             CSF_AT(void*, objectList, 0), index * 4);
        if (!object || !C_GetByte023C(reinterpret_cast<char*>(object), nullptr))
            continue;

        float* worldPosition = GetObjectPositionVector(object, nullptr);
        position[0] = worldPosition[0];
        position[1] = worldPosition[1];
        position[2] = worldPosition[2];
        CGuiTacticalMap_TransformObjectPosition(self, nullptr, position);

        const uint32_t selectedObject = GetSceneMenuContext(g_pCurrentScene, nullptr);
        void* sprites = reinterpret_cast<void*>(self->m_sprites);
        const float scalar =
            static_cast<float>(C_008342e8_ComputeResolvedObjectScalar(object, nullptr));
        CGuiTacticalMap_PositionObjectSprite(
            self, nullptr, CSF_AT(int*, sprites, 0), position, scalar);

        color[3] = 255.0f;
        if (reinterpret_cast<uintptr_t>(object) == selectedObject)
            color[3] = std::sin(self->field_0174 * g_TacticalMapPulsePi)
                       * g_ColorChannelScale;

        sprite = CSF_AT(void*, reinterpret_cast<void*>(self->m_sprites), 0);
        reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, sprite, 0)[0x68 / 4])(sprite, nullptr, color);

        sprite = CSF_AT(void*, reinterpret_cast<void*>(self->m_sprites), 0);
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sprite, 0)[0x08 / 4])(sprite, nullptr);

        void* nameString = reinterpret_cast<uint8_t*>(object->field_002c) + 4;
        CSF_AT(uint32_t, labelBytes, 0) = 0;
        CSF_AT(uint32_t, labelBytes, 0x18) = 7;
        CSF_AT(uint32_t, labelBytes, 0x14) = 0;
        CSF_AT(uint16_t, labelBytes, 0x04) = 0;

        const char* nameText;
        if (CSF_AT(uint32_t, nameString, 0x18) < 0x10u)
            nameText = static_cast<const char*>(nameString) + 4;
        else
            nameText = CSF_AT(const char*, nameString, 0x04);

        InitStringFromLiteral(labelBytes, nameText, 8);
        CGuiTacticalMap_GetLabelPosition(self, nullptr, labelPosition, position);

        void* labelText = CSF_AT(void*, labelBytes, 0x04);
        if (CSF_AT(uint32_t, labelBytes, 0x18) < 8)
            labelText = labelBytes + 0x04;

        void* renderer = reinterpret_cast<void*>(self->m_mapTexture);
        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, renderer, 0)[0x40 / 4])(renderer, nullptr, labelText);

        CSF_AT(uint32_t, scaledPositionBytes, 0) = labelPosition[0] + 10;
        CSF_AT(uint32_t, scaledPositionBytes, 4) = labelPosition[1] - 10;
        ScaleHudSizePair(scaledPosition);

        renderer = reinterpret_cast<void*>(self->m_mapTexture);
        reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
            CSF_AT(void**, renderer, 0)[0x50 / 4])(renderer, nullptr, scaledPosition);

        renderer = reinterpret_cast<void*>(self->m_mapTexture);
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, renderer, 0)[0x08 / 4])(renderer, nullptr);

        if (CSF_AT(uint32_t, labelBytes, 0x18) >= 8)
        {
            void* block = CSF_AT(void*, labelBytes, 0x04);
            if (block)
                Mem_FreeDebug(block);
        }
        CSF_AT(uint32_t, labelBytes, 0x14) = 0;
        CSF_AT(uint16_t, labelBytes, 0x04) = 0;
        CSF_AT(uint32_t, labelBytes, 0x18) = 7;
    }

    return reinterpret_cast<int*>(static_cast<uintptr_t>(objectCount));
}

HOOK(0x00437B80, CGuiTacticalMap_DrawObjectMarkers, "auto");
