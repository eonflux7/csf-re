#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __stdcall, Parser_ClassifyToken, 0x00692920, (uint32_t* a1));
CSF_FUNC(unsigned int, __fastcall, CorePaths_AppendNode, 0x00693410, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, CorePaths_AllocateNodeData, 0x006934E0, (uint32_t a1));
CSF_FUNC(uint8_t*, __fastcall, CorePaths_ConstructNode, 0x00693AD0, (void* self, void* edx, char a2));
CSF_FUNC(uint8_t*, __fastcall, CorePaths_ParseParenthesizedGroup, 0x00693F00, (void* self, void* edx, uint8_t* a2));
CSF_FUNC(int, __fastcall, GetElementByIndex, 0x0069E4D0, (void* self, void* , int index));

// The packet also lists these call-target addresses as vtable references.
// There are no corresponding vtable stores in this function.
[[maybe_unused]] static const auto kConstructNodeReference = CSF_VTBL(0x00693AD0);
[[maybe_unused]] static const auto kGrowArrayReference = CSF_VTBL(0x0041E5F0);

static uint8_t* __fastcall S_00693740_ParseBracketGroup(
    S_00693740* self, void* /*edx*/, uint8_t* parent)
{
    if (!parent)
        return parent;

    const uint32_t initialIndex = self->field_0010;
    if (initialIndex >= CSF_AT(uint32_t,
                              reinterpret_cast<void*>(self->field_000c), 0x04))
        return reinterpret_cast<uint8_t*>(initialIndex);

    uint8_t* pending = nullptr;
    auto* token = reinterpret_cast<uint32_t*>(
        GetElementByIndex(reinterpret_cast<void*>(self->field_000c),
                          nullptr, static_cast<int>(initialIndex)));

    for (;;)
    {
        ++self->field_0010;
        const uint8_t first = *reinterpret_cast<uint8_t*>(token[1]);

        if (first == 93 && token[0] == 1)
            break;

        auto* text = reinterpret_cast<uint8_t*>(token[1]);
        if (*text == 46 && text[-1] != 34)
        {
            void* storage = reinterpret_cast<void*>(CoreMemory_AllocUntagged(12));
            pending = storage
                ? CorePaths_ConstructNode(storage, nullptr, 1)
                : nullptr;

            if (pending[11] == 0xFF)
                CSF_AT(uint32_t*, CSF_AT(void*, pending, 0x00), 0x04) = token;

            if (parent[11] == 0xFF)
                CorePaths_AppendNode(CSF_AT(void*, parent, 0x00), nullptr,
                                     reinterpret_cast<int>(pending));
        }
        else if (first == 91 && token[0] == 1 && text[-1] != 34)
        {
            if (pending[11] == 0xFF)
                CSF_AT(int, CSF_AT(void*, pending, 0x00), 0x00) = 2;
            else
                pending[10] = 2;

            if (pending[11] == 0xFF)
                CSF_AT(uint32_t, CSF_AT(void*, pending, 0x00), 0x44) = token[1];

            S_00693740_ParseBracketGroup(self, nullptr, pending);
            pending = nullptr;
        }
        else if (first == 40 && token[0] == 1 && text[-1] != 34)
        {
            if (pending[11] == 0xFF)
                CSF_AT(int, CSF_AT(void*, pending, 0x00), 0x00) = 1;
            else
                pending[10] = 1;

            if (pending[11] == 0xFF)
                CSF_AT(uint32_t, CSF_AT(void*, pending, 0x00), 0x44) = token[1];

            CorePaths_ParseParenthesizedGroup(self, nullptr, pending);
            pending = nullptr;
        }
        else
        {
            auto* value = reinterpret_cast<uint8_t*>(CoreMemory_AllocUntagged(12));
            if (value)
            {
                void* data = CorePaths_AllocateNodeData(76);
                if (data)
                {
                    CSF_AT(uint32_t, data, 0x08) = 0;
                    CSF_AT(uint32_t, data, 0x0C) = 0;
                    CSF_AT(uint32_t, data, 0x10) = 0;
                    CSF_AT(uint32_t, data, 0x14) = 10;
                    CSF_AT(int, data, 0x00) = -1;
                    CSF_AT(uint32_t, data, 0x04) = 0;
                    CSF_AT(uint32_t, data, 0x40) = 0;
                    CSF_AT(uint32_t, data, 0x14) = 100;
                    CSF_AT(void*, value, 0x00) = data;
                }
                else
                {
                    CSF_AT(void*, value, 0x00) = nullptr;
                }
                value[11] = 0xFF;
            }

            const int type = Parser_ClassifyToken(token);
            if (value[11] == 0xFF)
                CSF_AT(int, CSF_AT(void*, value, 0x00), 0x00) = type;
            else
                value[10] = static_cast<uint8_t>(type);

            if (value[11] == 0xFF)
            {
                CSF_AT(uint32_t*, CSF_AT(void*, value, 0x00), 0x04) = token;
                if (value[11] == 0xFF)
                {
                    CSF_AT(uint32_t, CSF_AT(void*, value, 0x00), 0x44) = token[1];
                    if (value[11] == 0xFF)
                        CSF_AT(uint32_t, CSF_AT(void*, value, 0x00), 0x48) =
                            token[1] + token[0];
                }
            }

            if (pending[11] == 0xFF)
                CSF_AT(int, CSF_AT(void*, pending, 0x00), 0x00) = 0;
            else
                pending[10] = 0;

            if (pending[11] == 0xFF)
            {
                CSF_AT(uint32_t, CSF_AT(void*, pending, 0x00), 0x48) =
                    token[1] + token[0];

                if (pending[11] == 0xFF)
                {
                    void* data = CSF_AT(void*, pending, 0x00);
                    const uint32_t inlineCount = CSF_AT(uint32_t, data, 0x40);
                    if (inlineCount < 0xA)
                    {
                        CSF_AT(uint8_t*, data, 0x18 + 4 * inlineCount) = value;
                        ++CSF_AT(uint32_t, data, 0x40);
                    }
                    else
                    {
                        const uint32_t count = CSF_AT(uint32_t, data, 0x10);
                        const uint32_t capacity = CSF_AT(uint32_t, data, 0x0C);
                        auto* overflow = reinterpret_cast<S_0041e5f0*>(
                            static_cast<uint8_t*>(data) + 8);

                        if (count >= capacity)
                            S_0041e5f0_GrowArrayCapacity(
                                overflow, nullptr,
                                static_cast<int>(overflow->field_000c));

                        const uint32_t index = overflow->field_0008;
                        auto* destination = reinterpret_cast<uint8_t**>(
                            overflow->field_0000 + 4 * index);
                        overflow->field_0008 = index + 1;
                        *destination = value;
                    }
                }
            }
            pending = nullptr;
        }

        token = reinterpret_cast<uint32_t*>(
            GetElementByIndex(reinterpret_cast<void*>(self->field_000c),
                              nullptr, static_cast<int>(self->field_0010)));
    }

    if (parent[11] == 0xFF)
        CSF_AT(uint32_t, CSF_AT(void*, parent, 0x00), 0x48) = token[1] + 1;
    return parent;
}

HOOK(0x00693C90, S_00693740_ParseBracketGroup, "auto");
