#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008445f4_ReleaseStatsDisplay, 0x0054ED50,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008445f4_ResetStatsDisplay, 0x0054EE80,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __stdcall, StoreArgsIntoFetchedSlotAndProcess, 0x0073CC40,
         (int firstValue, int secondValue));
using SpawnSlotPointer = uint32_t* (__stdcall **)(uint32_t*);
CSF_FUNC(SpawnSlotPointer, __cdecl, NetSession_ResolveSpawnVtableSlotPointer,
         0x0073E6B0, ());
CSF_FUNC(C_00867768*, __fastcall, C_00867768_Construct, 0x0073F700,
         (C_00867768* self, void*));
CSF_FUNC(int, __fastcall, SpawnPoints_AttachObjectReference, 0x0073F730,
         (void* self, void*, int a2));
CSF_FUNC(int, __fastcall, MpGameModes_GetStatsReferenceValue, 0x00744580,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB,
         (char* buffer, const char* format, ...));

CSF_GLOBAL(uint8_t, g_StatsSeriesColors, 0x0089A430);
CSF_GLOBAL(uint8_t, g_StatsLegendPositions, 0x0089A480);
CSF_GLOBAL(uint8_t, g_StatsGridColor, 0x0089A4A8);
CSF_GLOBAL(char, g_MaxPlayersSettingValue, 0x008445D8);
CSF_GLOBAL(float, g_StatsRateScale, 0x008445DC);

