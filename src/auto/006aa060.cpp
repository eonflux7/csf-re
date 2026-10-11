#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(uint32_t*, __fastcall, NetGatheringDdl_ClearEmbeddedCollection, 0x006508A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetGatheringDdl_ResetEmbeddedCollection, 0x00652810, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, NetGatheringDdl_ResetEmbeddedState, 0x006A7000, (void* self, void* edx));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static void* __fastcall C_0085fe28_Reset(C_0085fe28* self, void* /*edx*/)
{
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0320 = 0;

    NetGatheringDdl_ResetEmbeddedState(
        reinterpret_cast<uint8_t*>(self) + 0x40, nullptr);
    NetGatheringDdl_ClearEmbeddedCollection(
        reinterpret_cast<uint8_t*>(self) + 0x50, nullptr);
    NetGatheringDdl_ResetEmbeddedCollection(
        reinterpret_cast<uint8_t*>(self) + 0x60, nullptr);

    self->field_0080 = 0;
    void* ownedObject = reinterpret_cast<void*>(self->field_0088);
    if (ownedObject)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
    }

    self->field_0088 = 0;
    self->field_0090 = 0;
    self->field_00ae = 0;
    self->field_02b0 = -1;
    self->field_02b4 = 0;
    self->field_02c8 = 0;

    AssignStringRange_429B60(
        reinterpret_cast<uint8_t*>(self) + 0x2CC, nullptr,
        reinterpret_cast<char*>(&g_byte_82E65C), 0);
    AssignStringRange_429B60(
        reinterpret_cast<uint8_t*>(self) + 0x2E8, nullptr,
        reinterpret_cast<char*>(&g_byte_82E65C), 0);
    void* result = AssignStringRange_429B60(
        reinterpret_cast<uint8_t*>(self) + 0x304, nullptr,
        reinterpret_cast<char*>(&g_byte_82E65C), 0);

    self->field_0328 = 0;
    self->field_032c = 0;
    self->field_0330 = 0;
    self->field_0334 = 0;
    self->field_0338 = 0;
    self->field_02c9 = 0;
    self->field_033c = 0;
    return result;
}

HOOK(0x006AA060, C_0085fe28_Reset, "auto");
