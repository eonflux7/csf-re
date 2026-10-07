#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwTexDictionaryCreate, 0x00631650, ());

static char __fastcall C_008655e8_InitializeTextureDictionaryOnce(
    C_008655e8* self, void* /*edx*/)
{
    const uint8_t initialized = self->field_0004;
    if (initialized != 0)
        return static_cast<char>(initialized);

    self->field_0004 = 1;
    const int dictionary = RwTexDictionaryCreate();
    self->field_002c = static_cast<uint32_t>(dictionary);
    return static_cast<char>(static_cast<uint8_t>(dictionary));
}

HOOK(0x00726480, C_008655e8_InitializeTextureDictionaryOnce, "auto");
