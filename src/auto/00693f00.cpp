#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __stdcall, Parser_ClassifyToken, 0x00692920, (uint32_t* a1));
CSF_FUNC(unsigned int, __fastcall, CorePaths_AppendChild, 0x00693410, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, CorePaths_AllocateNodeData, 0x006934E0, (uint32_t a1));
CSF_FUNC(uint8_t*, __fastcall, CorePaths_InitializeNode, 0x00693AD0, (void* self, void* edx, char a2));
CSF_FUNC(uint8_t*, __fastcall, CorePaths_ParseBracketedGroup, 0x00693C90, (void* self, void* edx, uint8_t* a2));
CSF_FUNC(int, __fastcall, GetElementByIndex, 0x0069E4D0, (void* self, void* , int index));

// The packet also identifies these call targets as vtable references.
[[maybe_unused]] static const auto kNodeTableReference = CSF_VTBL(0x00693AD0);
[[maybe_unused]] static const auto kArrayTableReference = CSF_VTBL(0x0041E5F0);

static uint8_t* CorePaths_CreateGroupNode(uint32_t* token, uint32_t kind)
{
    void* storage = reinterpret_cast<void*>(CoreMemory_AllocUntagged(12));
    uint8_t* node = storage
        ? CorePaths_InitializeNode(storage, nullptr, 1)
        : nullptr;

    if (node[11] == 0xFF)
        CSF_AT(uint32_t, CSF_AT(void*, node, 0), 0) = kind;
    else
        node[10] = static_cast<uint8_t>(kind);

    if (node[11] == 0xFF)
    {
        CSF_AT(uint32_t, CSF_AT(void*, node, 0), 4) = 0;
        if (node[11] == 0xFF)
            CSF_AT(uint32_t, CSF_AT(void*, node, 0), 68) = token[1];
    }
    return node;
}

static uint8_t* CorePaths_CreateTokenNode(uint32_t* token)
{
    uint8_t* node =
        reinterpret_cast<uint8_t*>(CoreMemory_AllocUntagged(12));

    if (node)
    {
        void* data = CorePaths_AllocateNodeData(76);
        if (data)
        {
            CSF_AT(uint32_t, data, 8) = 0;
            CSF_AT(uint32_t, data, 12) = 0;
            CSF_AT(uint32_t, data, 16) = 0;
            CSF_AT(uint32_t, data, 20) = 10;
            CSF_AT(uint32_t, data, 0) = static_cast<uint32_t>(-1);
            CSF_AT(uint32_t, data, 4) = 0;
            CSF_AT(uint32_t, data, 64) = 0;
            CSF_AT(uint32_t, data, 20) = 100;
            CSF_AT(void*, node, 0) = data;
        }
        else
        {
            CSF_AT(void*, node, 0) = nullptr;
        }
        node[11] = 0xFF;
    }

    int kind = Parser_ClassifyToken(token);
    if (node[11] == 0xFF)
        CSF_AT(int, CSF_AT(void*, node, 0), 0) = kind;
    else
        node[10] = static_cast<uint8_t>(kind);

    if (node[11] == 0xFF)
        CSF_AT(uint32_t*, CSF_AT(void*, node, 0), 4) = token;

    return node;
}

static void CorePaths_StoreTokenChild(uint8_t* parent, uint8_t* child)
{
    if (parent[11] != 0xFF)
        return;

    void* data = CSF_AT(void*, parent, 0);
    uint32_t inlineCount = CSF_AT(uint32_t, data, 64);
    if (inlineCount < 0xA)
    {
        CSF_AT(uint8_t*, data, 24 + 4 * inlineCount) = child;
        ++CSF_AT(uint32_t, data, 64);
    }
    else
    {
        uint8_t* array = static_cast<uint8_t*>(data) + 8;
        if (CSF_AT(uint32_t, data, 16) >= CSF_AT(uint32_t, array, 4))
        {
            S_0041e5f0_GrowArrayCapacity(
                reinterpret_cast<S_0041e5f0*>(array), nullptr,
                CSF_AT(int, array, 12));
        }

        uint32_t count = CSF_AT(uint32_t, array, 8);
        uint8_t** slot = CSF_AT(uint8_t**, array, 0) + count;
        CSF_AT(uint32_t, array, 8) = count + 1;
        *slot = child;
    }
}

static uint8_t* __fastcall S_00693740_ParseParenthesizedGroup(
    S_00693740* self, void* /*edx*/, uint8_t* parent)
{
    if (!parent)
        return parent;

    if (self->field_0010 >=
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_000c), 4))
    {
        return reinterpret_cast<uint8_t*>(self->field_0010);
    }

    uint32_t* token = reinterpret_cast<uint32_t*>(
        GetElementByIndex(reinterpret_cast<void*>(self->field_000c),
                          nullptr, static_cast<int>(self->field_0010)));

    for (;;)
    {
        ++self->field_0010;

        uint8_t firstCharacter = *reinterpret_cast<uint8_t*>(token[1]);
        if (firstCharacter == 41 && token[0] == 1)
            break;

        uint8_t* tokenText = reinterpret_cast<uint8_t*>(token[1]);
        uint32_t groupKind = 0;
        bool makeTokenNode = false;

        if (*tokenText == 40 && token[0] == 1 && tokenText[-1] != 34)
        {
            groupKind = 1;
        }
        else if (firstCharacter == 91 && token[0] == 1)
        {
            if (tokenText[-1] == 34)
                makeTokenNode = true;
            else
                groupKind = 2;
        }
        else if (tokenText[-1] != 34 &&
                 (firstCharacter == 93 || firstCharacter == 46))
        {
            // Unquoted closing brackets and dots do not produce token nodes.
        }
        else
        {
            makeTokenNode = true;
        }

        if (groupKind != 0)
        {
            uint8_t* child = CorePaths_CreateGroupNode(token, groupKind);
            if (parent[11] == 0xFF)
            {
                CorePaths_AppendChild(
                    CSF_AT(void*, parent, 0), nullptr,
                    reinterpret_cast<int>(child));
            }

            if (groupKind == 1)
                S_00693740_ParseParenthesizedGroup(self, nullptr, child);
            else
                CorePaths_ParseBracketedGroup(self, nullptr, child);
        }
        else if (makeTokenNode)
        {
            uint8_t* child = CorePaths_CreateTokenNode(token);
            CorePaths_StoreTokenChild(parent, child);
        }

        token = reinterpret_cast<uint32_t*>(
            GetElementByIndex(reinterpret_cast<void*>(self->field_000c),
                              nullptr, static_cast<int>(self->field_0010)));
    }

    if (parent[11] == 0xFF)
        CSF_AT(uint32_t, CSF_AT(void*, parent, 0), 72) = token[1] + 1;

    return parent;
}

HOOK(0x00693F00, S_00693740_ParseParenthesizedGroup, "auto");
