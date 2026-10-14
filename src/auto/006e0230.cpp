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

// Callee bindings (by original address).
CSF_FUNC(std::uint32_t*, __cdecl, CreateParticleObject, 0x006C9BE0, (int type));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0,
         (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// Creates a particle object of the given type, stores it in the first empty slot
// of the object's pointer array (or appends it, growing the array when full),
// bumps the registered-particle counter and returns the particle.
static int __fastcall C_00861988_CreateAndRegisterParticle(C_00861988* self, void* /*edx*/, int a2)
{
    const std::uint32_t particle =
        reinterpret_cast<std::uint32_t>(CreateParticleObject(a2));
    if (particle == 0)
        return static_cast<int>(particle);

    const std::uint32_t count = self->field_001c;
    std::uint32_t scanIndex = 0;
    bool foundEmptySlot = false;

    // Scan the used part of the array for a null (empty) slot.
    if (count != 0) {
        std::uint32_t* cursor = reinterpret_cast<std::uint32_t*>(self->field_0014);
        while (true) {
            if (*cursor == 0) {
                foundEmptySlot = true;
                break;
            }
            ++scanIndex;
            ++cursor;
            if (scanIndex >= count)
                break;
        }
    }

    if (foundEmptySlot && scanIndex != 0xFFFFFFFFu) {
        // Reuse the empty slot.
        std::uint32_t* slots = reinterpret_cast<std::uint32_t*>(self->field_0014);
        slots[scanIndex] = particle;
        ++self->field_0050;
        return static_cast<int>(particle);
    }

    // Append at the end, growing the array first if it is full.
    if (self->field_001c >= self->field_0018) {
        // Code-address annotation: function value, not a vtable.
        [[maybe_unused]] auto growArrayVtbl = reinterpret_cast<void**>(CSF_ADDR_FN(0x0041E5F0));
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(&self->field_0014),
            nullptr,
            static_cast<int>(CSF_AT(std::uint32_t, self, 0x20)));
    }

    const std::uint32_t appendIndex = self->field_001c;
    std::uint32_t* slots = reinterpret_cast<std::uint32_t*>(self->field_0014);
    self->field_001c = appendIndex + 1;
    slots[appendIndex] = particle;
    ++self->field_0050;
    return static_cast<int>(particle);
}

HOOK(0x006E0230, C_00861988_CreateAndRegisterParticle, "auto");
