#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(uint32_t*, __fastcall, RegisterSpawnId, 0x007A6B60,
               (void* self, void* edx, int a2));
static CSF_FUNC(bool, __fastcall, SendSpawnMessage, 0x007A9B20,
               (void* self, void* edx, uint8_t a2, int a3));

static bool __fastcall C_00868abc_RegisterAndSendSpawn(
    C_00868abc* self, void* /*edx*/, char flags, int spawnId)
{
    bool handled = false;
    if (self->field_0024)
    {
        void* receiver = reinterpret_cast<void*>(self->field_0024);
        handled = reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, receiver, 0)[0x18 / 4])(
                receiver, nullptr, spawnId);
    }

    if (self->field_0020 && !handled)
        RegisterSpawnId(reinterpret_cast<void*>(self->field_0020),
                        nullptr, spawnId);

    for (int index = 0; index < 32; ++index)
    {
        if (static_cast<int32_t>((&self->field_0028)[index]) < 0)
        {
            (&self->field_0028)[index] = static_cast<uint32_t>(spawnId);
            break;
        }
    }

    ++self->field_00a8;
    return SendSpawnMessage(self, nullptr, static_cast<uint8_t>(flags), spawnId);
}

HOOK(0x00751F70, C_00868abc_RegisterAndSendSpawn, "auto");
