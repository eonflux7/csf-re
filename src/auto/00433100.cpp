#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, Hud_InitializeDrawRecord, 0x0042E420, (void* self, void* edx));
CSF_FUNC(uint64_t, __cdecl, Hud_ConvertCoordinates, 0x0042F3B0, (int* a1));
CSF_FUNC(void*, __fastcall, Hud_GrowDrawRecords, 0x0042FCC0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Hud_GetOverlayIcon, 0x00431F10, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetEntryCount, 0x0044BB80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetEntryAt, 0x0044BBA0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, DefaultDestination_Get, 0x004FE4A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fHudFadeStep, 0x0082E04C);
CSF_GLOBAL(float, g_fInv255, 0x00830A00);

static float Hud_FloatFromBits(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static uint32_t Hud_FloatToBits(float value)
{
    uint32_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static void CHud_AppendDrawRecord(CHud* self, const uint32_t* record)
{
    if (self->field_027c >= CSF_AT(uint32_t, self, 0x278))
    {
        Hud_GrowDrawRecords(
            reinterpret_cast<uint8_t*>(self) + 0x274, nullptr,
            CSF_AT(int, self, 0x280));
    }

    const uint32_t index = self->field_027c;
    const uint32_t storage = CSF_AT(uint32_t, self, 0x274);
    void* destination = reinterpret_cast<void*>(storage + 60 * index);
    self->field_027c = index + 1;
    std::memcpy(destination, record, 0x3C);
}

static void __fastcall CHud_BuildDebugOverlay(CHud* self, void* /*edx*/)
{
    const float fade = Hud_FloatFromBits(self->field_0270) - g_fHudFadeStep;
    self->field_0270 = Hud_FloatToBits(fade);
    if (fade <= g_fZero)
        self->field_0270 = 0;

    void* manager = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    void* entries = DefaultDestination_Get(manager, nullptr);
    if (!entries)
        return;

    const unsigned int entryCount =
        static_cast<unsigned int>(GetEntryCount(entries, nullptr));
    if (!entryCount)
        return;
    if (Hud_FloatFromBits(self->field_0270) == g_fZero)
        return;
    if (!self->field_0268)
        return;
    if (!self->field_026c)
        return;

    void* background = reinterpret_cast<void*>(self->field_0268);
    void* backgroundColor =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, background, 0)[48 / 4])(background, nullptr);

    uint32_t color[4];
    color[0] = CSF_AT(uint32_t, backgroundColor, 0);
    color[1] = CSF_AT(uint32_t, backgroundColor, 4);
    color[2] = CSF_AT(uint32_t, backgroundColor, 8);
    color[3] = CSF_AT(uint32_t, backgroundColor, 12);

    void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    const float opacity = CSF_AT(float, settings, 2560) * g_fInv255 *
                          Hud_FloatFromBits(self->field_0270);
    color[3] = Hud_FloatToBits(opacity);
    background = reinterpret_cast<void*>(self->field_0268);
    ((void (__fastcall*)(void*, void*, uint32_t*))
        CSF_AT(void**, background, 0)[44 / 4])(background, nullptr, color);

    self->field_027c = 0;

    void* positionSettings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    int left = CSF_AT(int, settings, 2544);
    int y = CSF_AT(int, positionSettings, 2548);

    void* sizeSettings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    const int textWidth = CSF_AT(int, settings, 2552);
    const int textHeight = CSF_AT(int, sizeSettings, 2556);

    void* textWidget = reinterpret_cast<void*>(self->field_026c);
    left += 3;
    y += 3;
    void* font =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, textWidget, 0)[40 / 4])(textWidget, nullptr);
    const int lineHeight =
        ((int (__fastcall*)(void*, void*, int))
            CSF_AT(void**, font, 0)[24 / 4])(font, nullptr, 0);

    uint32_t record[16];
    Hud_InitializeDrawRecord(record, nullptr);

    for (int category = 0; category < 2; ++category)
    {
        bool headingAdded = false;
        for (unsigned int index = 0; index < entryCount; ++index)
        {
            void* entry = reinterpret_cast<void*>(
                GetEntryAt(entries, nullptr, static_cast<int>(index)));
            if (CSF_AT(int, entry, 0) != 0)
                continue;
            if (CSF_AT(int, entry, 56) < 0)
                continue;
            if (CSF_AT(uint8_t, entry, 92) != category)
                continue;

            if (!headingAdded)
            {
                record[5] = 0;
                record[0] = 0;
                int position[2] = {left, y};
                record[14] = 1279413296 + (category != 0);
                Hud_ConvertCoordinates(position);
                int dimensions[2] = {textWidth, textHeight};
                Hud_ConvertCoordinates(dimensions);
                record[10] = position[0];
                record[11] = position[1];
                record[12] = dimensions[0];
                record[13] = dimensions[1];

                settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                const uint32_t red = CSF_AT(uint32_t, settings, 2564);
                settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                const uint32_t green = CSF_AT(uint32_t, settings, 2568);
                settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                const uint32_t blue = CSF_AT(uint32_t, settings, 2572);
                const uint32_t alpha = self->field_0270;
                record[7] = green;
                record[8] = blue;
                record[6] = red;
                record[9] = alpha;
                CHud_AppendDrawRecord(self, record);
                y += lineHeight;
                headingAdded = true;
            }

            void* icon = reinterpret_cast<void*>(
                Hud_GetOverlayIcon(self, nullptr, (category != 0) + 6));
            if (!icon)
                continue;

            record[0] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(icon));
            int position[2] = {left, y};
            Hud_ConvertCoordinates(position);
            const uint32_t iconAlpha = self->field_0270;
            record[1] = position[0];
            record[2] = position[1];
            record[5] = iconAlpha;

            void* iconSize =
                ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, icon, 0)[60 / 4])(icon, nullptr);
            const int iconWidth = CSF_AT(int, iconSize, 0);
            const int iconHeight = CSF_AT(int, iconSize, 4);
            int dimensions[2] = {iconWidth, iconHeight};
            Hud_ConvertCoordinates(dimensions);
            record[4] = dimensions[1];
            record[3] = dimensions[0];

            const char* description;
            if (CSF_AT(uint32_t, entry, 88) < 0x10)
                description = reinterpret_cast<const char*>(entry) + 68;
            else
                description = reinterpret_cast<const char*>(
                    CSF_AT(uint32_t, entry, 68));

            const int textLeft = left + iconWidth + 5;
            record[14] = MenuExtras_ParseDescriptionId(description);
            position[0] = textLeft;
            position[1] = y;
            Hud_ConvertCoordinates(position);
            int textDimensions[2] = {textWidth, textHeight};
            Hud_ConvertCoordinates(textDimensions);
            record[11] = position[1];
            left = -5 - iconWidth + textLeft;

            textWidget = reinterpret_cast<void*>(self->field_026c);
            record[10] = position[0];
            record[12] = textDimensions[0];
            record[13] = textDimensions[1];
            void* textColor =
                ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, textWidget, 0)[100 / 4])(textWidget, nullptr);
            uint32_t red = CSF_AT(uint32_t, textColor, 0);
            uint32_t green = CSF_AT(uint32_t, textColor, 4);
            uint32_t blue = CSF_AT(uint32_t, textColor, 8);
            uint32_t alpha = CSF_AT(uint32_t, textColor, 12);

            const int state = CSF_AT(int, entry, 60);
            if (state == -1)
            {
                settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                red = CSF_AT(uint32_t, settings, 2576);
                settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                green = CSF_AT(uint32_t, settings, 2580);
                settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                blue = CSF_AT(uint32_t, settings, 2584);
            }
            else if (state == 0)
            {
                settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                red = CSF_AT(uint32_t, settings, 2600);
                settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                green = CSF_AT(uint32_t, settings, 2604);
                settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                blue = CSF_AT(uint32_t, settings, 2608);
            }
            else if (state == 1)
            {
                settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                red = CSF_AT(uint32_t, settings, 2588);
                settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                green = CSF_AT(uint32_t, settings, 2592);
                settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                blue = CSF_AT(uint32_t, settings, 2596);
            }

            alpha = self->field_0270;
            record[7] = green;
            record[8] = blue;
            record[6] = red;
            record[9] = alpha;
            CHud_AppendDrawRecord(self, record);
            y += iconHeight;
        }
        y += lineHeight;
    }
}

HOOK(0x00433100, CHud_BuildDebugOverlay, "auto");
