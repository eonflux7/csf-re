#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Copia bloques de 3 DWORD hacia atras: desde a2 hasta a1 (exclusivo),
// escribiendo en a3 hacia abajo. Devuelve el puntero de destino final.
// a4 no se usa en el original.
static uint32_t* __cdecl SoundAreas_CopyTriplesBackward(uint32_t* sourceBegin,
                                                       uint32_t* sourceEnd,
                                                       uint32_t* destEnd,
                                                       int /*unused*/) {
    uint32_t* src = sourceEnd;
    uint32_t* dst = destEnd;

    while (src != sourceBegin) {
        src -= 3;
        dst -= 3;
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
    }
    return dst;
}

HOOK(0x7704A0, SoundAreas_CopyTriplesBackward, "auto");
