#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetIndexedTableValue_6AF8A0, 0x006AF8A0, (void* self, void* , int index));
CSF_FUNC(char, __fastcall, GrowPositionBuffer, 0x006D64A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008670f0_CalculateTokenPosition, 0x007336B0, (void* self, void* edx, int a2, int a3, uint32_t* a4, float a5));
CSF_FUNC(int, __fastcall, C_008670f0_GetTokenStyle, 0x00733720, (void* self, void* edx, unsigned int a2));
CSF_FUNC(char, __fastcall, GrowLayoutRecords, 0x00734000, (void* self, void* edx, int a2));
CSF_FUNC(wchar_t*, __fastcall, C_008670f0_ContinueLayout, 0x00736250, (void* self, void* edx, float a2));

CSF_GLOBAL(float, g_LayoutEpsilon, 0x008670C8);
CSF_GLOBAL(uint8_t*, g_LayoutTokens, 0x008A88BC);
CSF_GLOBAL(uint32_t, g_LayoutTokenCount, 0x008A88C4);
CSF_GLOBAL(uint8_t, g_ForceNewLayoutLine, 0x008A88CC);
CSF_GLOBAL(uint32_t, g_LayoutLineIndex, 0x008A88D0);
CSF_GLOBAL(float, g_LayoutLineHeight, 0x008A88D4);
CSF_GLOBAL(float, g_LayoutContinuation, 0x008A88D8);

