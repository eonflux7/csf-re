#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int* __fastcall S_0043e970_FindStringLowerBound(
    S_0043e970* self, void* /*edx*/, const void* key)
{
    int* candidate = reinterpret_cast<int*>(self->field_0004);
    void* node = CSF_AT(void*, candidate, 0x04);

    if (CSF_AT(uint8_t, node, 0x2D))
        return candidate;

    const uint32_t keyCapacity = CSF_AT(uint32_t, key, 0x18);
    const uint32_t keyLength = CSF_AT(uint32_t, key, 0x14);
    const auto* keyStorage = static_cast<const uint8_t*>(key) + 0x04;

    do
    {
        const uint8_t* keyText = keyCapacity < 0x10
            ? keyStorage
            : CSF_AT(const uint8_t*, keyStorage, 0x00);

        const uint32_t nodeLength = CSF_AT(uint32_t, node, 0x20);
        int comparison = 0;

        if (nodeLength != 0)
        {
            const uint32_t compareLength =
                nodeLength < keyLength ? nodeLength : keyLength;

            const uint8_t* nodeText =
                CSF_AT(uint32_t, node, 0x24) < 0x10
                ? static_cast<const uint8_t*>(node) + 0x10
                : CSF_AT(const uint8_t*, node, 0x10);

            for (uint32_t index = 0; index < compareLength; ++index)
            {
                const uint8_t nodeByte = nodeText[index];
                const uint8_t keyByte = keyText[index];
                if (nodeByte != keyByte)
                {
                    comparison = nodeByte < keyByte ? -1 : 1;
                    break;
                }
            }
        }

        if (comparison < 0 ||
            (comparison == 0 && nodeLength < keyLength))
        {
            node = CSF_AT(void*, node, 0x08);
        }
        else
        {
            candidate = static_cast<int*>(node);
            node = CSF_AT(void*, node, 0x00);
        }
    }
    while (!CSF_AT(uint8_t, node, 0x2D));

    return candidate;
}

HOOK(0x0043E970, S_0043e970_FindStringLowerBound, "auto");
