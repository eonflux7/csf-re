#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00865980_UpdateAttachedObjectWithScaledState(
    C_00865980* self, void* /*edx*/)
{
    if (self->field_0030 == 0)
        return 0;

    void* source =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            self->vtbl[0x18 / 4])(self, nullptr);
    if (source == nullptr)
        return 0;

    // The push of EBX saves the register; this call has one stack argument.
    uint8_t sourceState[8];
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, source, 0)[0x160 / 4])(
            source, nullptr, sourceState);

    const uint32_t* sourcePair =
        reinterpret_cast<const uint32_t* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, source, 0)[0xA4 / 4])(source, nullptr);

    uint32_t pair[2];
    pair[0] = sourcePair[0];
    pair[1] = sourcePair[1];

    void* attached = reinterpret_cast<void*>(self->field_0030);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, attached, 0)[0x1C / 4])(
            attached, nullptr, sourceState);

    attached = reinterpret_cast<void*>(self->field_0030);
    reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, attached, 0)[0x24 / 4])(
            attached, nullptr, pair);

    attached = reinterpret_cast<void*>(self->field_0030);
    const uint32_t* currentState =
        reinterpret_cast<const uint32_t* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, attached, 0)[0x30 / 4])(attached, nullptr);

    uint32_t originalState[4];
    originalState[0] = currentState[0];
    originalState[1] = currentState[1];
    originalState[2] = currentState[2];
    originalState[3] = currentState[3];

    uint32_t scaledState[4];
    scaledState[0] = originalState[0];
    scaledState[1] = originalState[1];
    scaledState[2] = originalState[2];
    scaledState[3] = originalState[3];

    const uint8_t useFirstScale =
        reinterpret_cast<uint8_t (__fastcall*)(void*, void*)>(
            CSF_AT(void**, source, 0)[0xBC / 4])(source, nullptr);

    const double selectedScale =
        useFirstScale ? self->field_003c : self->field_0044;
    const float scaledComponent =
        static_cast<float>(selectedScale * static_cast<double>(self->field_0034));
    std::memcpy(&scaledState[3], &scaledComponent, sizeof(scaledComponent));

    attached = reinterpret_cast<void*>(self->field_0030);
    reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, attached, 0)[0x2C / 4])(
            attached, nullptr, scaledState);

    attached = reinterpret_cast<void*>(self->field_0030);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, attached, 0)[0x08 / 4])(attached, nullptr);

    attached = reinterpret_cast<void*>(self->field_0030);
    return reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, attached, 0)[0x2C / 4])(
            attached, nullptr, originalState);
}

HOOK(0x007288F0, C_00865980_UpdateAttachedObjectWithScaledState, "auto");