static wchar_t* __fastcall C_008670f0_LayoutTokens(
    C_008670f0* self, void* /*edx*/,
    uint32_t* remainingCount, float* cursor, uint32_t* status)
{
    if (g_ForceNewLayoutLine)
    {
        *status = 0;
        cursor[0] = 0.0f;
        cursor[1] = g_LayoutLineHeight + self->field_00c4 + cursor[1];
        ++g_LayoutLineIndex;
        return C_008670f0_ContinueLayout(self, nullptr, g_LayoutContinuation);
    }

    void* positionOwner = reinterpret_cast<void*>(self->field_0004);
    float lineHeight = g_LayoutLineHeight;
    uint32_t* positions =
        ((uint32_t* (__fastcall*)(void*, void*))
            CSF_AT(void**, positionOwner, 0)[0xAC / 4])(positionOwner, nullptr);

    const int availableWidth = static_cast<int>(self->field_00a0 - self->field_0098);
    const int availableHeight = static_cast<int>(self->field_00a4 - self->field_009c);
    const bool startsAtLeft = cursor[0] < g_LayoutEpsilon;
    bool emitRecords = true;
    float advance = 0.0f;
    uint32_t tokenLimit = g_LayoutTokenCount;

    alignas(8) uint8_t tokenPositionBytes[0x8];
    alignas(8) uint8_t inputPositionBytes[0x40];

    uint32_t tokenIndex = 0;
    uint32_t tokenOffset = 0;
    while (tokenIndex < tokenLimit)
    {
        const int kind = CSF_AT(int, g_LayoutTokens, tokenOffset);
        const int value = CSF_AT(int, g_LayoutTokens, tokenOffset + 4);
        float tokenWidth = 0.0f;

        if (kind == 1)
        {
            int indexedValue = GetIndexedTableValue_6AF8A0(
                reinterpret_cast<void*>(self->field_0008), nullptr, value);
            if (indexedValue)
            {
                void* item = reinterpret_cast<void*>(indexedValue);
                float* width =
                    ((float* (__fastcall*)(void*, void*))
                        CSF_AT(void**, item, 0)[0x40 / 4])(item, nullptr);
                tokenWidth = *width;
            }
        }
        else if (kind == 0 && value)
        {
            tokenWidth = static_cast<float>(
                CSF_AT(int, reinterpret_cast<void*>(value), 0x28)) * self->field_00c0;
        }

        if (startsAtLeft && availableWidth > 0 &&
            static_cast<float>(availableWidth) < tokenWidth + cursor[0] + advance)
        {
            tokenLimit = tokenIndex;
            break;
        }

        if (availableHeight > 0 && cursor[1] >= lineHeight)
        {
            const int nextLineBottom = static_cast<int>(
                static_cast<int64_t>(lineHeight + cursor[1]));
            if (nextLineBottom > availableHeight)
            {
                self->field_00c8 = 1;
                emitRecords = false;
                break;
            }
        }

        bool missingItem = false;
        if (kind == 1)
        {
            missingItem = GetIndexedTableValue_6AF8A0(
                reinterpret_cast<void*>(self->field_0008), nullptr, value) == 0;
        }
        else if (kind == 0)
        {
            missingItem = value == 0;
        }

        if (missingItem)
        {
            const float x = static_cast<float>(static_cast<int>(self->field_0098))
                + (advance + cursor[0]);
            const float y = static_cast<float>(static_cast<int>(self->field_009c))
                + cursor[1];
            if (positions[2] >= positions[1])
                GrowPositionBuffer(positions, nullptr, static_cast<int>(positions[3]));
            const uint32_t index = positions[2];
            float* destination = reinterpret_cast<float*>(positions[0] + 8 * index);
            positions[2] = index + 1;
            destination[0] = x;
            destination[1] = y;
        }
        else
        {
            if (!self->field_00fd &&
                cursor[0] < g_LayoutEpsilon && advance < g_LayoutEpsilon)
            {
                if (CSF_AT(uint32_t, g_LayoutTokens, tokenOffset) == 0 &&
                    *reinterpret_cast<uint16_t*>(
                        CSF_AT(uint32_t, g_LayoutTokens, tokenOffset + 4)) == 32 &&
                    !self->field_00e4)
                {
                    emitRecords = false;
                    break;
                }
            }

            CSF_AT(float, inputPositionBytes, 0) = advance + cursor[0];
            CSF_AT(float, inputPositionBytes, 4) = cursor[1];
            C_008670f0_CalculateTokenPosition(
                self, nullptr,
                reinterpret_cast<int>(tokenPositionBytes),
                reinterpret_cast<int>(inputPositionBytes),
                reinterpret_cast<uint32_t*>(g_LayoutTokens + tokenOffset),
                lineHeight);

            const float x = CSF_AT(float, tokenPositionBytes, 0)
                + static_cast<float>(static_cast<int>(self->field_0098));
            const float y = CSF_AT(float, tokenPositionBytes, 4)
                + static_cast<float>(static_cast<int>(self->field_009c));
            if (positions[2] >= positions[1])
                GrowPositionBuffer(positions, nullptr, static_cast<int>(positions[3]));
            const uint32_t index = positions[2];
            float* destination = reinterpret_cast<float*>(positions[0] + 8 * index);
            advance = tokenWidth + advance;
            positions[2] = index + 1;
            destination[0] = x;
            destination[1] = y;
        }

        ++tokenIndex;
        tokenOffset += 12;
    }

    cursor[0] = advance + cursor[0];

    if (emitRecords)
    {
        uint32_t index = 0;
        uint32_t offset = 0;
        while (index < tokenLimit)
        {
            const int kind = CSF_AT(int, g_LayoutTokens, offset);
            bool missingItem;
            if (kind == 1)
            {
                missingItem = GetIndexedTableValue_6AF8A0(
                    reinterpret_cast<void*>(self->field_0008), nullptr,
                    CSF_AT(int, g_LayoutTokens, offset + 4)) == 0;
            }
            else if (kind == 0)
            {
                missingItem = CSF_AT(uint32_t, g_LayoutTokens, offset + 4) == 0;
            }
            else
            {
                ++index;
                offset += 12;
                continue;
            }

            if (!missingItem)
            {
                const int currentKind = CSF_AT(int, g_LayoutTokens, offset);
                if (currentKind == 0)
                {
                    uint32_t payload[7];
                    payload[0] = CSF_AT(uint32_t, g_LayoutTokens, offset + 4);
                    const uint32_t y = CSF_AT(
                        uint32_t, reinterpret_cast<void*>(positions[0]), 8 * index + 4);
                    payload[1] = CSF_AT(
                        uint32_t, reinterpret_cast<void*>(positions[0]), 8 * index);
                    const unsigned int styleIndex =
                        CSF_AT(uint32_t, g_LayoutTokens, offset + 8);
                    uint32_t* style = reinterpret_cast<uint32_t*>(
                        C_008670f0_GetTokenStyle(self, nullptr, styleIndex));
                    payload[2] = y;
                    payload[3] = style[0];
                    payload[4] = style[1];
                    payload[5] = style[2];
                    payload[6] = style[3];

                    uint32_t record[12];
                    record[2] = 0;
                    record[3] = 0;
                    record[1] = static_cast<uint32_t>(-1);
                    record[0] = 0;
                    std::memcpy(record + 4, payload, 28);
                    record[11] = g_LayoutLineIndex;

                    if (self->field_0060 >= CSF_AT(uint32_t, self, 0x5C))
                    {
                        GrowLayoutRecords(
                            &self->field_0058, nullptr, CSF_AT(int, self, 0x64));
                    }
                    const uint32_t count = self->field_0060;
                    void* destination =
                        reinterpret_cast<void*>(self->field_0058 + 48 * count);
                    self->field_0060 = count + 1;
                    std::memcpy(destination, record, 0x30);
                }
                else if (currentKind == 1)
                {
                    const uint32_t x = CSF_AT(
                        uint32_t, reinterpret_cast<void*>(positions[0]), 8 * index);
                    const uint32_t y = CSF_AT(
                        uint32_t, reinterpret_cast<void*>(positions[0]), 8 * index + 4);
                    uint32_t record[12];
                    record[1] = CSF_AT(uint32_t, g_LayoutTokens, offset + 4);
                    record[2] = x;
                    const uint32_t countBeforeGrowth = CSF_AT(uint32_t, self, 0x70);
                    record[11] = g_LayoutLineIndex;
                    const bool hasRoom = countBeforeGrowth < CSF_AT(uint32_t, self, 0x6C);
                    CSF_AT(float, record, 0x28) = 1.0f;
                    std::memset(record + 4, 0, 24);
                    record[0] = 1;
                    record[3] = y;
                    if (!hasRoom)
                    {
                        GrowLayoutRecords(
                            &self->field_0068, nullptr, CSF_AT(int, self, 0x74));
                    }
                    const uint32_t count = CSF_AT(uint32_t, self, 0x70);
                    void* destination =
                        reinterpret_cast<void*>(self->field_0068 + 48 * count);
                    CSF_AT(uint32_t, self, 0x70) = count + 1;
                    std::memcpy(destination, record, 0x30);
                }
            }
            ++index;
            offset += 12;
        }
    }

    positionOwner = reinterpret_cast<void*>(self->field_0004);
    ((void (__fastcall*)(void*, void*))
        CSF_AT(void**, positionOwner, 0)[0xB0 / 4])(positionOwner, nullptr);

    const uint32_t unprocessed = g_LayoutTokenCount - tokenLimit;
    if (g_LayoutTokenCount != tokenLimit)
    {
        *status = 0;
        *remainingCount -= unprocessed;
        cursor[0] = 0.0f;
        cursor[1] = g_LayoutLineHeight + self->field_00c4 + cursor[1];
        uint32_t continuationBits;
        std::memcpy(&continuationBits, &g_LayoutContinuation, sizeof(continuationBits));
        continuationBits -= unprocessed;
        std::memcpy(&g_LayoutContinuation, &continuationBits, sizeof(continuationBits));
        ++g_LayoutLineIndex;
        return C_008670f0_ContinueLayout(self, nullptr, g_LayoutContinuation);
    }
    return reinterpret_cast<wchar_t*>(unprocessed);
}

HOOK(0x00736800, C_008670f0_LayoutTokens, "auto");
