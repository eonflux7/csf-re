#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Forwarder: tail-jumps to the object's own "get position" virtual; takes only `this`.
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// vtable slot 41 (+0xA4) of CPlayerCharacter.
// Returns the character's reference point: normally its position raised by the
// height offset at +0x1D4; when flag 0x2000 is set, the position of node 3000
// of the object referenced by the handle at +0x1B4 (if that node exists).
static float* __fastcall CPlayerCharacter_GetEyePosition(CPlayerCharacter* self, void* /*edx*/, float* outPosition)
{
    const float* basePosition = GetObjectPositionVector(self, nullptr);

    float posX = basePosition[0];
    float posY = basePosition[1];
    float posZ = basePosition[2];

    if ((self->field_02e0 & 0x2000) != 0)
    {
        void* attached = reinterpret_cast<void*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x1B4)));

        void* nodeContainer =
            ((void*(__fastcall*)(void*, void*))CSF_AT(void**, attached, 0)[0x28 / 4])(attached, nullptr);

        int nodeIndex =
            ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, nodeContainer, 0)[0x98 / 4])(nodeContainer, nullptr, 3000);

        if (nodeIndex != -1)
        {
            nodeContainer =
                ((void*(__fastcall*)(void*, void*))CSF_AT(void**, attached, 0)[0x28 / 4])(attached, nullptr);

            void* node =
                ((void*(__fastcall*)(void*, void*, int))CSF_AT(void**, nodeContainer, 0)[0x8C / 4])(nodeContainer, nullptr, nodeIndex);

            void* nodeTransform =
                ((void*(__fastcall*)(void*, void*))CSF_AT(void**, node, 0)[0x18 / 4])(node, nullptr);

            const float* nodePosition =
                ((const float*(__fastcall*)(void*, void*))CSF_AT(void**, nodeTransform, 0)[0x10 / 4])(nodeTransform, nullptr);

            posX = nodePosition[0];
            posY = nodePosition[1];
            posZ = nodePosition[2];
        }
    }
    else
    {
        posY = posY + self->field_01d4;
    }

    outPosition[0] = posX;
    outPosition[1] = posY;
    outPosition[2] = posZ;
    return outPosition;
}

HOOK(0x004949A0, CPlayerCharacter_GetEyePosition, "auto");
