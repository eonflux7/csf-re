#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MissionScript_GetNodeByIndex, 0x0046F4D0,
         (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, MissionScript_EvaluateEmbeddedExpression, 0x00471F00,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_768E70, 0x00768E70, ());
CSF_FUNC(int, __fastcall, MissionScript_GetNodeDefinition, 0x00769360,
         (void* self, void* edx, unsigned int a2));

static char __fastcall MissionScript_EvaluateNode(
    S_0046f790* self, void* /*edx*/, std::int16_t* node)
{
    // The packet labels the recursive call address as a vtable reference.
    (void)CSF_VTBL(0x0046F790);

    if (node[1] == -1)
    {
        void* expression = CSF_AT(void*, node, 0x4);
        if (expression)
            return MissionScript_EvaluateEmbeddedExpression(expression, nullptr);
    }

    const std::int16_t type = node[0];
    if (type == 0)
        return 0;

    void* registry = reinterpret_cast<void*>(GetGlobalValue_768E70());
    void* definition = reinterpret_cast<void*>(
        MissionScript_GetNodeDefinition(
            registry, nullptr, static_cast<unsigned int>(static_cast<int>(type))));

    if (CSF_AT(int, definition, 0x0) == 4)
        return 1;

    const int childCount = CSF_AT(int, definition, 0x18);
    char satisfied = 1;

    for (int index = 0; index < childCount; ++index)
    {
        if (satisfied)
        {
            const int childIndex = index + node[1];
            void* nodes = CSF_AT(void*, self, 0x6C);
            auto* child = reinterpret_cast<std::int16_t*>(
                MissionScript_GetNodeByIndex(nodes, nullptr, childIndex));
            satisfied = MissionScript_EvaluateNode(self, nullptr, child) != 0;
        }
    }

    return satisfied;
}

HOOK(0x0046F790, MissionScript_EvaluateNode, "auto");
