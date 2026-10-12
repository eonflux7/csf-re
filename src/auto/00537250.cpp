#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Interfaz (INSTANCING SCENE/RESULT): stores the byte flag at +0x182 of the
// owning object and returns it. Original: mov al, [ebp+8]; mov [ecx+0x182], al; ret 4.
static char __fastcall Interfaz_SetSceneResultFlag(void* self, void* /*edx*/, char value)
{
    CSF_AT(uint8_t, self, 0x182) = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x537250, Interfaz_SetSceneResultFlag, "auto");
