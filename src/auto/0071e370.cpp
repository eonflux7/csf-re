#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(char*, g_PositionKeyword, 0x008A85EC);
CSF_GLOBAL(char*, g_SizeKeyword, 0x008A85F0);
CSF_GLOBAL(char*, g_ColorKeyword, 0x008A8628);
CSF_GLOBAL(char*, g_InheritedColorKeyword, 0x008A8784);

CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0,
         (std::uint8_t* node, void* edx, char* keyword));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0,
         (std::uint8_t* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40,
         (std::uint32_t component));
CSF_FUNC(char, __stdcall, ReadFourComponents, 0x0071DA70,
         (std::uint8_t* node, float* components));

static int __fastcall CGuiBuilder_CreateColorQuad(
    CGuiBuilder* self, void* /*edx*/, std::uint8_t* node)
{
    if (!node)
        return 0;

    void* factory = reinterpret_cast<void*>(self->field_0004);
    void* quad = ((void* (__fastcall*)(void*, void*))
                 CSF_AT(void**, factory, 0)[0x48 / 4])(factory, nullptr);

    int position[2] = {0, 0};
    int size[2] = {0, 0};

    std::uint8_t* property = FindProperty(node, nullptr, g_PositionKeyword);
    if (property)
    {
        const std::uint32_t first = GetComponent(property, nullptr, 0);
        const std::uint32_t second = GetComponent(property, nullptr, 1);
        const int y = ParseIntegerComponent(second);
        position[0] = ParseIntegerComponent(first);
        position[1] = y;
        ((void (__fastcall*)(void*, void*, int*))
         CSF_AT(void**, quad, 0)[0x1C / 4])(quad, nullptr, position);
    }

    property = FindProperty(node, nullptr, g_SizeKeyword);
    if (property)
    {
        const std::uint32_t first = GetComponent(property, nullptr, 0);
        const std::uint32_t second = GetComponent(property, nullptr, 1);
        const int height = ParseIntegerComponent(second);
        size[0] = ParseIntegerComponent(first);
        size[1] = height;
        ((void (__fastcall*)(void*, void*, int*))
         CSF_AT(void**, quad, 0)[0x24 / 4])(quad, nullptr, size);
    }

    float color[4];
    if (FindProperty(node, nullptr, g_InheritedColorKeyword))
    {
        color[0] = CSF_AT(float, self, 0x40);
        color[1] = CSF_AT(float, self, 0x44);
        color[2] = CSF_AT(float, self, 0x48);
        color[3] = CSF_AT(float, self, 0x4C);
        ((void (__fastcall*)(void*, void*, float*))
         CSF_AT(void**, quad, 0)[0x2C / 4])(quad, nullptr, color);
    }
    else
    {
        color[0] = 0.0f;
        color[1] = 0.0f;
        color[2] = 0.0f;
        color[3] = 1.0f;
        property = FindProperty(node, nullptr, g_ColorKeyword);
        if (ReadFourComponents(property, color))
        {
            ((void (__fastcall*)(void*, void*, float*))
             CSF_AT(void**, quad, 0)[0x2C / 4])(quad, nullptr, color);
        }
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(quad));
}

HOOK(0x0071E370, CGuiBuilder_CreateColorQuad, "auto");
