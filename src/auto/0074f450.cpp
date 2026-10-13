#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, NetPlayerDDL_ResolveObject, 0x00655DF0, (void* self, void* edx, bool* a2));
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));

static uint32_t* __fastcall C_008622e8_ReadPlayerListFields(
    C_008622e8* self, void* /*edx*/, C_008622e8* reader)
{
    NetPlayerDDL_ResolveObject(reader, nullptr, reinterpret_cast<bool*>(self));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&self->field_0004));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&self->field_0008));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&self->field_000c));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&self->field_0010));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&self->field_0014));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&self->field_0018));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&self->field_001c));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&self->field_0020));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&self->field_0024));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&CSF_AT(uint32_t, self, 0x28)));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&CSF_AT(uint32_t, self, 0x2C)));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&CSF_AT(uint32_t, self, 0x30)));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&CSF_AT(uint32_t, self, 0x34)));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&CSF_AT(uint32_t, self, 0x38)));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&CSF_AT(uint32_t, self, 0x3C)));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&CSF_AT(uint32_t, self, 0x40)));
    return NetCogibleDdl_ReadIntoSlot(
        reader, nullptr, reinterpret_cast<int>(&CSF_AT(uint32_t, self, 0x44)));
}

HOOK(0x0074F450, C_008622e8_ReadPlayerListFields, "auto");
