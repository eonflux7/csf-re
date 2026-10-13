#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(unsigned int, __cdecl, Hud_WriteNormalizedScreenSize, 0x0042DE60, (float* out));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiControl_Construct_71A170, 0x0071A170, (CGuiControl* self, void* , int owner));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static std::uint32_t* __fastcall CGuiTacticalMap_Construct(
    CGuiTacticalMap* self, void* /*edx*/)
{
    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* stateObject = CSF_AT(void*, globalState, 0x14);
    void* ownerProvider =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, stateObject, 0)[0x54 / 4])(stateObject, nullptr);
    int owner =
        ((int (__fastcall*)(void*, void*))
            CSF_AT(void**, ownerProvider, 0)[0x10 / 4])(ownerProvider, nullptr);

    CGuiControl_Construct_71A170(self, nullptr, owner);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00830A48);

    self->field_0144 = 0;
    self->field_0140 = 0;
    self->field_013c = 0;
    self->field_0138 = 0;
    self->field_0154 = 0;
    self->field_0150 = 0;
    self->field_014c = 0;
    self->field_0148 = 0;
    self->m_sprites = 0;
    self->field_0164 = 0;
    self->field_0168 = 0;
    self->field_016c = 10;
    self->m_mapX = 0;
    self->m_mapY = 0;
    self->m_mapWidth = 0;
    self->m_mapHeight = 0;
    self->field_0158 = 0;
    self->m_medicSprite = 0;
    self->m_spritesLoaded = 0;
    self->field_0174 = 0.0f;
    self->field_017c = static_cast<std::uint32_t>(-1);

    void* globalValues = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());

    // The string occupies the contiguous region from ebp-0x2C to ebp-0x10.
    alignas(8) std::uint8_t textureNameBytes[0x1C];
    CSF_AT(std::uint32_t, textureNameBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, textureNameBytes, 0x14) = 0;
    CSF_AT(std::uint8_t, textureNameBytes, 0x4) = 0;
    AssignStringSubstring(
        textureNameBytes, nullptr,
        static_cast<std::uint8_t*>(globalValues) + 6140,
        0, static_cast<std::uint32_t>(-1));

    void* menuControls = reinterpret_cast<void*>(GetGlobalInt());
    void* textureProvider = MenuControls_ResolveObject(menuControls, nullptr);

    const char* textureName;
    if (CSF_AT(std::uint32_t, textureNameBytes, 0x18) < 0x10)
        textureName = reinterpret_cast<const char*>(textureNameBytes + 0x4);
    else
        textureName = reinterpret_cast<const char*>(
            CSF_AT(std::uint32_t, textureNameBytes, 0x4));

    self->m_mapTexture =
        ((std::uint32_t (__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, textureProvider, 0)[0x50 / 4])(
                textureProvider, nullptr, textureName);

    alignas(8) std::uint8_t screenSizeBytes[0x8];
    CSF_AT(std::uint32_t, screenSizeBytes, 0x0) = 0;
    CSF_AT(std::uint32_t, screenSizeBytes, 0x4) = 0;
    Hud_WriteNormalizedScreenSize(reinterpret_cast<float*>(screenSizeBytes));

    alignas(8) std::uint8_t positionBytes[0x8];
    std::memcpy(positionBytes, screenSizeBytes, 0x8);
    reinterpret_cast<CGuiControl*>(self->m_mapTexture)->vSetPosition(
        reinterpret_cast<const std::int32_t*>(positionBytes));

    self->m_mapX = 145;
    self->m_mapY = 95;
    self->m_mapWidth = 408;
    self->m_mapHeight = 408;

    if (CSF_AT(std::uint32_t, textureNameBytes, 0x18) >= 0x10 &&
        CSF_AT(std::uint32_t, textureNameBytes, 0x4) != 0)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(
            CSF_AT(std::uint32_t, textureNameBytes, 0x4)));
    }

    return reinterpret_cast<std::uint32_t*>(self);
}

HOOK(0x00438010, CGuiTacticalMap_Construct, "auto");
