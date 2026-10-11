#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, GfxMatrix_EulerAnglesToDirection, 0x006AE640, (float* self, void* , float* out));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall C_008317c8_GetAimPositionAndDirection(
    C_008317c8* self, void* /*edx*/, float* target,
    float* position, float* direction)
{
    float scratch[4] = {};
    float fallbackPosition[3] = {};
    std::uint32_t temporary[2] = {};

    void* owner = reinterpret_cast<void*>(self->field_0004);
    float* source =
        reinterpret_cast<float*(__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, owner, 0)[0xA4 / 4])(
                owner, nullptr, scratch);
    position[0] = source[0];
    position[1] = source[1];
    position[2] = source[2];

    const int mode = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        self->vtbl[0x48 / 4])(self, nullptr);
    if (mode == 1)
    {
        source = reinterpret_cast<float*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0xB0 / 4])(owner, nullptr);
        direction[0] = source[0];
        direction[1] = source[1];
        direction[2] = source[2];
    }
    else if (mode > 1 && mode <= 3)
    {
        float* angles =
            reinterpret_cast<float*(__fastcall*)(void*, void*, std::uint32_t*)>(
                CSF_AT(void**, owner, 0)[0xAC / 4])(
                    owner, nullptr, temporary);
        source = GfxMatrix_EulerAnglesToDirection(angles, nullptr, scratch);
        direction[0] = source[0];
        direction[1] = source[1];
        direction[2] = source[2];
    }

    const int targetType = CSF_AT(int, target, 0);
    if (targetType == 1)
    {
        const int resolved = CHandleTable_Resolve(
            g_pCurrentScene, nullptr, reinterpret_cast<int>(target + 1));
        if (!resolved)
            return 0;

        void* entity = reinterpret_cast<void*>(resolved);
        if ((CSF_AT(std::uint32_t, entity, 0x170) & 0x2000) == 0)
            return static_cast<char>(GameConfig_IsResolvedEntryActive(
                CSF_AT(int, entity, 0x50)));

        void* attachment = reinterpret_cast<void*>(
            C_0082dd78_DispatchReturnedObject(
                CSF_AT(C_0082dd78*, entity, 0x4C), nullptr));
        const int index =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, attachment, 0)[0x54 / 4])(
                    attachment, nullptr, 1);

        if (index == -1)
        {
            source = reinterpret_cast<float*(__fastcall*)(void*, void*, float*)>(
                CSF_AT(void**, entity, 0)[0xA4 / 4])(
                    entity, nullptr, fallbackPosition);
        }
        else
        {
            source =
                reinterpret_cast<float*(__fastcall*)(void*, void*, float*, int)>(
                    CSF_AT(void**, attachment, 0)[0x4C / 4])(
                        attachment, nullptr, scratch, index);
        }

        const float x = source[0];
        const float y = source[1];
        const float z = source[2];
        const float dx = x - position[0];
        const float dy = y - position[1];
        const float dz = z - position[2];
        direction[0] = dx;
        direction[1] = dy;
        direction[2] = dz;

        std::uint32_t bits;
        std::memcpy(&bits, &dz, sizeof(bits));
        return static_cast<char>(bits);
    }

    if (targetType == 2)
    {
        void* controller = CSF_AT(void*, owner, 0x44);
        void* context = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0xD8 / 4])(controller, nullptr);
        void* entries = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, context, 0)[0xA8 / 4])(context, nullptr);
        if (!entries)
            return 0;

        const int index =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, entries, 0)[0x98 / 4])(entries, nullptr, 100);
        if (index == -1)
            return static_cast<char>(index);

        void* entry = reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, entries, 0)[0x8C / 4])(entries, nullptr, index);
        void* transform = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, entry, 0)[0x18 / 4])(entry, nullptr);
        source = reinterpret_cast<float*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, transform, 0)[0x10 / 4])(transform, nullptr);
        position[0] = source[0];
        position[1] = source[1];
        position[2] = source[2];

        transform = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, entry, 0)[0x18 / 4])(entry, nullptr);
        void* matrixReference =
            reinterpret_cast<void*(__fastcall*)(void*, void*, std::uint32_t*)>(
                CSF_AT(void**, transform, 0)[0x8C / 4])(
                    transform, nullptr, temporary);
        void* matrix = CSF_AT(void*, matrixReference, 0);
        direction[0] = CSF_AT(float, matrix, 0x20);
        const float y = CSF_AT(float, matrix, 0x24);
        const char owned = static_cast<char>(temporary[1]);
        direction[1] = y;
        direction[2] = CSF_AT(float, matrix, 0x28);
        if (owned)
            return static_cast<char>(RwMatrixDestroy(
                reinterpret_cast<std::uint32_t*>(temporary[0])));
        return owned;
    }

    if (targetType == 3)
    {
        const float dx = target[4] - position[0];
        const float dy = target[5] - position[1];
        const float dz = target[6] - position[2];
        direction[0] = dx;
        direction[1] = dy;
        direction[2] = dz;

        std::uint32_t bits;
        std::memcpy(&bits, &dz, sizeof(bits));
        return static_cast<char>(bits);
    }

    return static_cast<char>(reinterpret_cast<std::uintptr_t>(target));
}

HOOK(0x004784D0, C_008317c8_GetAimPositionAndDirection, "auto");
