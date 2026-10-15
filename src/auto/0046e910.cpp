#include <cstdint>
#include <cstring>
#ifdef CSF_STANDALONE
void csf_script_invalid_leaf(const char*, unsigned, unsigned, void*, int,
                             void*, unsigned, void*, const uint32_t*);
#endif
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MissionScript_ParseExpressionValue, 0x0046E590,
         (void* self, void* edx, void* a2, uint32_t* a3, int a4));
CSF_FUNC(int, __fastcall, MissionScript_GetExpressionChild, 0x0046F540,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, MissionScript_FinalizeExpressionChild, 0x0046F970,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, MissionScript_AllocateExpressionChildren, 0x00471060,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MissionScript_GetNodeType, 0x006927F0,
         (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0,
         (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40,
         (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, GetGlobalValue_768E70, 0x00768E70, ());
CSF_FUNC(int, __fastcall, MissionScript_FindExpressionDefinition, 0x007692F0,
         (void* self, void* edx, char* String1));

static int __fastcall S_0046c260_ParseExpression(
    S_0046c260* self, void* /*edx*/, S_00692b80* node, int expression)
{
    char attributeText[0x108];
    std::memset(attributeText, 0, 256);

    int result = MissionScript_GetNodeType(node, nullptr);
    if (result != 1)
        return result;

    const std::uint32_t attribute =
        GetComponent(reinterpret_cast<unsigned char*>(node), nullptr, 0);
    CopyAttributeText(reinterpret_cast<std::uint8_t*>(attribute),
                      nullptr, attributeText, 256);

    const int definitions = GetGlobalValue_768E70();
    auto* definition = reinterpret_cast<std::uint32_t*>(
        MissionScript_FindExpressionDefinition(
            reinterpret_cast<void*>(definitions), nullptr, attributeText));

    result = MissionScript_ParseExpressionValue(
        self, nullptr, node, definition, expression);

    void* expressionData = reinterpret_cast<void*>(expression);
    if (CSF_AT(std::uint16_t, expressionData, 0x2) == 0xFFFF ||
        CSF_AT(std::uint32_t, definition, 0x0) == 4)
        return result;

    const std::uint32_t childCount =
        CSF_AT(std::uint32_t, definition, 0x18);
    if (childCount == 0)
        return result;

    CSF_AT(std::uint16_t, expressionData, 0x2) =
        static_cast<std::uint16_t>(
            MissionScript_AllocateExpressionChildren(
                reinterpret_cast<void*>(self->field_0044),
                nullptr, static_cast<int>(childCount)));

    std::uint32_t childIndex = 0;
    do
    {
        const std::uint32_t nextIndex = childIndex + 1;
        auto* childNode = reinterpret_cast<S_00692b80*>(
            GetComponent(reinterpret_cast<unsigned char*>(node),
                         nullptr, nextIndex));

        const int childExpression = MissionScript_GetExpressionChild(
            reinterpret_cast<void*>(self->field_0044),
            nullptr, expression, static_cast<int>(childIndex));

        S_0046c260_ParseExpression(
            self, nullptr, childNode, childExpression);

#ifdef CSF_STANDALONE
        // Do not paper over an invalid leaf: the resolver requires a typed
        // payload. Preserve its behavior, but retain the parent/child context
        // before the game's SEH discards the parser stack.
        void* child = reinterpret_cast<void*>(childExpression);
        if (CSF_AT(uint16_t, child, 2) == 0xFFFF) {
            void* value = CSF_AT(void*, child, 4);
            if (value && CSF_AT(int, value, 8) == -1 && !CSF_AT(void*, value, 4)) {
                csf_script_invalid_leaf(
                    attributeText, childIndex, childCount, childNode,
                    MissionScript_GetNodeType(childNode, nullptr), child,
                    unsigned(CSF_AT(uint16_t, child, 0)), value, definition);
            }
        }
#endif

        result = MissionScript_FinalizeExpressionChild(
            reinterpret_cast<void*>(self->field_0044),
            nullptr, expression, static_cast<int>(childIndex));

        childIndex = nextIndex;
    } while (childIndex < childCount);

    return result;
}

HOOK(0x0046E910, S_0046c260_ParseExpression, "auto");
