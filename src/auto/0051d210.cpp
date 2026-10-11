#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(unsigned int, __fastcall, S_0051c1b0_GrowEntries, 0x0051C1B0, (void* self, void* edx, int newCapacity));
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, SceneLoader_GetObjectAttachment, 0x006B87A0, (int a1));
CSF_FUNC(void*, __fastcall, FxParticleManager_GetAttachmentTransform, 0x006B8B10, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, FxParticleManager_GetParticleController, 0x006CA810, (int a1));

static char __fastcall C_00840ee0_CreateAttachedParticle(
    C_00840ee0* self, void* /*edx*/,
    int source, int frameObject, S_0051c1b0* entries)
{
    int resolvedObject = 0;
    if (CSF_AT(int, source, 0x10) >= 0)
        resolvedObject = CHandleTable_Resolve(g_pCurrentScene, nullptr, source + 8);

    if (!frameObject)
        return static_cast<char>(frameObject);

    const int attachment = SceneLoader_GetObjectAttachment(frameObject);
    if (!attachment)
        return static_cast<char>(attachment);

    if (self->field_001c == 0xFFFFFFFFu)
        return static_cast<char>(self->field_001c);

    const int services = GetValue8B0554();
    void* definitions = CSF_AT(void*, services, 0x1FCC);
    const int definition =
        reinterpret_cast<int(__fastcall*)(void*, void*, std::uint32_t)>(
            CSF_AT(void**, definitions, 0)[8 / 4])(
                definitions, nullptr, self->field_001c) + 4;

    alignas(8) std::uint8_t transformBytes[0x68];
    alignas(8) std::uint8_t matrixBytes[0x18];
    alignas(8) std::uint8_t creationOutputBytes[0x8];

    CSF_AT(void**, matrixBytes, 0x0C) = CSF_VTBL(0x0082D448);
    CSF_AT(int, matrixBytes, 0x10) = -1;
    CSF_AT(int, matrixBytes, 0x14) = -1;
    CSF_AT(void*, matrixBytes, 0x08) = nullptr;
    CSF_AT(int, creationOutputBytes, 0x00) = -1;
    CSF_AT(int, creationOutputBytes, 0x04) = -1;

    const int operation =
        (!resolvedObject || CSF_AT(std::uint32_t, resolvedObject, 0x50)) ? 10 : 20;
    void* factory = reinterpret_cast<void*>(self->field_0004);
    void* particle =
        reinterpret_cast<void*(__fastcall*)(void*, void*, int, int, void*)>(
            CSF_AT(void**, factory, 0)[8 / 4])(
                factory, nullptr, definition, operation, creationOutputBytes);
    if (!particle)
        return static_cast<char>(reinterpret_cast<std::uintptr_t>(particle));

    FxParticleManager_GetAttachmentTransform(
        reinterpret_cast<void*>(attachment), nullptr, transformBytes);
    const int controller = FxParticleManager_GetParticleController(
        static_cast<int>(reinterpret_cast<std::uintptr_t>(particle)));

    std::uint8_t* parameters = transformBytes + 0x3C;
    CSF_AT(std::uint32_t, parameters, 0x20) =
        CSF_AT(std::uint32_t, transformBytes, 0x0C);
    CSF_AT(std::uint32_t, parameters, 0x28) =
        CSF_AT(std::uint32_t, transformBytes, 0x14);
    CSF_AT(std::uint32_t, parameters, 0x24) =
        CSF_AT(std::uint32_t, transformBytes, 0x10);
    CSF_AT(std::uint32_t, parameters, 0x08) =
        CSF_AT(std::uint32_t, transformBytes, 0x04);
    CSF_AT(std::uint32_t, parameters, 0x04) =
        CSF_AT(std::uint32_t, transformBytes, 0x00);
    std::memset(parameters + 0x10, 0, 12);
    CSF_AT(float, parameters, 0x1C) = 6.2831855f;
    CSF_AT(std::uint32_t, parameters, 0x00) = 0;
    CSF_AT(std::uint32_t, parameters, 0x0C) =
        CSF_AT(std::uint32_t, transformBytes, 0x08);

    reinterpret_cast<void(__fastcall*)(void*, void*, void*, int)>(
        CSF_AT(void**, controller, 0)[28 / 4])(
            reinterpret_cast<void*>(controller), nullptr, parameters, 1);

    CSF_AT(std::uint32_t*, matrixBytes, 0x00) =
        RwFrameGetLTM(CSF_AT(std::uint32_t, frameObject, 0x04));
    CSF_AT(std::uint8_t, matrixBytes, 0x04) = 0;

    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, particle, 0)[64 / 4])(particle, nullptr, matrixBytes);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, particle, 0)[40 / 4])(particle, nullptr, 0);

    CSF_AT(void*, matrixBytes, 0x08) = particle;
    CSF_AT(std::uint32_t, creationOutputBytes, 0x04) = self->field_001c;

    // The packet also classifies this direct callee address as a vtable reference.
    (void)CSF_VTBL(0x0051C1B0);
    if (entries->field_0008 >= entries->field_0004)
        S_0051c1b0_GrowEntries(entries, nullptr, entries->field_000c);

    const std::uint32_t index = entries->field_0008;
    std::uint8_t* entry =
        reinterpret_cast<std::uint8_t*>(entries->field_0000) + 24 * index;
    entries->field_0008 = index + 1;
    CSF_AT(void*, entry, 0x00) = CSF_AT(void*, matrixBytes, 0x08);
    CSF_AT(int, entry, 0x08) = CSF_AT(int, matrixBytes, 0x10);
    CSF_AT(int, entry, 0x0C) = CSF_AT(int, matrixBytes, 0x14);
    CSF_AT(int, entry, 0x10) = CSF_AT(int, creationOutputBytes, 0x00);
    CSF_AT(std::uint32_t, entry, 0x14) =
        CSF_AT(std::uint32_t, creationOutputBytes, 0x04);

    const char ownsMatrix = CSF_AT(char, matrixBytes, 0x04);
    if (ownsMatrix)
        return static_cast<char>(
            RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0x00)));
    return ownsMatrix;
}

HOOK(0x0051D210, C_00840ee0_CreateAttachedParticle, "auto");
