#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void*, __fastcall, NetDdl_GetPropertyObject, 0x0064F3A0, (void* self, void* edx, char a2));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(void, __fastcall, NetDdl_SendSubField, 0x0066CF50, (void* self, void* edx, void* a2, int a3, int a4, int a5, int a6));
CSF_FUNC(char, __fastcall, NetPlayerDdl_SendBaseFields, 0x007A3A90, (void* self, void* edx, void* arg0, int a3));

static char __fastcall C_00858180_SendFields(
    C_00858180* self, void* /*edx*/, C_00858180* selectedField, int contextAddress)
{
    struct FieldDescriptor
    {
        int offset;
        int id;
    };

    static constexpr FieldDescriptor fields[] = {
        {1088, 4},
        {1091, 5},
        {1094, 6},
        {1100, 7},
        {1103, 8},
        {1106, 9},
        {1112, 10},
        {1640, 11},
        {1652, 12},
        {1664, 13},
        {1670, 14},
        {1676, 15},
        {1682, 16},
        {1688, 17},
        {1694, 18},
        {1700, 19},
        {1704, 20}
    };

    int* context = reinterpret_cast<int*>(contextAddress);
    void* property = reinterpret_cast<void*>(contextAddress);

    for (const FieldDescriptor& field : fields)
    {
        void* fieldAddress = reinterpret_cast<uint8_t*>(self) + field.offset;
        if (selectedField && selectedField != fieldAddress)
            continue;

        void* schema = reinterpret_cast<void*>(
            LookupObjectByIndex(self->field_002c >> 22));

        if (field.id < 10)
        {
            uint8_t propertyId = static_cast<uint8_t>(field.id);
            NetCogibleDdl_FindProperty(schema, nullptr, &property, &propertyId);

            int secondContext = context[1];
            int firstContext = context[0];
            void* propertyObject = CSF_AT(void*, property, 0x10);
            NetDdl_SendSubField(
                propertyObject, nullptr, self,
                reinterpret_cast<int>(fieldAddress), field.id,
                firstContext, secondContext);
        }
        else
        {
            int secondContext = context[1];
            int firstContext = context[0];
            void* propertyObject = NetDdl_GetPropertyObject(
                schema, nullptr, static_cast<char>(field.id));
            NetDdl_SendSubField(
                propertyObject, nullptr, self,
                reinterpret_cast<int>(fieldAddress), field.id,
                firstContext, secondContext);
        }

        if (selectedField == fieldAddress)
            return 1;
    }

    return NetPlayerDdl_SendBaseFields(
        self, nullptr, selectedField, contextAddress);
}

HOOK(0x00746C80, C_00858180_SendFields, "auto");
