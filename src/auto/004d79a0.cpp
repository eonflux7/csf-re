#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Destructor-style callee bound by address (__thiscall, one stack arg, ret 4).
CSF_FUNC(char*, __fastcall, TLanzarObjetoAPos_DestroySubobjectsAndMaybeFree, 0x00421550,
         (TLanzarObjetoAPos* self, void* /*edx*/, char a2));

// Thunk for the secondary base of TLanzarObjetoAPos located 4 bytes into the object.
// The original does `sub ecx, 4` and tail-jumps to the real implementation, so the
// callee receives the adjusted `this` and the same stack argument.
static char* __fastcall TLanzarObjetoAPos_SecondaryBaseDestroyThunk(char* self, void* /*edx*/, char a2)
{
    return TLanzarObjetoAPos_DestroySubobjectsAndMaybeFree(
        reinterpret_cast<TLanzarObjetoAPos*>(self - 4), nullptr, a2);
}

HOOK(0x004D79A0, TLanzarObjetoAPos_SecondaryBaseDestroyThunk, "auto");
