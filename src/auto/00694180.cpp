#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(unsigned int, __fastcall, CoreParser_CopyBoundedString, 0x006927B0, (void* self, void* , void* dst, int maxLen));
CSF_FUNC(int, __fastcall, S_00692b80_GetChildCount, 0x00692B80, (S_00692b80* self, void*));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_GetComponent, 0x00692BD0, (S_00692b80* self, void* , unsigned int index));
CSF_FUNC(int, __fastcall, S_00692b80_ParseIntegerComponent, 0x00692C40, (S_00692b80* self, void*));
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(int, __fastcall, CorePaths_InternAttributeText, 0x006935A0, (void* self, void* edx, char* String1, char a3));
CSF_FUNC(int16_t, __fastcall, CorePaths_InternComponentName, 0x00693660, (void* self, void* edx, char* String1, char a3));

// The packet labels these direct-call addresses as vtable references.
[[maybe_unused]] static const auto kComponentAddress = CSF_VTBL(0x00692B80);
[[maybe_unused]] static const auto kArrayAddress = CSF_VTBL(0x0041E5F0);
[[maybe_unused]] static const auto kSerializeAddress = CSF_VTBL(0x00694180);
[[maybe_unused]] static const auto kTypeDispatchTable = CSF_ADDR_DATA(0x00694360);

static void __fastcall S_00694180_AppendComponentTree(
    S_00694180* self, void* /*edx*/,
    S_00694180* state, S_00692b80* component,
    std::uint32_t* previousSiblingLink, S_0041e5f0* records)
{
    if (!component)
        return;

    alignas(8) std::uint8_t bufferBytes[0x404];
    std::memset(bufferBytes, 0, 1024);
    char* buffer = reinterpret_cast<char*>(bufferBytes);

    const std::uint32_t recordIndex = CSF_AT(std::uint32_t, state, 0x0C);
    auto* record = reinterpret_cast<std::uint8_t*>(CoreMemory_AllocUntagged(12));
    if (record)
        CSF_AT(std::uint32_t, record, 0x00) = 0;

    CSF_AT(std::uint8_t, record, 0x0B) = 0;

    const int initialType = component->field_000b == 0xFF
        ? CSF_AT(int, reinterpret_cast<void*>(component->field_0000), 0x00)
        : CSF_AT(std::int8_t, component, 0x0A);
    CSF_AT(std::uint8_t, record, 0x0A) =
        static_cast<std::uint8_t>(initialType);

    if (component->field_000b != 0xFF
        || !CSF_AT(std::uint32_t,
                   reinterpret_cast<void*>(component->field_0000), 0x04)
        || static_cast<std::uint8_t>(initialType) == 5
        || static_cast<std::uint8_t>(initialType) == 3
        || static_cast<std::uint8_t>(initialType) == 4)
    {
        CSF_AT(std::int16_t, record, 0x08) = -1;
    }
    else
    {
        CoreParser_CopyBoundedString(
            reinterpret_cast<void*>(CSF_AT(
                std::uint32_t,
                reinterpret_cast<void*>(component->field_0000), 0x04)),
            nullptr, buffer, 1024);
        CSF_AT(std::int16_t, record, 0x08) =
            CorePaths_InternComponentName(self, nullptr, buffer, 1);
    }

    if (previousSiblingLink)
        *previousSiblingLink = recordIndex;
    ++CSF_AT(std::uint32_t, state, 0x0C);

    const int type = component->field_000b == 0xFF
        ? CSF_AT(int, reinterpret_cast<void*>(component->field_0000), 0x00)
        : CSF_AT(std::int8_t, component, 0x0A);

    bool appendRecord = true;
    switch (type)
    {
    case 0:
        CSF_AT(int, record, 0x04) = -1;
        break;
    case 1:
    case 2:
        CSF_AT(int, record, 0x04) =
            S_00692b80_GetChildCount(component, nullptr);
        break;
    case 3:
        CSF_AT(int, record, 0x04) = -1;
        CSF_AT(int, record, 0x04) =
            S_00692b80_ParseIntegerComponent(component, nullptr);
        break;
    case 4:
        CSF_AT(int, record, 0x04) = -1;
        CSF_AT(float, record, 0x04) = static_cast<float>(
            ReadComponentFloat(reinterpret_cast<std::uint32_t>(component)));
        break;
    case 5:
        CopyAttributeText(reinterpret_cast<std::uint8_t*>(component),
                          nullptr, buffer, 1024);
        CSF_AT(int, record, 0x04) =
            CorePaths_InternAttributeText(self, nullptr, buffer, 1);
        break;
    default:
        appendRecord = false;
        break;
    }

    if (appendRecord)
    {
        if (static_cast<std::uint32_t>(records->field_0008)
            >= static_cast<std::uint32_t>(records->field_0004))
        {
            S_0041e5f0_GrowArrayCapacity(
                records, nullptr, static_cast<int>(records->field_000c));
        }

        const std::uint32_t count = records->field_0008;
        auto* destination = reinterpret_cast<std::uint32_t*>(
            records->field_0000 + 4 * count);
        records->field_0008 = count + 1;
        *destination = reinterpret_cast<std::uint32_t>(record);
    }

    const std::uint32_t childCount = static_cast<std::uint32_t>(
        S_00692b80_GetChildCount(component, nullptr));

    for (std::uint32_t childIndex = 0; childIndex < childCount; ++childIndex)
    {
        if (childIndex == 0)
        {
            S_00692b80* child =
                S_00692b80_GetComponent(component, nullptr, 0);
            S_00694180_AppendComponentTree(
                self, nullptr, state, child, nullptr, records);
        }
        else
        {
            auto* entries =
                reinterpret_cast<std::uint32_t*>(records->field_0000);
            auto* siblingLink =
                reinterpret_cast<std::uint32_t*>(entries[recordIndex + 1]);

            std::uint32_t remaining = childIndex - 1;
            while (remaining)
            {
                --remaining;
                siblingLink =
                    reinterpret_cast<std::uint32_t*>(entries[*siblingLink]);
            }

            S_00692b80* child =
                S_00692b80_GetComponent(component, nullptr, childIndex);
            S_00694180_AppendComponentTree(
                self, nullptr, state, child, siblingLink, records);
        }
    }
}

HOOK(0x00694180, S_00694180_AppendComponentTree, "auto");
