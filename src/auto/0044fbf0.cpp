#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// Busca en la lista de entradas de la escena la primera entrada cuyo campo +0x20
// coincide con la clave pedida. Devuelve el puntero a la entrada o 0.
// Entrada: this+0x10 = puntero al array de punteros, this+0x18 = numero de elementos.
static int __fastcall SceneAccess_FindFocusEntry(void* self, void* /*edx*/, int entryKey)
{
    const uint32_t entryCount = CSF_AT(uint32_t, self, 0x18);
    uint32_t result = 0;

    for (uint32_t i = 0; i < entryCount; ++i)
    {
        if (result != 0)
            break;

        // El original recarga el puntero al array en cada iteracion.
        const uint32_t entry = CSF_AT(uint32_t*, self, 0x10)[i];
        if (entry != 0)
        {
            if (CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(entry)), 0x20) == entryKey)
                result = entry;
        }
    }

    return static_cast<int>(result);
}

HOOK(0x44FBF0, SceneAccess_FindFocusEntry, "auto");
