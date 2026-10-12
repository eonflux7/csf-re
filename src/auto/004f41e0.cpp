#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

static char __fastcall S_004f41e0_CreateFrameEdges(S_004f41e0* self, void* /*edx*/)
{
    char result = static_cast<char>(self->field_0034);
    if (!result)
        return result;

    using CreateEdge = uint32_t (__fastcall*)(void*, void*);
    using SetColor = void (__fastcall*)(void*, void*, float*);
    using SetStart = void (__fastcall*)(void*, void*, uint32_t*);
    using SetEnd = int (__fastcall*)(void*, void*, uint32_t*);

    void* object = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    self->field_0038 =
        reinterpret_cast<CreateEdge>(CSF_AT(void**, object, 0)[0x44 / 4])(object, nullptr);

    object = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    self->field_0040 =
        reinterpret_cast<CreateEdge>(CSF_AT(void**, object, 0)[0x44 / 4])(object, nullptr);

    object = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    self->field_003c =
        reinterpret_cast<CreateEdge>(CSF_AT(void**, object, 0)[0x44 / 4])(object, nullptr);

    object = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    uint32_t lastEdge =
        reinterpret_cast<CreateEdge>(CSF_AT(void**, object, 0)[0x44 / 4])(object, nullptr);
    self->field_0044 = lastEdge;
    result = static_cast<char>(lastEdge);

    alignas(8) uint8_t colorBytes[0x10];
    alignas(8) uint8_t pointBytes[0x8];
    float* color = reinterpret_cast<float*>(colorBytes);
    uint32_t* point = reinterpret_cast<uint32_t*>(pointBytes);
    color[0] = 255.0f;
    color[1] = 0.0f;
    color[2] = 0.0f;
    color[3] = 255.0f;

    object = reinterpret_cast<void*>(self->field_0038);
    if (object)
    {
        reinterpret_cast<SetColor>(CSF_AT(void**, object, 0)[0x2c / 4])(
            object, nullptr, color);
        point[0] = self->field_0024;
        point[1] = self->field_0028;
        object = reinterpret_cast<void*>(self->field_0038);
        reinterpret_cast<SetStart>(CSF_AT(void**, object, 0)[0x1c / 4])(
            object, nullptr, point);
        point[0] = self->field_002c;
        point[1] = self->field_0028;
        object = reinterpret_cast<void*>(self->field_0038);
        result = static_cast<char>(
            reinterpret_cast<SetEnd>(CSF_AT(void**, object, 0)[0x24 / 4])(
                object, nullptr, point));
    }

    object = reinterpret_cast<void*>(self->field_0040);
    if (object)
    {
        reinterpret_cast<SetColor>(CSF_AT(void**, object, 0)[0x2c / 4])(
            object, nullptr, color);
        point[0] = self->field_0024;
        point[1] = self->field_0030;
        object = reinterpret_cast<void*>(self->field_0040);
        reinterpret_cast<SetStart>(CSF_AT(void**, object, 0)[0x1c / 4])(
            object, nullptr, point);
        point[0] = self->field_002c;
        point[1] = self->field_0030;
        object = reinterpret_cast<void*>(self->field_0040);
        result = static_cast<char>(
            reinterpret_cast<SetEnd>(CSF_AT(void**, object, 0)[0x24 / 4])(
                object, nullptr, point));
    }

    object = reinterpret_cast<void*>(self->field_003c);
    if (object)
    {
        reinterpret_cast<SetColor>(CSF_AT(void**, object, 0)[0x2c / 4])(
            object, nullptr, color);
        point[0] = self->field_0024;
        point[1] = self->field_0028;
        object = reinterpret_cast<void*>(self->field_003c);
        reinterpret_cast<SetStart>(CSF_AT(void**, object, 0)[0x1c / 4])(
            object, nullptr, point);
        point[0] = self->field_0024;
        point[1] = self->field_0030;
        object = reinterpret_cast<void*>(self->field_003c);
        result = static_cast<char>(
            reinterpret_cast<SetEnd>(CSF_AT(void**, object, 0)[0x24 / 4])(
                object, nullptr, point));
    }

    object = reinterpret_cast<void*>(self->field_0044);
    if (object)
    {
        reinterpret_cast<SetColor>(CSF_AT(void**, object, 0)[0x2c / 4])(
            object, nullptr, color);
        point[0] = self->field_002c;
        point[1] = self->field_0028;
        object = reinterpret_cast<void*>(self->field_0044);
        reinterpret_cast<SetStart>(CSF_AT(void**, object, 0)[0x1c / 4])(
            object, nullptr, point);
        point[0] = self->field_002c;
        point[1] = self->field_0030;
        object = reinterpret_cast<void*>(self->field_0044);
        result = static_cast<char>(
            reinterpret_cast<SetEnd>(CSF_AT(void**, object, 0)[0x24 / 4])(
                object, nullptr, point));
    }

    return result;
}

HOOK(0x004F41E0, S_004f41e0_CreateFrameEdges, "auto");
