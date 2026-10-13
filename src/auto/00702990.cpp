#include <cstdint>
#include "globals.h"
#include "hook.h"

// Busca en el array de punteros (this+0x10, con this+0x18 entradas) el primer elemento
// cuyo campo +0x18 coincide con la clave pasada. Devuelve el indice o -1.
static int __fastcall ParticleAtomic_FindEmitterSlotByKey(void* self, void* /*edx*/, int key)
{
    uint32_t slotCount = CSF_AT(uint32_t, self, 0x18);
    int result = -1;

    if (slotCount == 0)
        return result;

    uint32_t* slotArray = CSF_AT(uint32_t*, self, 0x10);
    uint32_t index = 0;

    for (;;)
    {
        uint8_t* emitter = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(slotArray[index]));
        if (CSF_AT(int, emitter, 0x18) == key)
            return static_cast<int>(index);

        ++index;
        if (index >= slotCount)
            break;
    }

    return result;
}

HOOK(0x00702990, ParticleAtomic_FindEmitterSlotByKey, "auto");
