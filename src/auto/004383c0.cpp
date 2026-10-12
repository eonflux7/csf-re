#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Hud_ScaleRectPositionAndSize, 0x0042F440, (int* rect));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(const char*, g_pszKey_FADE, 0x00899654);
CSF_GLOBAL(const char*, g_pszKey_MAXOPACITY, 0x00899658);
CSF_GLOBAL(const char*, g_pszKey_POS_89965C, 0x0089965C);
CSF_GLOBAL(const char*, g_pszKey_SIZE, 0x00899660);
CSF_GLOBAL(const char*, g_pszKey_FUENTE_899664, 0x00899664);
CSF_GLOBAL(unsigned int, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(unsigned int, g_ScreenHeight, 0x00899BA0);

static std::uint32_t* __fastcall S_004383c0_ConstructQuickLoadSave(
    S_004383c0* self, void* /*edx*/, S_00692b80* definition)
{
    // The packet labels this direct-call address as a vtable as well.
    (void)CSF_VTBL(0x00692B80);

    char fontName[256];
    alignas(8) std::uint8_t scaleBytes[0xC];
    alignas(8) std::uint8_t rectangleBytes[0x10];
    alignas(8) std::uint8_t pointBytes[0x8];
    float* scale = reinterpret_cast<float*>(scaleBytes);
    int* rectangle = reinterpret_cast<int*>(rectangleBytes);
    int* point = reinterpret_cast<int*>(pointBytes);
    auto* properties = reinterpret_cast<std::uint8_t*>(definition);

    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 1065353216;
    fontName[0] = 0;
    self->field_0004 = 0;
    self->field_000c = 0.0f;
    self->field_0008 = 0;
    self->field_0014 = 0;
    self->field_0010 = 0;
    std::memset(fontName + 1, 0, 0xFC);
    fontName[253] = 0;
    fontName[254] = 0;
    self->field_0000 = -1;
    fontName[255] = 0;

    if (FindProperty(properties, nullptr, const_cast<char*>(g_pszKey_FUENTE_899664)))
    {
        MenuExtras_ReadText(properties, nullptr,
                           const_cast<char*>(g_pszKey_FUENTE_899664), fontName, 256);

        void* controls = MenuControls_ResolveObject(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        void* font = ((void*(__fastcall*)(void*, void*, char*))
                         CSF_AT(void**, controls, 0)[80 / 4])(
            controls, nullptr, fontName);
        self->field_0004 = reinterpret_cast<std::uint32_t>(font);

        ((void(__fastcall*)(void*, void*, int))
             CSF_AT(void**, font, 0)[104 / 4])(font, nullptr, 1);

        font = reinterpret_cast<void*>(self->field_0004);
        ((void(__fastcall*)(void*, void*, int))
             CSF_AT(void**, font, 0)[108 / 4])(font, nullptr, 1);

        void* display = LookupObjectById(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        void* dimensions = ((void*(__fastcall*)(void*, void*))
                                CSF_AT(void**, display, 0)[24 / 4])(
            display, nullptr);
        unsigned int width = CSF_AT(unsigned int, dimensions, 0);

        display = LookupObjectById(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        dimensions = ((void*(__fastcall*)(void*, void*))
                          CSF_AT(void**, display, 0)[24 / 4])(display, nullptr);
        unsigned int height = CSF_AT(unsigned int, dimensions, 0x4);

        // The original uses g_UnsignedIntegerBias for unsigned x87 conversion.
        scale[0] = static_cast<float>(width) / static_cast<float>(g_ScreenWidth);
        scale[1] = static_cast<float>(height) / static_cast<float>(g_ScreenHeight);
        scale[2] = 0.0f; // Uninitialized third component in the original.
        font = reinterpret_cast<void*>(self->field_0004);
        ((void(__fastcall*)(void*, void*, float*))
             CSF_AT(void**, font, 0)[152 / 4])(font, nullptr, scale);

        rectangle[0] = 0;
        rectangle[1] = 280;
        rectangle[2] = 800;
        rectangle[3] = 310;
        float fade = 0.5f;
        scale[2] = 2.0f;

        std::uint8_t* property = FindProperty(
            properties, nullptr, const_cast<char*>(g_pszKey_POS_89965C));
        if (property && MenuExtras_GetChildCount(property, nullptr) == 2)
        {
            std::uint32_t first = GetComponent(property, nullptr, 0);
            std::uint32_t second = GetComponent(property, nullptr, 1u);
            int y = ParseIntegerComponent(second);
            rectangle[0] = ParseIntegerComponent(first);
            rectangle[1] = y;
        }

        property = FindProperty(
            properties, nullptr, const_cast<char*>(g_pszKey_SIZE));
        if (property && MenuExtras_GetChildCount(property, nullptr) == 2)
        {
            std::uint32_t first = GetComponent(property, nullptr, 0);
            std::uint32_t second = GetComponent(property, nullptr, 1u);
            int heightValue = ParseIntegerComponent(second);
            int widthValue = ParseIntegerComponent(first);
            rectangle[2] = rectangle[0] + widthValue;
            rectangle[3] = heightValue + rectangle[1];
        }

        Hud_ScaleRectPositionAndSize(rectangle);
        point[1] = rectangle[1];
        point[0] = rectangle[0];
        font = reinterpret_cast<void*>(self->field_0004);
        ((void(__fastcall*)(void*, void*, int*))
             CSF_AT(void**, font, 0)[80 / 4])(font, nullptr, point);

        point[0] = rectangle[2] - rectangle[0];
        point[1] = rectangle[3] - rectangle[1];
        font = reinterpret_cast<void*>(self->field_0004);
        ((void(__fastcall*)(void*, void*, int*))
             CSF_AT(void**, font, 0)[88 / 4])(font, nullptr, point);

        if (FindProperty(properties, nullptr, const_cast<char*>(g_pszKey_FADE)))
            fade = static_cast<float>(ReadFloatProperty(
                properties, nullptr, const_cast<char*>(g_pszKey_FADE)));

        double maximumOpacity;
        if (FindProperty(properties, nullptr, const_cast<char*>(g_pszKey_MAXOPACITY)))
            maximumOpacity = ReadFloatProperty(
                properties, nullptr, const_cast<char*>(g_pszKey_MAXOPACITY));
        else
            maximumOpacity = scale[2];

        self->field_000c = static_cast<float>(maximumOpacity);
        std::memcpy(&self->field_0008, &fade, sizeof(fade));

        font = reinterpret_cast<void*>(self->field_0004);
        auto* color = ((std::uint32_t*(__fastcall*)(void*, void*))
                          CSF_AT(void**, font, 0)[100 / 4])(font, nullptr);
        self->field_0018 = color[0];
        self->field_001c = color[1];
        self->field_0020 = color[2];
        self->field_0024 = color[3];
    }

    return &self->field_0000;
}

HOOK(0x004383C0, S_004383c0_ConstructQuickLoadSave, "auto");
