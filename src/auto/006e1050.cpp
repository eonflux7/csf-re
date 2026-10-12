#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));

static int __fastcall GfxParticleEmitters_EqualsEmitterParameters(
    void* self, void* /*edx*/, void* other)
{
    const unsigned int otherLength = CSF_AT(unsigned int, other, 0x14);
    const char* otherText =
        CSF_AT(unsigned int, other, 0x18) < 0x10u
            ? reinterpret_cast<const char*>(other) + 4
            : CSF_AT(const char*, other, 0x04);

    return CompareStringSubstring(
               self, nullptr, 0, CSF_AT(unsigned int, self, 0x14),
               otherText, otherLength) == 0
        && CSF_AT(float, self, 0x20) == CSF_AT(float, other, 0x20)
        && CSF_AT(float, self, 0x24) == CSF_AT(float, other, 0x24)
        && CSF_AT(float, self, 0x28) == CSF_AT(float, other, 0x28)
        && CSF_AT(float, self, 0x2C) == CSF_AT(float, other, 0x2C)
        && CSF_AT(float, self, 0x30) == CSF_AT(float, other, 0x30)
        && CSF_AT(float, self, 0x34) == CSF_AT(float, other, 0x34);
}

HOOK(0x006E1050, GfxParticleEmitters_EqualsEmitterParameters, "auto");
