#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(bool, __fastcall, ForwardToDelegate, 0x0048E710, (void* self, void* edx, CPlayerCharacter* owner));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));

// Fallback target evaluation for the player commando: if no target is set yet and the
// handle at +0x240 resolves to an object of type 27 that accepts forwarding, record the
// fallback target and its description id.
static int __fastcall CPlayerCharacter_EvaluateFallbackTarget(CPlayerCharacter* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0298);
    if (result != 0) {
        return result;
    }

    result = static_cast<int>(self->field_0248);
    if (result < 0) {
        return result;
    }

    const int resolvedHandle = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(&self->field_0240)));
    result = resolvedHandle;
    if (resolvedHandle == 0) {
        return result;
    }

    auto* entity = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(resolvedHandle));
    if (CSF_AT(int, entity, 0x50) != 27) {
        return result;
    }

    const bool forwarded = ForwardToDelegate(entity, nullptr, self);
    if (!forwarded) {
        // Only the low byte (bool result) is meaningful on this path.
        result &= ~0xFF;
        return result;
    }

    self->field_0298 = 12;
    self->field_02a0 = CSF_AT(uint32_t, self, 0x244);
    self->field_02a4 = self->field_0248;
    const int parsedId = MenuExtras_ParseDescriptionId(CSF_STR(0x00833DD0, "G010"));
    self->field_02ac = static_cast<uint32_t>(parsedId);
    return parsedId;
}

HOOK(0x004941D0, CPlayerCharacter_EvaluateFallbackTarget, "auto");
