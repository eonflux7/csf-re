#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __fastcall, Campaign_DecrementStringTreeIterator, 0x0043E130, (void* self, void* edx));
CSF_FUNC(void**, __fastcall, Campaign_InsertStringTreeNode, 0x00440280, (int a1, int a2, void** a3, char a4, void** a5, void* a6));

static int __fastcall Campaign_InsertUniqueString(
    void* self, void* /*edx*/, void* result, void* value)
{
    void* header = CSF_AT(void*, self, 0x4);
    void** node = CSF_AT(void**, header, 0x4);
    void** parent = static_cast<void**>(header);
    char insertLeft = 1;

    if (!CSF_AT(uint8_t, node, 0x2D))
    {
        const unsigned int valueLength = CSF_AT(unsigned int, value, 0x14);

        do
        {
            parent = node;
            const unsigned int nodeLength = CSF_AT(unsigned int, node, 0x20);
            const char* nodeText =
                CSF_AT(unsigned int, node, 0x24) < 0x10u
                    ? reinterpret_cast<const char*>(node) + 0x10
                    : CSF_AT(const char*, node, 0x10);

            int comparison = 0;
            if (valueLength)
            {
                const unsigned int compareLength =
                    valueLength < nodeLength ? valueLength : nodeLength;
                const char* valueText =
                    CSF_AT(unsigned int, value, 0x18) < 0x10u
                        ? static_cast<const char*>(value) + 0x4
                        : CSF_AT(const char*, value, 0x4);
                comparison = std::memcmp(valueText, nodeText, compareLength);
            }

            if (!comparison)
            {
                const unsigned int currentNodeLength =
                    CSF_AT(unsigned int, node, 0x20);
                comparison = valueLength < currentNodeLength
                    ? -1
                    : static_cast<int>(valueLength != currentNodeLength);
            }

            insertLeft = comparison < 0;
            node = insertLeft
                ? CSF_AT(void**, node, 0x0)
                : CSF_AT(void**, node, 0x8);
        } while (!CSF_AT(uint8_t, node, 0x2D));
    }

    void* iterator = parent;
    void** candidate = parent;

    if (insertLeft)
    {
        if (parent == CSF_AT(void**, CSF_AT(void*, self, 0x4), 0x0))
        {
            void** inserted = Campaign_InsertStringTreeNode(
                reinterpret_cast<int>(self), 0, &iterator, 1, parent, value);
            CSF_AT(void*, result, 0x0) = *inserted;
            CSF_AT(uint8_t, result, 0x4) = 1;
            return reinterpret_cast<int>(result);
        }

        Campaign_DecrementStringTreeIterator(&iterator, nullptr);
        candidate = static_cast<void**>(iterator);
    }

    const unsigned int valueLength = CSF_AT(unsigned int, value, 0x14);
    const char* valueText =
        CSF_AT(unsigned int, value, 0x18) < 0x10u
            ? static_cast<const char*>(value) + 0x4
            : CSF_AT(const char*, value, 0x4);

    if (CompareStringSubstring(
            reinterpret_cast<uint8_t*>(candidate) + 0xC,
            nullptr, 0, CSF_AT(unsigned int, candidate, 0x20),
            valueText, valueLength) >= 0)
    {
        CSF_AT(void*, result, 0x0) = candidate;
        CSF_AT(uint8_t, result, 0x4) = 0;
    }
    else
    {
        void** inserted = Campaign_InsertStringTreeNode(
            reinterpret_cast<int>(self), 0, &iterator, insertLeft, parent, value);
        CSF_AT(void*, result, 0x0) = *inserted;
        CSF_AT(uint8_t, result, 0x4) = 1;
    }

    return reinterpret_cast<int>(result);
}

HOOK(0x00440C80, Campaign_InsertUniqueString, "auto");
