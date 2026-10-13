#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static __declspec(safebuffers) char __fastcall C_008443c8_ClearDelegatedWordAt(
    C_008443c8* self, void* /*edx*/, int bufferAddress,
    unsigned int capacity, int /*unused*/)
{
    // The original reserves the slot with `push ecx`: it holds `this` until the callee writes it.
    uint32_t index = reinterpret_cast<uint32_t>(self);

    void* innerObject = reinterpret_cast<void*>(self->field_0004);
    const uint8_t gotIndex =
        reinterpret_cast<uint8_t(__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, innerObject, 0)[0x20 / 4])(
                innerObject, &index, &index);

    if (!gotIndex || index >= capacity)
        return 0;

    const char result = static_cast<char>(
        self->vSlot64(bufferAddress, static_cast<int>(2 * index)));

    if (result)
    {
        CSF_AT(uint16_t, reinterpret_cast<void*>(bufferAddress), 2 * index) = 0;
    }

    return result;
}

HOOK(0x0054E350, C_008443c8_ClearDelegatedWordAt, "auto");
