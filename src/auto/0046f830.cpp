#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, S_0046f790_ValidateReference, 0x0046F790,
         (void* self, void* edx, int16_t* a2));

// The packet labels this direct-call address as a vtable reference.
[[maybe_unused]] static const auto g_ReferenceValidatorAddressMarker =
    CSF_VTBL(0x0046F790);

static char __fastcall S_0046f790_ValidateReferences(
    S_0046f790* self, void* /*edx*/)
{
    char valid = 1;

    int32_t count = self->field_000c
        ? static_cast<int32_t>(self->field_0010 - self->field_000c) >> 3
        : 0;
    for (int32_t index = 0; index < count; ++index)
    {
        if (!valid)
            break;

        int16_t* reference = CSF_AT(
            int16_t*, reinterpret_cast<void*>(self->field_000c),
            8 * index + 4);
        if (reference)
            valid = S_0046f790_ValidateReference(self, nullptr, reference);
    }

    count = self->field_001c
        ? static_cast<int32_t>(self->field_0020 - self->field_001c) >> 3
        : 0;
    for (int32_t index = 0; index < count; ++index)
    {
        if (!valid)
            break;

        valid = S_0046f790_ValidateReference(
            self, nullptr,
            reinterpret_cast<int16_t*>(self->field_001c + 8u * index));
    }

    count = self->field_0030
        ? static_cast<int32_t>(self->field_0034 - self->field_0030) >> 3
        : 0;
    for (int32_t index = 0; index < count; ++index)
    {
        if (!valid)
            break;

        valid = S_0046f790_ValidateReference(
            self, nullptr,
            reinterpret_cast<int16_t*>(self->field_0030 + 8u * index));
    }

    count = self->field_0040
        ? static_cast<int32_t>(self->field_0044 - self->field_0040) >> 2
        : 0;
    for (int32_t index = 0; index < count; ++index)
    {
        if (!valid)
            break;

        valid = reinterpret_cast<int32_t*>(self->field_0040)[index] != -1;
    }

    return valid;
}

HOOK(0x0046F830, S_0046f790_ValidateReferences, "auto");
