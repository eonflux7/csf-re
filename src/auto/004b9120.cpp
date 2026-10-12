#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiCore_ConstructStateData, 0x004A9D80, (uint32_t* self, void*));
CSF_FUNC(int, __fastcall, S_004a9e10_CheckNodeContext, 0x004A9E10, (S_004a9e10* self, void* , S_004a9e10* context));
CSF_FUNC(uint32_t*, __fastcall, AiDistracted_GrowPointList, 0x004ADDB0, (void* self, void* edx, int a2));

static int __stdcall AiCombat_CheckTaskNodes(int* nodes, S_004a9e10* context)
{
    // The packet also identifies the direct CheckNodeContext call as a vtable reference.
    (void)CSF_VTBL(0x004A9E10);

    const uint8_t useExistingCount = CSF_AT(uint8_t, context, 0x14);
    const uint8_t reuseNodes = CSF_AT(uint8_t, context, 0x15);

    // In the other mode this starts as an uninitialized out parameter.
    uint32_t nodeCount = 0;
    if (useExistingCount)
        nodeCount = CSF_AT(uint32_t, nodes, 0x8);

    bool rejected = false;
    if (context->pad_0000[0] == 0)
    {
        void* handler = CSF_AT(void*, context, 0x4);
        rejected = !reinterpret_cast<bool(__fastcall*)(
            void*, void*, uint32_t*, S_004a9e10*)>(
                CSF_AT(void**, handler, 0)[0x78 / 4])(
                    handler, nullptr, &nodeCount, context);
    }
    context->pad_0000[0] = static_cast<uint8_t>(rejected);

    if (!rejected)
    {
        if (useExistingCount || reuseNodes)
        {
            uint32_t index = 0;
            uint32_t byteOffset = 0;
            while (index < nodeCount)
            {
                auto* node = reinterpret_cast<S_004a9e10*>(
                    reinterpret_cast<uint8_t*>(
                        static_cast<uintptr_t>(CSF_AT(uint32_t, nodes, 0))) +
                    byteOffset);
                const bool canCheck = context->pad_0000[0] == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (canCheck)
                {
                    CSF_AT(uint8_t, context, 0x15) = reuseNodes;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    S_004a9e10_CheckNodeContext(node, nullptr, context);
                }
                ++index;
                byteOffset += 20;
            }
        }
        else
        {
            const uint32_t capacity = CSF_AT(uint32_t, nodes, 0x4);
            CSF_AT(uint32_t, nodes, 0x8) = 0;
            if (nodeCount > capacity)
            {
                AiDistracted_GrowPointList(
                    nodes, nullptr, static_cast<int>(nodeCount - capacity));
            }

            uint32_t index = 0;
            uint32_t byteOffset = 0;
            alignas(8) uint8_t stateBytes[0x14];
            while (index < nodeCount)
            {
                AiCore_ConstructStateData(
                    reinterpret_cast<uint32_t*>(stateBytes), nullptr);

                if (CSF_AT(uint32_t, nodes, 0x8) >=
                    CSF_AT(uint32_t, nodes, 0x4))
                {
                    AiDistracted_GrowPointList(
                        nodes, nullptr, CSF_AT(int, nodes, 0xC));
                }

                const uint32_t appendIndex = CSF_AT(uint32_t, nodes, 0x8);
                auto* destination = reinterpret_cast<uint8_t*>(
                    static_cast<uintptr_t>(CSF_AT(uint32_t, nodes, 0))) +
                    20 * appendIndex;
                CSF_AT(uint32_t, nodes, 0x8) = appendIndex + 1;

                CSF_AT(uint32_t, destination, 0x0) = CSF_AT(uint32_t, stateBytes, 0x0);
                CSF_AT(uint32_t, destination, 0x4) = CSF_AT(uint32_t, stateBytes, 0x4);
                CSF_AT(uint32_t, destination, 0x8) = CSF_AT(uint32_t, stateBytes, 0x8);
                CSF_AT(uint32_t, destination, 0xC) = CSF_AT(uint32_t, stateBytes, 0xC);
                CSF_AT(uint32_t, destination, 0x10) = CSF_AT(uint32_t, stateBytes, 0x10);

                auto* node = reinterpret_cast<S_004a9e10*>(
                    reinterpret_cast<uint8_t*>(
                        static_cast<uintptr_t>(CSF_AT(uint32_t, nodes, 0))) +
                    byteOffset);
                const bool canCheck = context->pad_0000[0] == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (canCheck)
                {
                    CSF_AT(uint8_t, context, 0x15) = 0;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    S_004a9e10_CheckNodeContext(node, nullptr, context);
                }
                byteOffset += 20;
                ++index;
            }
        }

        if (context->pad_0000[0] == 0)
        {
            void* handler = CSF_AT(void*, context, 0x4);
            if (reinterpret_cast<bool(__fastcall*)(void*, void*, S_004a9e10*)>(
                    CSF_AT(void**, handler, 0)[0x7C / 4])(
                        handler, nullptr, context))
            {
                context->pad_0000[0] = 0;
                return 0;
            }
        }
    }

    context->pad_0000[0] = 1;
    return 1;
}

HOOK(0x004B9120, AiCombat_CheckTaskNodes, "auto");
