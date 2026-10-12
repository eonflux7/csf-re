#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(uint32_t*, __fastcall, FindSpawnEntry, 0x007A6B60,
               (void* self, void* edx, int a2));
static CSF_FUNC(char, __fastcall, ProcessSpawnMessage, 0x007A9B50,
               (void* self, void* edx, uint8_t a2, int a3, int a4, int a5));

static bool __fastcall C_00868abc_ProcessEnabledSpawn(
    C_00868abc* self, void* /*edx*/, char messageType,
    int entryId, int argument4, int argument5)
{
    char blocked = 0;
    if (self->field_0024)
    {
        void* filter = reinterpret_cast<void*>(self->field_0024);
        blocked = reinterpret_cast<char (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, filter, 0)[0x18 / 4])(
                filter, nullptr, entryId);
    }

    if (self->field_0020 && !blocked)
    {
        uint32_t* entry = FindSpawnEntry(
            reinterpret_cast<void*>(self->field_0020), nullptr, entryId);
        blocked = CSF_AT(char, entry, 0x0C);
    }

    return self->field_00ac && !blocked &&
           ProcessSpawnMessage(self, nullptr,
                               static_cast<uint8_t>(messageType),
                               entryId, argument4, argument5);
}

HOOK(0x00752020, C_00868abc_ProcessEnabledSpawn, "auto");
