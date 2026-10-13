#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void*, __fastcall, NetDdl_GetPropertyObject, 0x0064F3A0, (void* self, void* edx, char a2));
CSF_FUNC(char, __stdcall, NetDdl_FallbackDispatch, 0x0065E720, (int a1, int* a2));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(void, __fastcall, NetDdl_SendSubField, 0x0066CF50, (void* self, void* edx, void* a2, int a3, int a4, int a5, int a6));

static void C_00858180_SendResolvedSubField(
    C_00858180* self, void* field, int fieldId,
    int* context, void*& property)
{
    void* owner = reinterpret_cast<void*>(
        LookupObjectByIndex(self->field_002c >> 22));
    uint8_t propertyId = static_cast<uint8_t>(fieldId);
    NetCogibleDdl_FindProperty(
        static_cast<uint8_t*>(owner) + 0x0C, nullptr,
        &property, &propertyId);

    int second = context[1];
    int first = context[0];
    void* propertyObject = CSF_AT(void*, property, 0x10);
    NetDdl_SendSubField(
        propertyObject, nullptr, self,
        reinterpret_cast<int>(field), fieldId, first, second);
}

static char __fastcall C_00858180_DispatchSubFields(
    C_00858180* self, void* /*edx*/, C_00858180* selectedField,
    int contextAddress)
{
    int* context = reinterpret_cast<int*>(contextAddress);
    void* property = context;
    uint8_t* bytes = reinterpret_cast<uint8_t*>(self);

    if (!selectedField || static_cast<void*>(selectedField) == &self->field_0044)
    {
        C_00858180_SendResolvedSubField(
            self, &self->field_0044, 1, context, property);
        if (static_cast<void*>(selectedField) == &self->field_0044)
            return 1;
    }

    if (!selectedField || static_cast<void*>(selectedField) == bytes + 0x50)
    {
        C_00858180_SendResolvedSubField(
            self, bytes + 0x50, 2, context, property);
        if (static_cast<void*>(selectedField) == bytes + 0x50)
            return 1;
    }

    if (!selectedField || static_cast<void*>(selectedField) == bytes + 0x5C)
    {
        C_00858180_SendResolvedSubField(
            self, bytes + 0x5C, 3, context, property);
        if (static_cast<void*>(selectedField) == bytes + 0x5C)
            return 1;
    }

    if (!selectedField || static_cast<void*>(selectedField) == &self->field_006c)
    {
        C_00858180_SendResolvedSubField(
            self, &self->field_006c, 4, context, property);
        if (static_cast<void*>(selectedField) == &self->field_006c)
            return 1;
    }

    if (!selectedField || static_cast<void*>(selectedField) == &self->field_0078)
    {
        C_00858180_SendResolvedSubField(
            self, &self->field_0078, 5, context, property);
        if (static_cast<void*>(selectedField) == &self->field_0078)
            return 1;
    }

    if (!selectedField || static_cast<void*>(selectedField) == bytes + 136)
    {
        C_00858180_SendResolvedSubField(
            self, bytes + 136, 6, context, property);
        if (static_cast<void*>(selectedField) == bytes + 136)
            return 1;
    }

    if (!selectedField || static_cast<void*>(selectedField) == bytes + 160)
    {
        void* owner = reinterpret_cast<void*>(
            LookupObjectByIndex(self->field_002c >> 22));
        int second = context[1];
        int first = context[0];
        void* propertyObject = NetDdl_GetPropertyObject(owner, nullptr, 7);
        NetDdl_SendSubField(
            propertyObject, nullptr, self,
            reinterpret_cast<int>(bytes + 160), 7, first, second);
        if (static_cast<void*>(selectedField) == bytes + 160)
            return 1;
    }

    return NetDdl_FallbackDispatch(
        reinterpret_cast<int>(selectedField), context);
}

HOOK(0x007506A0, C_00858180_DispatchSubFields, "auto");
