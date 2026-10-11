#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x0047BEE0 (sub ecx,4 ; jmp TNavegando_Destroy)
// Tail-forwards to TNavegando_Destroy with `this` moved back 4 bytes.
CSF_FUNC(char*, __fastcall, TNavegando_Destroy, 0x00421590, (TNavegando* self, void* , char a2));

static char* __fastcall AttackTasks_DestroyNavegandoSubobject(char* self, void* /*edx*/, char a2)
{
    // The navegando base sits 4 bytes before this subobject.
    TNavegando* navegando = reinterpret_cast<TNavegando*>(self - 4);
    return TNavegando_Destroy(navegando, nullptr, a2);
}

HOOK(0x0047BEE0, AttackTasks_DestroyNavegandoSubobject, "auto");