static int __fastcall C_008445f4_ConstructStatsDisplay(
    C_008445f4* self, void* /*edx*/, int rendererAddress)
{
    alignas(8) uint8_t frameBytes[0x50];
    char* buffer = reinterpret_cast<char*>(frameBytes);
    float* color = reinterpret_cast<float*>(frameBytes + 0x20);
    int* pair28 = reinterpret_cast<int*>(frameBytes + 0x28);
    int* pair20 = reinterpret_cast<int*>(frameBytes + 0x30);
    int* pair18 = reinterpret_cast<int*>(frameBytes + 0x38);
    int* pair10 = reinterpret_cast<int*>(frameBytes + 0x40);

    void* renderer = reinterpret_cast<void*>(rendererAddress);
    bool complete = true;

    C_00867768_Construct(self, nullptr);
    self->vtbl = CSF_VTBL(0x008445F4);
    self->field_07e0 = rendererAddress;
    C_008445f4_ResetStatsDisplay(self, nullptr);

    uint32_t* segment = &CSF_AT(uint32_t, self, 0x7E4);
    for (int row = 98; row != 0; --row)
    {
        for (int column = 5; column != 0; --column)
        {
            void* object =
                ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, renderer, 0)[68 / 4])(renderer, nullptr);
            *segment = reinterpret_cast<uint32_t>(object);
            if (object)
            {
                color[0] = 255.0f;
                color[1] = 0.0f;
                color[2] = 0.0f;
                color[3] = 255.0f;
                ((void (__fastcall*)(void*, void*, const void*))
                    CSF_AT(void**, object, 0)[44 / 4])(object, nullptr, color);
            }
            else
            {
                complete = false;
            }
            ++segment;
        }
    }

    if (!complete)
    {
        C_008445f4_ReleaseStatsDisplay(self, nullptr);
        return reinterpret_cast<int>(self);
    }

    void* object =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, renderer, 0)[72 / 4])(renderer, nullptr);
    self->field_0fb4 = reinterpret_cast<uint32_t>(object);
    if (!object)
    {
        C_008445f4_ReleaseStatsDisplay(self, nullptr);
        return reinterpret_cast<int>(self);
    }

    color[0] = 0.0f;
    color[1] = 0.0f;
    color[2] = 0.0f;
    color[3] = 150.0f;
    ((void (__fastcall*)(void*, void*, const void*))
        CSF_AT(void**, object, 0)[44 / 4])(object, nullptr, color);

    object = reinterpret_cast<void*>(self->field_0fb4);
    pair10[0] = 50;
    pair10[1] = 50;
    ((void (__fastcall*)(void*, void*, const void*))
        CSF_AT(void**, object, 0)[28 / 4])(object, nullptr, pair10);

    object = reinterpret_cast<void*>(self->field_0fb4);
    pair10[0] = 750;
    pair10[1] = 500;
    ((void (__fastcall*)(void*, void*, const void*))
        CSF_AT(void**, object, 0)[36 / 4])(object, nullptr, pair10);

    object =
        ((void* (__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, renderer, 0)[76 / 4])(
                renderer, nullptr, CSF_STR(0x008445E0, "MENUS\\LBoxFon.FNT"));
    self->field_0fb8 = reinterpret_cast<uint32_t>(object);
    if (!object)
    {
        C_008445f4_ReleaseStatsDisplay(self, nullptr);
        return reinterpret_cast<int>(self);
    }

    const uint8_t* seriesColor = &g_StatsSeriesColors;
    const uint8_t* legendPosition = &g_StatsLegendPositions;
    const char* legendText = CSF_STR(0x008444C8, "BPS Enviados");
    uint32_t* label = &CSF_AT(uint32_t, self, 0x103C);
    do
    {
        object =
            ((void* (__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, renderer, 0)[84 / 4])(
                    renderer, nullptr, self->field_0fb8);
        *label = reinterpret_cast<uint32_t>(object);
        if (object)
        {
            ((void (__fastcall*)(void*, void*, const void*))
                CSF_AT(void**, object, 0)[96 / 4])(
                    object, nullptr, seriesColor);
            object = reinterpret_cast<void*>(*label);
            ((void (__fastcall*)(void*, void*, const void*))
                CSF_AT(void**, object, 0)[80 / 4])(
                    object, nullptr, legendPosition);
            object = reinterpret_cast<void*>(*label);
            pair18[0] = 150;
            pair18[1] = 30;
            ((void (__fastcall*)(void*, void*, const void*))
                CSF_AT(void**, object, 0)[88 / 4])(object, nullptr, pair18);
            object = reinterpret_cast<void*>(*label);
            ((void (__fastcall*)(void*, void*, const char*))
                CSF_AT(void**, object, 0)[60 / 4])(object, nullptr, legendText);
        }
        else
        {
            complete = false;
        }
        seriesColor += 16;
        ++label;
        legendPosition += 8;
        legendText += 50;
    }
    while (reinterpret_cast<int32_t>(seriesColor) <
           reinterpret_cast<int32_t>(&g_StatsLegendPositions));

    if (!complete)
    {
        C_008445f4_ReleaseStatsDisplay(self, nullptr);
        return reinterpret_cast<int>(self);
    }

    int tickValue = 0;
    label = &CSF_AT(uint32_t, self, 0xFD0);
    for (int y = 200; y > 50; y -= 30)
    {
        object =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, renderer, 0)[68 / 4])(renderer, nullptr);
        label[-5] = reinterpret_cast<uint32_t>(object);
        if (object)
        {
            object =
                ((void* (__fastcall*)(void*, void*, uint32_t))
                    CSF_AT(void**, renderer, 0)[84 / 4])(
                        renderer, nullptr, self->field_0fb8);
            *label = reinterpret_cast<uint32_t>(object);
            if (object)
            {
                object = reinterpret_cast<void*>(label[-5]);
                pair18[0] = 50;
                pair18[1] = y;
                ((void (__fastcall*)(void*, void*, const void*))
                    CSF_AT(void**, object, 0)[28 / 4])(object, nullptr, pair18);
                object = reinterpret_cast<void*>(label[-5]);
                pair10[0] = 650;
                pair10[1] = y;
                ((void (__fastcall*)(void*, void*, const void*))
                    CSF_AT(void**, object, 0)[36 / 4])(object, nullptr, pair10);
                object = reinterpret_cast<void*>(label[-5]);
                ((void (__fastcall*)(void*, void*, const void*))
                    CSF_AT(void**, object, 0)[44 / 4])(
                        object, nullptr, &g_StatsGridColor);
                object = reinterpret_cast<void*>(*label);
                ((void (__fastcall*)(void*, void*, const void*))
                    CSF_AT(void**, object, 0)[96 / 4])(
                        object, nullptr, &g_StatsGridColor);
                object = reinterpret_cast<void*>(*label);
                pair20[0] = 650;
                pair20[1] = y;
                ((void (__fastcall*)(void*, void*, const void*))
                    CSF_AT(void**, object, 0)[80 / 4])(object, nullptr, pair20);
                object = reinterpret_cast<void*>(*label);
                pair28[0] = 150;
                pair28[1] = 30;
                ((void (__fastcall*)(void*, void*, const void*))
                    CSF_AT(void**, object, 0)[88 / 4])(object, nullptr, pair28);
                _sprintf(buffer, CSF_STR(0x0083090C, "%d"),
                         static_cast<int>(static_cast<int64_t>(
                             static_cast<double>(tickValue) +
                             static_cast<double>(tickValue))));
                object = reinterpret_cast<void*>(*label);
                ((void (__fastcall*)(void*, void*, const char*))
                    CSF_AT(void**, object, 0)[60 / 4])(object, nullptr, buffer);
            }
        }
        else
        {
            complete = false;
        }
        ++label;
        tickValue += 30;
    }

    if (!complete)
    {
        C_008445f4_ReleaseStatsDisplay(self, nullptr);
        return reinterpret_cast<int>(self);
    }

    tickValue = 0;
    label = &CSF_AT(uint32_t, self, 0x1010);
    for (int y = 600; y > 270; y -= 30)
    {
        object =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, renderer, 0)[68 / 4])(renderer, nullptr);
        label[-11] = reinterpret_cast<uint32_t>(object);
        if (object)
        {
            object =
                ((void* (__fastcall*)(void*, void*, uint32_t))
                    CSF_AT(void**, renderer, 0)[84 / 4])(
                        renderer, nullptr, self->field_0fb8);
            *label = reinterpret_cast<uint32_t>(object);
            if (object)
            {
                object = reinterpret_cast<void*>(label[-11]);
                pair28[0] = 50;
                pair28[1] = y;
                ((void (__fastcall*)(void*, void*, const void*))
                    CSF_AT(void**, object, 0)[28 / 4])(object, nullptr, pair28);
                object = reinterpret_cast<void*>(label[-11]);
                pair20[0] = 650;
                pair20[1] = y;
                ((void (__fastcall*)(void*, void*, const void*))
                    CSF_AT(void**, object, 0)[36 / 4])(object, nullptr, pair20);
                object = reinterpret_cast<void*>(label[-11]);
                ((void (__fastcall*)(void*, void*, const void*))
                    CSF_AT(void**, object, 0)[44 / 4])(
                        object, nullptr, &g_StatsGridColor);
                object = reinterpret_cast<void*>(*label);
                ((void (__fastcall*)(void*, void*, const void*))
                    CSF_AT(void**, object, 0)[96 / 4])(
                        object, nullptr, &g_StatsGridColor);
                object = reinterpret_cast<void*>(*label);
                pair18[0] = 650;
                pair18[1] = y;
                ((void (__fastcall*)(void*, void*, const void*))
                    CSF_AT(void**, object, 0)[80 / 4])(object, nullptr, pair18);
                object = reinterpret_cast<void*>(*label);
                pair10[0] = 150;
                pair10[1] = 30;
                ((void (__fastcall*)(void*, void*, const void*))
                    CSF_AT(void**, object, 0)[88 / 4])(object, nullptr, pair10);
                _sprintf(buffer, CSF_STR(0x0083090C, "%d"),
                         static_cast<int>(static_cast<int64_t>(
                             static_cast<double>(tickValue) *
                             static_cast<double>(g_StatsRateScale))));
                object = reinterpret_cast<void*>(*label);
                ((void (__fastcall*)(void*, void*, const char*))
                    CSF_AT(void**, object, 0)[60 / 4])(object, nullptr, buffer);
            }
        }
        else
        {
            complete = false;
        }
        ++label;
        tickValue += 30;
    }

    if (!complete)
    {
        C_008445f4_ReleaseStatsDisplay(self, nullptr);
        return reinterpret_cast<int>(self);
    }

    // Three readout columns, constructed in the original order.
    for (int column = 0; column != 3; ++column)
    {
        int y;
        if (column == 0)
        {
            y = 100;
            label = &CSF_AT(uint32_t, self, 0x1064);
        }
        else if (column == 1)
        {
            y = 120;
            label = &CSF_AT(uint32_t, self, 0x1078);
        }
        else
        {
            y = 140;
            label = &CSF_AT(uint32_t, self, 0x1050);
        }

        seriesColor = &g_StatsSeriesColors;
        do
        {
            object =
                ((void* (__fastcall*)(void*, void*, uint32_t))
                    CSF_AT(void**, renderer, 0)[84 / 4])(
                        renderer, nullptr, self->field_0fb8);
            *label = reinterpret_cast<uint32_t>(object);
            if (object)
            {
                ((void (__fastcall*)(void*, void*, const void*))
                    CSF_AT(void**, object, 0)[96 / 4])(
                        object, nullptr, seriesColor);
                object = reinterpret_cast<void*>(*label);
                pair28[0] = 700;
                pair28[1] = y;
                ((void (__fastcall*)(void*, void*, const void*))
                    CSF_AT(void**, object, 0)[80 / 4])(object, nullptr, pair28);
                object = reinterpret_cast<void*>(*label);
                pair20[0] = 150;
                pair20[1] = 30;
                ((void (__fastcall*)(void*, void*, const void*))
                    CSF_AT(void**, object, 0)[88 / 4])(object, nullptr, pair20);
                object = reinterpret_cast<void*>(*label);
                ((void (__fastcall*)(void*, void*, const char*))
                    CSF_AT(void**, object, 0)[60 / 4])(
                        object, nullptr, &g_MaxPlayersSettingValue);
            }
            else
            {
                complete = false;
            }
            seriesColor += 16;
            ++label;
            y += 100;
        }
        while (reinterpret_cast<int32_t>(seriesColor) <
               reinterpret_cast<int32_t>(&g_StatsLegendPositions));

        if (!complete)
        {
            C_008445f4_ReleaseStatsDisplay(self, nullptr);
            return reinterpret_cast<int>(self);
        }
    }

    GetGlobalDword_73C020();
    SpawnSlotPointer reference = NetSession_ResolveSpawnVtableSlotPointer();
    if (!reference)
    {
        C_008445f4_ReleaseStatsDisplay(self, nullptr);
        return reinterpret_cast<int>(self);
    }

    SpawnPoints_AttachObjectReference(
        self, nullptr, reinterpret_cast<int>(reference));
    self->field_1098 = 1;
    self->field_1099 = 1;
    self->field_109a = 1;

    if (MpGameModes_GetStatsReferenceValue(
            reinterpret_cast<void*>(self->field_0004), nullptr, 2))
    {
        int secondValue = MpGameModes_GetStatsReferenceValue(
            reinterpret_cast<void*>(self->field_0004), nullptr, 1);
        int firstValue = MpGameModes_GetStatsReferenceValue(
            reinterpret_cast<void*>(self->field_0004), nullptr, 0);
        GetGlobalDword_73C020();
        StoreArgsIntoFetchedSlotAndProcess(firstValue, secondValue);
    }
    return reinterpret_cast<int>(self);
}

HOOK(0x0054FA80, C_008445f4_ConstructStatsDisplay, "auto");
