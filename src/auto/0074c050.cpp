#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(char, __stdcall, NetDdl_FallbackDispatch, 0x0065E720, (int a1, int* a2));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(void, __fastcall, NetDdl_SendSubField, 0x0066CF50, (void* self, void* edx, void* a2, int a3, int a4, int a5, int a6));

static void C_00858180_SendDdlProperty(
    C_00858180* self, void* field, int propertyId,
    int* context, void*& property)
{
    auto* owner = reinterpret_cast<uint8_t*>(
        LookupObjectByIndex(self->field_002c >> 22));
    uint8_t searchId = static_cast<uint8_t>(propertyId);
    NetCogibleDdl_FindProperty(owner + 0x0C, nullptr, &property, &searchId);

    const int secondValue = context[1];
    const int firstValue = context[0];
    void* sender = CSF_AT(void*, property, 0x10);
    NetDdl_SendSubField(sender, nullptr, self,
        reinterpret_cast<int>(field), propertyId, firstValue, secondValue);
}

static char __fastcall C_00858180_SendDdlFields(
    C_00858180* self, void* /*edx*/, C_00858180* selectedField, int contextAddress)
{
    int* context = reinterpret_cast<int*>(contextAddress);
    void* property = context;
    auto* bytes = reinterpret_cast<uint8_t*>(self);

    if (!selectedField || selectedField == reinterpret_cast<C_00858180*>(&self->field_0044))
    {
        C_00858180_SendDdlProperty(self, &self->field_0044, 1, context, property);
        if (selectedField == reinterpret_cast<C_00858180*>(&self->field_0044))
            return 1;
    }

    if (!selectedField || selectedField == reinterpret_cast<C_00858180*>(bytes + 0x47))
    {
        C_00858180_SendDdlProperty(self, bytes + 0x47, 2, context, property);
        if (selectedField == reinterpret_cast<C_00858180*>(bytes + 0x47))
            return 1;
    }

    if (!selectedField || selectedField == reinterpret_cast<C_00858180*>(bytes + 0x4A))
    {
        C_00858180_SendDdlProperty(self, bytes + 0x4A, 3, context, property);
        if (selectedField == reinterpret_cast<C_00858180*>(bytes + 0x4A))
            return 1;
    }

    if (!selectedField || selectedField == reinterpret_cast<C_00858180*>(bytes + 0x50))
    {
        C_00858180_SendDdlProperty(self, bytes + 0x50, 4, context, property);
        if (selectedField == reinterpret_cast<C_00858180*>(bytes + 0x50))
            return 1;
    }

    if (!selectedField || selectedField == reinterpret_cast<C_00858180*>(bytes + 0x5C))
    {
        C_00858180_SendDdlProperty(self, bytes + 0x5C, 5, context, property);
        if (selectedField == reinterpret_cast<C_00858180*>(bytes + 0x5C))
            return 1;
    }

    if (!selectedField || selectedField == reinterpret_cast<C_00858180*>(&self->field_0068))
    {
        C_00858180_SendDdlProperty(self, &self->field_0068, 6, context, property);
        if (selectedField == reinterpret_cast<C_00858180*>(&self->field_0068))
            return 1;
    }

    return NetDdl_FallbackDispatch(reinterpret_cast<int>(selectedField), context);
}

HOOK(0x0074C050, C_00858180_SendDdlFields, "auto");
