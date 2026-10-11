#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpHAnimHierarchyGetMatrixArray, 0x0056CF80, (int a1));
CSF_FUNC(int, __fastcall, BindHierarchyNode, 0x00707590, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6));

static int __fastcall C_00861e98_BindHierarchyNodes(
    C_00861e98* self, void* /*edx*/)
{
    if (!CSF_AT(uint32_t, self, 0x1C))
        return 0;

    const int objectCount =
        CSF_AT(int, CSF_AT(void*, self, 0x14), 0x04);

    int objectIndex = 0;
    for (; objectIndex < objectCount; ++objectIndex)
    {
        void* object =
            CSF_AT(void**, self, 0x4C)[objectIndex];
        if (!object)
            continue;

        const int nodeId =
            ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, object, 0)[0x10 / 4])(object, nullptr);

        const uint32_t hierarchyCount = CSF_AT(uint32_t, self, 0x0C);
        const uint32_t originalHierarchy = CSF_AT(uint32_t, object, 0x0C);
        const bool isHierarchyRoot =
            (CSF_AT(uint32_t,
                    CSF_AT(uint8_t*, self, 0x04) + 36 * originalHierarchy,
                    0x14) & 4) != 0;

        for (uint32_t hierarchyIndex = hierarchyCount; hierarchyIndex != 0;)
        {
            --hierarchyIndex;

            uint8_t* record =
                CSF_AT(uint8_t*, self, 0x04) + 36 * hierarchyIndex;
            const uint32_t sourceHierarchy = CSF_AT(uint32_t, object, 0x0C);

            if (isHierarchyRoot ? hierarchyIndex <= sourceHierarchy
                                : hierarchyIndex == sourceHierarchy)
                continue;
            if ((CSF_AT(uint8_t, record, 0x14) & 4) == 0)
                continue;

            void* hierarchy = CSF_AT(void*, record, 0);
            const int nodeCount = CSF_AT(int, hierarchy, 0x04);
            if (nodeCount <= 0)
                continue;

            uint8_t* node = CSF_AT(uint8_t*, hierarchy, 0x10);
            int nodeIndex = 0;
            while (nodeIndex < nodeCount &&
                   CSF_AT(int, node, 0) != nodeId)
            {
                ++nodeIndex;
                node += 16;
            }
            if (nodeIndex == nodeCount)
                continue;

            uint8_t* animation = CSF_AT(uint8_t*, hierarchy, 0x20);
            uint8_t* transform =
                animation + 76 +
                static_cast<uint32_t>(nodeIndex) *
                    CSF_AT(uint32_t, animation, 0x24);

            const int matrices =
                RpHAnimHierarchyGetMatrixArray(reinterpret_cast<int>(hierarchy));
            const uint32_t matrix =
                static_cast<uint32_t>(matrices) +
                (static_cast<uint32_t>(nodeIndex) << 6);

            BindHierarchyNode(
                object, nullptr,
                static_cast<int>(hierarchyIndex),
                reinterpret_cast<int>(transform + 8),
                reinterpret_cast<int>(transform + 24),
                static_cast<int>(matrix),
                reinterpret_cast<int>(self));

            CSF_AT(uint8_t, object, 0x20) = 1;
            break;
        }
    }
    return objectIndex;
}

HOOK(0x006E9D60, C_00861e98_BindHierarchyNodes, "auto");
