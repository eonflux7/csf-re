#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));

static uint32_t* __fastcall C_0083f364_InitializeRenderObject(
    C_0083f364* self, void* /*edx*/, uint32_t* parameters)
{
    *(void***)self = CSF_VTBL(0x0083F364);
    self->field_0004 = parameters[0];

    alignas(8) uint8_t requestBytes[0x28] = {};
    auto* request = reinterpret_cast<uint32_t*>(requestBytes);
    request[1] = parameters[1];
    request[2] = parameters[2];

    const uint32_t fourthParameter = parameters[3];
    const uint32_t fifthParameter = parameters[4];
    request[3] = fourthParameter;
    request[4] = fifthParameter;
    request[5] = parameters[5];
    request[6] = parameters[6];

    const uint32_t eighthParameter = parameters[7];
    request[8] = parameters[8];
    const bool defaultMode = parameters[0] == 0;
    request[7] = eighthParameter;
    request[0] = 7;
    requestBytes[0x24] = 1;
    requestBytes[0x25] = defaultMode;

    const int managerId = GetGlobalInt();
    void* manager = reinterpret_cast<void*>(ResolveRwManager(managerId, nullptr));
    self->field_0008 =
        reinterpret_cast<uint32_t (__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, manager, 0)[0x18 / 4])(manager, nullptr, request);

    if (self->field_0004 == 1)
    {
        void* context = reinterpret_cast<void*>(GetValue8B0554());
        void* renderObject = reinterpret_cast<void*>(self->field_0008);
        const uint32_t components = CSF_AT(uint32_t, context, 0x900);
        reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, renderObject, 0)[0x20 / 4])(
                renderObject, nullptr, components);
    }

    self->field_0010 = -1;
    self->field_000c = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004FBC10, C_0083f364_InitializeRenderObject, "auto");
