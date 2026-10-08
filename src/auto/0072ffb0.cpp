#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_PrepareDrawing, 0x00719A20,
         (CGuiControl* self, void* edx));

static void __fastcall DrawSwitchComponent(void* component, void* /*edx*/)
{
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, component, 0)[2])(component, nullptr);
}

static int* __fastcall CGuiSwitch_Draw(CGuiSwitch* self, void* /*edx*/)
{
    int* result = ((int*(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0xAC / 4])(self, nullptr);

    if ((reinterpret_cast<std::uintptr_t>(result) & 0xFFu) != 0)
        return result;

    CGuiControl_PrepareDrawing(self, nullptr);

    void* component = CSF_AT(void*, self, 0x140);
    if (component)
        DrawSwitchComponent(component, nullptr);

    const std::uint32_t state = CSF_AT(std::uint32_t, self, 0x144);
    if (state == 0)
    {
        component = CSF_AT(void*, self, 0x134);
        if (component)
            DrawSwitchComponent(component, nullptr);
    }
    else if (state == 1)
    {
        const unsigned char alternate = ((unsigned char(__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0xBC / 4])(self, nullptr);

        if (alternate)
        {
            component = CSF_AT(void*, self, 0x13C);
            if (component)
            {
                DrawSwitchComponent(component, nullptr);
            }
            else
            {
                component = CSF_AT(void*, self, 0x138);
                if (component)
                    DrawSwitchComponent(component, nullptr);
            }
        }
        else
        {
            component = CSF_AT(void*, self, 0x138);
            if (component)
                DrawSwitchComponent(component, nullptr);
        }
    }

    void* overlay = CSF_AT(void*, self, 0x148);
    if (!overlay)
        return nullptr;

    void** overlayVtable = CSF_AT(void**, overlay, 0);
    std::uint32_t positionStorage[2];
    void* position = ((void*(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, self, 0)[0x160 / 4])(self, nullptr, positionStorage);

    ((void(__fastcall*)(void*, void*, void*))
        overlayVtable[0x50 / 4])(
            CSF_AT(void*, self, 0x148), nullptr, position);

    overlay = CSF_AT(void*, self, 0x148);
    overlayVtable = CSF_AT(void**, overlay, 0);
    const std::uint32_t property = ((std::uint32_t(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);

    ((void(__fastcall*)(void*, void*, std::uint32_t))
        overlayVtable[0x58 / 4])(
            CSF_AT(void*, self, 0x148), nullptr, property);

    overlay = CSF_AT(void*, self, 0x148);
    const std::uint32_t* color = ((const std::uint32_t*(__fastcall*)(void*, void*))
        CSF_AT(void**, overlay, 0)[0x64 / 4])(overlay, nullptr);

    std::uint32_t savedColor[4];
    std::uint32_t drawingColor[4];

    const float opacity = CSF_AT(float, self, 0x17C);
    savedColor[0] = color[0];
    const double scaledOpacity =
        static_cast<double>(opacity) *
        static_cast<double>(CSF_AT(float, self, 0x150));
    savedColor[1] = color[1];
    savedColor[2] = color[2];
    savedColor[3] = color[3];

    const float drawingOpacity = static_cast<float>(scaledOpacity);
    std::memcpy(&drawingColor[3], &drawingOpacity, sizeof(drawingOpacity));
    drawingColor[0] = savedColor[0];
    overlay = CSF_AT(void*, self, 0x148);
    drawingColor[1] = savedColor[1];
    drawingColor[2] = savedColor[2];

    ((void(__fastcall*)(void*, void*, const std::uint32_t*))
        CSF_AT(void**, overlay, 0)[0x60 / 4])(
            overlay, nullptr, drawingColor);

    overlay = CSF_AT(void*, self, 0x148);
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, self, 0)[0x238 / 4])(self, nullptr, overlay);

    overlay = CSF_AT(void*, self, 0x148);
    return ((int*(__fastcall*)(void*, void*, const std::uint32_t*))
        CSF_AT(void**, overlay, 0)[0x60 / 4])(
            overlay, nullptr, savedColor);
}

HOOK(0x0072FFB0, CGuiSwitch_Draw, "auto");
