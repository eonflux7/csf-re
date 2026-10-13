#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, ReleaseBufferAndResetFields, 0x0044FD70, (void* self, void*));
CSF_FUNC(int, __fastcall, CGuiControl_PushAnimRecord, 0x00450FB0, (CGuiControl* self, void* , const void* record));
CSF_FUNC(const void*, __fastcall, ResizePairBuffer, 0x0045BBA0, (void* self, void* , int extraCount));
CSF_FUNC(char, __fastcall, C_0086c288_ParseBlockIfNonZero, 0x0075ED00, (void* self, void* , uint8_t* a2, int a3));

static char __fastcall C_0083e0d8_ParseAndBuildConnections(
    C_0083e0d8* self, void* /*edx*/, int block, int size)
{
    ReleaseBufferAndResetFields(g_pCurrentScene, nullptr);
    const char result = C_0086c288_ParseBlockIfNonZero(
        self, nullptr, reinterpret_cast<uint8_t*>(block), size);
    if (!result)
        return 0;

    uint32_t begin = self->field_0008;
    const uint32_t nodeCount = begin
        ? static_cast<int32_t>(self->field_000c - begin) / 72
        : 0;

    uint32_t nodeOffset = 0;
    for (uint32_t nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
    {
        auto* node = reinterpret_cast<C_00833908*>(
            self->field_0008 + nodeOffset);
        uint32_t remaining = node->field_002c;
        uint32_t connectionOffset = 0;

        while (remaining)
        {
            auto* connection = reinterpret_cast<uint8_t*>(
                node->field_0024 + connectionOffset);
            const uint32_t nodesBegin = self->field_0008;
            const uint32_t pointsBegin = node->field_0038;
            const int32_t otherNodeIndex = CSF_AT(int32_t, connection, 0x4);
            auto* otherNode = reinterpret_cast<C_00833908*>(
                nodesBegin + 72 * static_cast<uint32_t>(otherNodeIndex));
            const int32_t otherPointIndex = CSF_AT(int32_t, connection, 0x8);

            auto* pairs = reinterpret_cast<uint8_t*>(
                pointsBegin
                + 44 * static_cast<uint32_t>(CSF_AT(int32_t, connection, 0x0))
                + 28);

            if (CSF_AT(uint32_t, pairs, 0x8) >= CSF_AT(uint32_t, pairs, 0x4))
                ResizePairBuffer(pairs, nullptr, CSF_AT(int, pairs, 0xC));

            uint32_t pairCount = CSF_AT(uint32_t, pairs, 0x8);
            auto* entry = reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, pairs, 0x0) + 8 * pairCount);
            CSF_AT(uint32_t, pairs, 0x8) = pairCount + 1;
            CSF_AT(int32_t, entry, 0x0) = otherNodeIndex;
            CSF_AT(int32_t, entry, 0x4) = otherPointIndex;

            // Re-read the connection after the first buffer resize.
            const int32_t pointIndex = CSF_AT(int32_t, connection, 0x0);
            auto* otherPoint = reinterpret_cast<uint8_t*>(
                otherNode->field_0038
                + 44 * static_cast<uint32_t>(CSF_AT(int32_t, connection, 0x8)));
            pairs = otherPoint + 28;

            if (CSF_AT(uint32_t, otherPoint, 0x24) >=
                CSF_AT(uint32_t, otherPoint, 0x20))
            {
                ResizePairBuffer(pairs, nullptr, CSF_AT(int, pairs, 0xC));
            }

            pairCount = CSF_AT(uint32_t, pairs, 0x8);
            entry = reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, pairs, 0x0) + 8 * pairCount);
            CSF_AT(uint32_t, pairs, 0x8) = pairCount + 1;
            CSF_AT(uint32_t, entry, 0x0) = nodeIndex;
            CSF_AT(int32_t, entry, 0x4) = pointIndex;

            connectionOffset += 12;
            --remaining;
        }
        nodeOffset += 72;
    }

    begin = self->field_0008;
    uint32_t remainingNodes = begin
        ? static_cast<int32_t>(self->field_000c - begin) / 72
        : 0;

    nodeOffset = 0;
    while (remainingNodes)
    {
        auto* node = reinterpret_cast<C_00833908*>(
            self->field_0008 + nodeOffset);
        if (node->field_0020 == 4)
        {
            const uint32_t pointsBegin = node->field_0038;
            if (pointsBegin &&
                static_cast<int32_t>(node->field_003c - pointsBegin) / 44 == 4)
            {
                auto* points = reinterpret_cast<uint8_t*>(node->field_0038);
                alignas(8) uint8_t recordBytes[0x54] = {};

                for (uint32_t point = 0; point < 4; ++point)
                {
                    auto* source = points + 44 * point;
                    auto* destination = recordBytes + 12 * point;
                    CSF_AT(uint32_t, destination, 0x0) =
                        CSF_AT(uint32_t, source, 0x4);
                    CSF_AT(uint32_t, destination, 0x4) =
                        CSF_AT(uint32_t, source, 0x8);
                    CSF_AT(uint32_t, destination, 0x8) =
                        CSF_AT(uint32_t, source, 0xC);
                }

                CGuiControl_PushAnimRecord(
                    reinterpret_cast<CGuiControl*>(g_pCurrentScene),
                    nullptr, recordBytes);
            }
        }

        nodeOffset += 72;
        --remainingNodes;
    }
    return result;
}

HOOK(0x004DCB00, C_0083e0d8_ParseAndBuildConnections, "auto");
