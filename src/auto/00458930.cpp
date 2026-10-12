#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Busca en la lista de objetos de escena el indice de la entrada cuyo campo +0x04
// coincide con el del objeto recibido como argumento. Devuelve -1 si no existe.
// Cada entrada ocupa 40 bytes; la clave de comparacion esta en +0x20 de cada entrada.
static int __fastcall S_00458930_FindIndex(S_00458930* self, void* /*edx*/, int a2)
{
    const uint32_t entriesBegin = self->field_000c;
    int32_t entryCount = 0;
    if (entriesBegin != 0)
        entryCount = (int32_t)(self->field_0010 - entriesBegin) / 40;

    int foundIndex = 0;
    if (entryCount == 0)
        return -1;

    const uint32_t searchKey = CSF_AT(uint32_t, a2, 0x4);
    uint8_t* base = reinterpret_cast<uint8_t*>(self->field_000c);

    for (int32_t i = 0; ; )
    {
        if (CSF_AT(uint32_t, base, 0x20 + i * 40) == searchKey)
            return foundIndex;

        ++foundIndex;
        ++i;
        if (foundIndex >= entryCount)
            return -1;
    }
}

HOOK(0x458930, S_00458930_FindIndex, "auto");
