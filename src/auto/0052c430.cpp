#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160,
         (void* self, void* edx));
CSF_FUNC(void*, __fastcall, LookupNamedElement, 0x00693360,
         (void* self, void* edx, const char* name));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB,
         (char* buffer, const char* format, ...));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_LayoutWidthAdjustment, 0x00833E44);
CSF_GLOBAL(uint32_t, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(uint32_t, g_ScreenHeight, 0x00899BA0);

static double ConvertUnsignedDimension(uint32_t value)
{
    double converted = static_cast<double>(static_cast<int32_t>(value));
    if (static_cast<int32_t>(value) < 0)
        converted += static_cast<double>(g_UnsignedIntegerBias);
    return converted;
}

static int __fastcall C_008414e8_InitializePrincipalPanel(
    C_008414e8* self, void* /*edx*/, void* screen, void* layoutContext)
{
    uint32_t result = self->field_001c;
    if (result != 0)
        return static_cast<int32_t>(result);

    void* root =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, screen, 0)[0x54 / 4])(screen, nullptr);

    void* identifierContext = CSF_AT(void*, layoutContext, 0);
    void** rootVtable = CSF_AT(void**, root, 0);
    void* identifier = LookupNamedElement(
        identifierContext, nullptr, CSF_STR(0x00841304, ".PANEL_PRINCIPAL"));

    void* panel =
        ((void* (__fastcall*)(void*, void*, void*))
            rootVtable[0x18 / 4])(root, nullptr, identifier);
    result = reinterpret_cast<uint32_t>(panel);
    self->field_001c = result;
    if (result == 0)
        return 0;

    void* display = LookupObjectById(GetGlobalInt(), nullptr);
    void* dimensions =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    uint32_t width = CSF_AT(uint32_t, dimensions, 0);

    display = LookupObjectById(GetGlobalInt(), nullptr);
    dimensions =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    uint32_t height = CSF_AT(uint32_t, dimensions, 4);

    panel = reinterpret_cast<void*>(self->field_001c);
    void** panelVtable = CSF_AT(void**, panel, 0);
    const float heightScale = static_cast<float>(
        ConvertUnsignedDimension(height) /
        ConvertUnsignedDimension(g_ScreenHeight));
    const float widthScale = static_cast<float>(
        ConvertUnsignedDimension(width) /
        ConvertUnsignedDimension(g_ScreenWidth));

    ((void (__fastcall*)(void*, void*, float, float))
        panelVtable[0x150 / 4])(panel, nullptr, widthScale, heightScale);

    display = LookupObjectById(GetGlobalInt(), nullptr);
    dimensions =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    const float positionScale = static_cast<float>(
        static_cast<double>(CSF_AT(int32_t, dimensions, 0)) /
        ConvertUnsignedDimension(g_ScreenWidth));

    panel = reinterpret_cast<void*>(self->field_001c);
    void* positionedChild =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, panel, 0)[0x1CC / 4])(panel, nullptr);
    self->field_0034 = reinterpret_cast<uint32_t>(positionedChild);

    if (positionedChild != nullptr)
    {
        void* originalPosition =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, positionedChild, 0)[0x28 / 4])(
                    positionedChild, nullptr);

        uint32_t position[2];
        position[0] = CSF_AT(uint32_t, originalPosition, 0);
        const double adjustment =
            static_cast<double>(positionScale) *
            static_cast<double>(g_LayoutWidthAdjustment);
        position[1] = CSF_AT(uint32_t, originalPosition, 4);
        const int64_t displacement = static_cast<int64_t>(adjustment);

        positionedChild = reinterpret_cast<void*>(self->field_0034);
        position[0] += static_cast<uint32_t>(displacement);
        self->field_0128 = position[0];

        ((void (__fastcall*)(void*, void*, uint32_t*))
            CSF_AT(void**, positionedChild, 0)[0x24 / 4])(
                positionedChild, nullptr, position);
    }

    panel = reinterpret_cast<void*>(self->field_001c);
    void* secondaryChild =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, panel, 0)[0x1DC / 4])(panel, nullptr);
    self->field_0038 = reinterpret_cast<uint32_t>(secondaryChild);

    for (uint32_t index = 0; index < 4; ++index)
    {
        char childName[2];
        _sprintf(childName, CSF_STR(0x0083090C, "%d"), index);

        panel = reinterpret_cast<void*>(self->field_001c);
        void* child =
            ((void* (__fastcall*)(void*, void*, const char*))
                CSF_AT(void**, panel, 0)[0xFC / 4])(
                    panel, nullptr, childName);
        result = reinterpret_cast<uint32_t>(child);

        switch (index)
        {
        case 0: self->field_0024 = result; break;
        case 1: self->field_0028 = result; break;
        case 2: CSF_AT(uint32_t, self, 0x2C) = result; break;
        case 3: CSF_AT(uint32_t, self, 0x30) = result; break;
        }

        if (child != nullptr)
        {
            void* appearance =
                ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, child, 0)[0x110 / 4])(child, nullptr);
            void* values =
                ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, appearance, 0)[0x64 / 4])(
                        appearance, nullptr);

            const uint32_t destinationOffset = 0x150 + index * 16;
            if (index == 0)
                self->field_0150 = CSF_AT(uint32_t, values, 0);
            else
                CSF_AT(uint32_t, self, destinationOffset) =
                    CSF_AT(uint32_t, values, 0);

            CSF_AT(uint32_t, self, destinationOffset + 4) =
                CSF_AT(uint32_t, values, 4);
            CSF_AT(uint32_t, self, destinationOffset + 8) =
                CSF_AT(uint32_t, values, 8);
            result = CSF_AT(uint32_t, values, 12);
            CSF_AT(uint32_t, self, destinationOffset + 12) = result;
        }
    }

    return static_cast<int32_t>(result);
}

HOOK(0x0052C430, C_008414e8_InitializePrincipalPanel, "auto");
