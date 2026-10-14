#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static void CGuiListBoxBase_DispatchLine(
    CGuiListBoxBase* self, void* start, void* end)
{
    void* line = reinterpret_cast<void*>(self->field_017c);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, line, 0)[0x1C / 4])(line, nullptr, start);

    line = reinterpret_cast<void*>(self->field_017c);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, line, 0)[0x24 / 4])(line, nullptr, end);

    self->vDispatchStateDependentAction(
        reinterpret_cast<void*>(self->field_017c));
}

static char __fastcall CGuiListBoxBase_FinalizeDisplayUpdate(
    CGuiListBoxBase* self, void* /*edx*/)
{
    if (!self->m_bLines)
        return 0;

    alignas(8) uint8_t lineEndBytes[0x8] = {};
    alignas(8) uint8_t lineStartBytes[0x8] = {};
    alignas(8) uint8_t positionBytes[0x8] = {};
    alignas(8) uint8_t cornerBytes[0x8] = {};
    alignas(8) uint8_t oppositeBytes[0xC] = {};

    uint32_t& positionX = CSF_AT(uint32_t, positionBytes, 0);
    uint32_t& positionY = CSF_AT(uint32_t, positionBytes, 4);
    uint32_t& cornerX = CSF_AT(uint32_t, cornerBytes, 0);
    uint32_t& cornerY = CSF_AT(uint32_t, cornerBytes, 4);
    uint32_t& oppositeX = CSF_AT(uint32_t, oppositeBytes, 0);
    uint32_t& oppositeY = CSF_AT(uint32_t, oppositeBytes, 4);
    uint32_t& loopIndex = CSF_AT(uint32_t, oppositeBytes, 8);
    uint32_t& lineStartX = CSF_AT(uint32_t, lineStartBytes, 0);
    uint32_t& lineStartY = CSF_AT(uint32_t, lineStartBytes, 4);
    uint32_t& lineEndX = CSF_AT(uint32_t, lineEndBytes, 0);
    uint32_t& lineEndY = CSF_AT(uint32_t, lineEndBytes, 4);

    self->vGetAbsolutePosition(reinterpret_cast<int*>(positionBytes));
    void* size = self->vGetSize();

    cornerX = positionX + CSF_AT(uint32_t, size, 0);
    cornerY = positionY;
    CGuiListBoxBase_DispatchLine(self, positionBytes, cornerBytes);

    oppositeX = CSF_AT(uint32_t, size, 0) + positionX;
    oppositeY = positionY + CSF_AT(uint32_t, size, 4);
    cornerY = positionY;
    cornerX = oppositeX;
    CGuiListBoxBase_DispatchLine(self, cornerBytes, oppositeBytes);

    uint32_t bottom = CSF_AT(uint32_t, size, 4) + positionY;
    uint32_t right = positionX + CSF_AT(uint32_t, size, 0);
    oppositeX = positionX;
    cornerX = right;
    oppositeY = bottom;
    cornerY = bottom;
    CGuiListBoxBase_DispatchLine(self, cornerBytes, oppositeBytes);

    cornerY = positionY + CSF_AT(uint32_t, size, 4);
    cornerX = positionX;
    CGuiListBoxBase_DispatchLine(self, cornerBytes, positionBytes);

    uint32_t rowY = positionY + self->field_0174;
    uint32_t columnsBegin = self->m_columnsBegin;
    int32_t columnCount = columnsBegin
        ? static_cast<int32_t>(self->m_columnsEnd - columnsBegin) / 12
        : 0;
    uint32_t columnX = positionX;
    int32_t dividerCount = columnCount - 1;

    if (dividerCount > 0)
    {
        loopIndex = 0;
        oppositeY = static_cast<uint32_t>(dividerCount);
        do
        {
            columnX += CSF_AT(
                uint32_t, reinterpret_cast<void*>(self->m_columnsBegin),
                loopIndex);
            lineEndX = columnX;
            lineEndY = positionY + CSF_AT(uint32_t, size, 4);
            lineStartX = lineEndX;
            cornerY = lineEndX;
            lineStartY = rowY;
            CGuiListBoxBase_DispatchLine(self, lineStartBytes, lineEndBytes);

            loopIndex += 12;
            --oppositeY;
            if (oppositeY == 0)
                break;
            columnX = cornerY;
        } while (true);
        columnX = positionX;
    }

    if (static_cast<int32_t>(self->field_0178) > 0)
    {
        lineStartX = CSF_AT(uint32_t, size, 0) + columnX;
        lineEndX = columnX;
        lineStartY = rowY;
        lineEndY = rowY;
        CGuiListBoxBase_DispatchLine(self, lineEndBytes, lineStartBytes);
        rowY += self->field_0178;
    }

    int32_t rowCount =
        reinterpret_cast<int32_t (__fastcall*)(CGuiListBoxBase*, void*)>(
            CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);
    uint32_t result = static_cast<uint32_t>(rowCount);
    int32_t row = static_cast<int32_t>(self->field_0168);
    cornerY = static_cast<uint32_t>(rowCount);
    loopIndex = static_cast<uint32_t>(row);

    if (row < rowCount)
    {
        do
        {
            if (row > static_cast<int32_t>(self->field_0168))
            {
                uint32_t candidate = rowY + self->m_rowHeight + 2 * self->m_lineSpacing;
                uint32_t bottomY = positionY + CSF_AT(uint32_t, size, 4);
                // 7154EF..7154FB: EAX holds bottomY, EDX the candidate.
                result = bottomY;
                if (static_cast<int32_t>(candidate) > static_cast<int32_t>(bottomY))
                    break;
            }

            lineStartX = positionX + CSF_AT(uint32_t, size, 0);
            lineEndX = positionX;
            lineStartY = rowY;
            lineEndY = rowY;
            CGuiListBoxBase_DispatchLine(self, lineEndBytes, lineStartBytes);

            rowY += self->m_rowHeight + 2 * self->m_lineSpacing;
            ++loopIndex;
            result = loopIndex;
            if (static_cast<int32_t>(loopIndex) >= static_cast<int32_t>(cornerY))
                break;
            row = static_cast<int32_t>(loopIndex);
        } while (true);
    }

    return static_cast<char>(result);
}

HOOK(0x00715270, CGuiListBoxBase_FinalizeDisplayUpdate, "auto");
