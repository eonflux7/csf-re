#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Libreria RenderWare: creacion, multiplicacion y destruccion de matrices 4x4.
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(uint32_t*, __cdecl, RwMatrixMultiply, 0x00624630, (uint32_t* a1, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

// Combina dos transformaciones (this * *a3) en una nueva matriz, guarda su
// puntero en el registro de salida a2 y marca el campo +4 a 1.
// Devuelve a2.
static int __fastcall GfxSkin_CombineTransform(uint32_t* self, void* /*edx*/, int a2, uint32_t* a3)
{
    uint32_t* combinedTemp = RwMatrixCreate();
    RwMatrixMultiply(combinedTemp, reinterpret_cast<uint32_t*>(self[0]), reinterpret_cast<uint32_t*>(a3[0]));

    uint32_t* outMatrix = RwMatrixCreate();
    uint8_t* outRecord = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));
    *reinterpret_cast<uint32_t**>(outRecord) = outMatrix;
    std::memcpy(outMatrix, combinedTemp, 0x40);
    outRecord[4] = 1;

    RwMatrixDestroy(combinedTemp);
    return static_cast<int>(reinterpret_cast<uintptr_t>(outRecord));
}

HOOK(0x0048E060, GfxSkin_CombineTransform, "auto");
