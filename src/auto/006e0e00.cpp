#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, RpPrtAdvEClassStdCreate, 0x005785B0, (int a1, uint32_t* a2, int a3, int a4, int a5));
CSF_FUNC(uint32_t*, __cdecl, RpPrtStdPClassStdCreate, 0x0057F970, (int a1));
CSF_FUNC(int, __cdecl, RpPrtStdPClassSetCallBack, 0x0057FCF0, (int a1, int a2, int a3));
CSF_FUNC(int, __stdcall, GfxParticleEmitters_SetClassProperties, 0x006E05D0, (int a1, int a2));

static int __fastcall S_0041e5f0_InitializeParticleClasses(
    S_0041e5f0* self, void* /*edx*/)
{
    // The packet labels the grow-call address as a vtable reference.
    (void)CSF_VTBL(0x0041E5F0);

    if (self->field_0014 < 4)
    {
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<uint8_t*>(self) + 0x10),
            nullptr, static_cast<int>(4 - self->field_0014));
    }

    for (uint32_t i = CSF_AT(uint32_t, self, 0x18); i < 4; ++i)
        reinterpret_cast<uint32_t*>(self->field_0010)[i] = 0;
    CSF_AT(uint32_t, self, 0x18) = 4;

    alignas(8) uint8_t callbacks[0x38];
    const int callbacksAddress = reinterpret_cast<int>(callbacks);

    int particleClass = reinterpret_cast<int>(RpPrtStdPClassStdCreate(117));
    GfxParticleEmitters_SetClassProperties(particleClass, 0);
    std::memset(callbacks, 0, sizeof(callbacks));
    CSF_AT(uint32_t, callbacks, 0x00) =
        reinterpret_cast<uint32_t>(CSF_ADDR_FN(0x0057CFE0));
    RpPrtStdPClassSetCallBack(particleClass, 1, callbacksAddress);
    reinterpret_cast<uint32_t*>(self->field_0010)[0] = particleClass;

    particleClass = reinterpret_cast<int>(RpPrtStdPClassStdCreate(101));
    GfxParticleEmitters_SetClassProperties(particleClass, 2);
    std::memset(callbacks, 0, sizeof(callbacks));
    CSF_AT(uint32_t, callbacks, 0x00) =
        reinterpret_cast<uint32_t>(CSF_ADDR_FN(0x0057CFE0));
    RpPrtStdPClassSetCallBack(particleClass, 1, callbacksAddress);
    reinterpret_cast<uint32_t*>(self->field_0010)[2] = particleClass;

    // The last three arguments occupy the preceding callback call's
    // still-live argument slots; the original callee uses only the first two.
    particleClass = RpPrtAdvEClassStdCreate(
        69, nullptr, particleClass, 1, callbacksAddress);
    GfxParticleEmitters_SetClassProperties(particleClass, 3);
    std::memset(callbacks, 0, sizeof(callbacks));
    CSF_AT(uint32_t, callbacks, 0x00) =
        reinterpret_cast<uint32_t>(CSF_ADDR_FN(0x0057CFE0));
    CSF_AT(uint32_t, callbacks, 0x08) =
        reinterpret_cast<uint32_t>(CSF_ADDR_FN(0x006C2AB0));
    CSF_AT(uint32_t, callbacks, 0x0C) =
        reinterpret_cast<uint32_t>(CSF_ADDR_FN(0x006C2AB0));
    RpPrtStdPClassSetCallBack(particleClass, 1, callbacksAddress);
    reinterpret_cast<uint32_t*>(self->field_0010)[3] = particleClass;

    particleClass = reinterpret_cast<int>(RpPrtStdPClassStdCreate(2));
    GfxParticleEmitters_SetClassProperties(particleClass, 1);
    std::memset(callbacks, 0, sizeof(callbacks));
    CSF_AT(uint32_t, callbacks, 0x00) =
        reinterpret_cast<uint32_t>(CSF_ADDR_FN(0x00704D60));
    const int result =
        RpPrtStdPClassSetCallBack(particleClass, 1, callbacksAddress);
    reinterpret_cast<uint32_t*>(self->field_0010)[1] = particleClass;
    return result;
}

HOOK(0x006E0E00, S_0041e5f0_InitializeParticleClasses, "auto");
