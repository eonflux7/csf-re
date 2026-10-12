#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CollisionShapes_DestroyElements, 0x006CE270,
         (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static char __fastcall ReleaseSphereEllipsoidBuffer(S_0044bb60* self, void* /*edx*/)
{
    uint32_t bufferPtr = self->field_0004;
    char returnByte = static_cast<char>(bufferPtr);

    if (bufferPtr != 0)
    {
        CollisionShapes_DestroyElements(static_cast<int>(bufferPtr),
                                        static_cast<int>(self->field_0008),
                                        reinterpret_cast<int>(self), 0);
        bufferPtr = self->field_0004;
        if (bufferPtr != 0)
            returnByte = static_cast<char>(
                Mem_FreeDebug(reinterpret_cast<void*>(bufferPtr)));
    }

    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    return returnByte;
}

HOOK(0x006CE690, ReleaseSphereEllipsoidBuffer, "auto");
