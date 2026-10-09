#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __cdecl, Hud_GetCoordinateScale, 0x0042DE60, (float* a1));
CSF_FUNC(void*, __fastcall, Hud_ExpandEntryStorage, 0x0042FCC0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static const void* __fastcall CHud_InitializeConfiguredWidgets(CHud* self, void* /*edx*/)
{
    self->field_0270 = 0;

    auto* firstSettings = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
    auto* secondSettings = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
    int32_t positionX = CSF_AT(int32_t, secondSettings, 0x9F0);
    int32_t positionY = CSF_AT(int32_t, firstSettings, 0x9F4);
    int32_t position[2] = {0, 0};

    auto* heightSettings = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
    auto* widthSettings = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
    int32_t width = CSF_AT(int32_t, widthSettings, 0x9F8);
    int32_t height = CSF_AT(int32_t, heightSettings, 0x9FC);
    int32_t size[2] = {0, 0};

    void* controls = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    uint32_t panel = reinterpret_cast<uint32_t(__fastcall*)(void*, void*)>(
        CSF_AT(void**, controls, 0)[72 / 4])(controls, nullptr);
    self->field_0268 = panel;

    if (panel)
    {
        position[0] = positionX;
        position[1] = positionY;
        float scale[2] = {0.0f, 0.0f};
        Hud_GetCoordinateScale(scale);
        position[0] = static_cast<int32_t>(
            static_cast<int64_t>(static_cast<double>(position[0]) * scale[0]));
        position[1] = static_cast<int32_t>(
            static_cast<int64_t>(static_cast<double>(position[1]) * scale[1]));

        size[0] = width;
        size[1] = height;
        scale[0] = 0.0f;
        scale[1] = 0.0f;
        Hud_GetCoordinateScale(scale);
        size[0] = static_cast<int32_t>(
            static_cast<int64_t>(static_cast<double>(size[0]) * scale[0]));
        size[1] = static_cast<int32_t>(
            static_cast<int64_t>(static_cast<double>(size[1]) * scale[1]));

        void* widget = reinterpret_cast<void*>(self->field_0268);
        reinterpret_cast<void(__fastcall*)(void*, void*, int32_t*)>(
            CSF_AT(void**, widget, 0)[28 / 4])(widget, nullptr, position);

        widget = reinterpret_cast<void*>(self->field_0268);
        reinterpret_cast<void(__fastcall*)(void*, void*, int32_t*)>(
            CSF_AT(void**, widget, 0)[36 / 4])(widget, nullptr, size);

        float color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        color[0] = CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xA34);
        color[1] = CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xA38);
        color[2] = CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xA3C);

        widget = reinterpret_cast<void*>(self->field_0268);
        reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, widget, 0)[44 / 4])(widget, nullptr, color);
    }

    controls = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    auto* settings = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
    uint32_t textureNameCapacity = CSF_AT(uint32_t, settings, 0x1814);
    auto* textureNameObject = settings + 0x17FC;
    const char* textureName;
    if (textureNameCapacity < 0x10)
        textureName = reinterpret_cast<const char*>(textureNameObject + 4);
    else
        textureName = CSF_AT(const char*, textureNameObject, 4);

    uint32_t texture = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, controls, 0)[80 / 4])(controls, nullptr, textureName);
    self->field_026c = texture;

    if (texture)
    {
        float scale[2] = {0.0f, 0.0f};
        Hud_GetCoordinateScale(scale);
        void* widget = reinterpret_cast<void*>(self->field_026c);
        reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, widget, 0)[152 / 4])(widget, nullptr, scale);
    }

    uint32_t entryCount = self->field_0278;
    if (entryCount < 0x1E)
        return Hud_ExpandEntryStorage(
            &self->field_0274, nullptr, static_cast<int>(30 - entryCount));
    return reinterpret_cast<const void*>(entryCount);
}

HOOK(0x00431D10, CHud_InitializeConfiguredWidgets, "auto");
