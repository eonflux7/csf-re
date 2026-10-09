#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Busca en la lista de rectángulos de depuración el primer entry cuyo tag
// coincide con a2->+4 y cuyo objeto asociado (slot en field_0010) tiene el
// mismo +0x20 que el entry en +0x10. Devuelve el objeto encontrado o 0.
static int __fastcall CGuiControl_FindDebugRectObject(CGuiControl* self, void* /*edx*/, int a2)
{
    uint8_t* rectBase = reinterpret_cast<uint8_t*>(self->m_pDebugRect);

    // Número de entries de 24 bytes entre m_pDebugRect y el puntero final (+0x114).
    uint32_t rectCount = 0;
    if (self->m_pDebugRect != 0)
    {
        int32_t byteSpan = static_cast<int32_t>(CSF_AT(uint32_t, self, 0x114) - self->m_pDebugRect);
        rectCount = static_cast<uint32_t>(byteSpan / 24);
    }

    int foundObject = 0;
    int entryOffset = 0;
    for (uint32_t i = 0; i < rectCount; ++i)
    {
        if (foundObject != 0)
            break;

        uint8_t* entry = reinterpret_cast<uint8_t*>(self->m_pDebugRect) + entryOffset;
        uint8_t* argObject = reinterpret_cast<uint8_t*>(a2);

        if (CSF_AT(uint32_t, argObject, 4) == CSF_AT(uint32_t, entry, 4))
        {
            int slotIndex = CSF_AT(int, entry, 0x14);
            int candidate = 0;
            if (slotIndex >= 0 && slotIndex < static_cast<int>(self->m_id.alloc))
            {
                uint32_t* slots = reinterpret_cast<uint32_t*>(self->field_0010);
                candidate = static_cast<int>(slots[slotIndex]);
                if (candidate != 0)
                {
                    uint8_t* candidateObject = reinterpret_cast<uint8_t*>(candidate);
                    if (CSF_AT(uint32_t, candidateObject, 0x20) != CSF_AT(uint32_t, entry, 0x10))
                        candidate = 0;
                }
            }
            foundObject = candidate;
        }

        entryOffset += 24;
    }
    return foundObject;
}

HOOK(0x00452FD0, CGuiControl_FindDebugRectObject, "auto");
