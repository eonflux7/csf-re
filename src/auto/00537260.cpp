#include <cstdint>
#include "globals.h"
#include "hook.h"

// Interfaz instancing scene/result flag setter: stores the byte flag at +0x184 and returns it.
// Original: mov al, [ebp+8]; mov [ecx+0x184], al; ret 4 (returns the value in al only).
static char __fastcall Interfaz_SetInstancingSceneResult(void* self, void* /*edx*/, char value)
{
    CSF_AT(uint8_t, self, 0x184) = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x537260, Interfaz_SetInstancingSceneResult, "auto");
