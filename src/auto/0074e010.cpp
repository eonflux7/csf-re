#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void*, __fastcall, NetDdl_GetPropertyObject, 0x0064F3A0, (void* self, void* edx, char a2));
CSF_FUNC(char, __stdcall, NetDdl_FallbackDispatch, 0x0065E720, (int a1, int* a2));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(void, __fastcall, NetDdl_SendSubField, 0x0066CF50, (void* self, void* edx, void* a2, int a3, int a4, int a5, int a6));

static char __fastcall C_00858180_SendSelectedSubFields(
    C_00858180* self, void* /*edx*/, C_00858180* selectedField,
    int dispatchContext)
{
    int* context = reinterpret_cast<int*>(dispatchContext);
    void* propertyEntry = reinterpret_cast<void*>(dispatchContext);
    auto* bytes = reinterpret_cast<uint8_t*>(self);

    struct SubField
    {
        void* address;
        char propertyId;
    };

    const SubField fields[] = {
        {&self->field_0044, 1},
        {bytes + 0x50, 2},
        {bytes + 0x5C, 3},
        {bytes + 0x5F, 4},
        {bytes + 0x64, 5},
        {bytes + 696, 6},
        {bytes + 708, 7},
        {bytes + 714, 8},
        {bytes + 720, 9},
        {bytes + 726, 10},
        {bytes + 732, 11},
        {bytes + 744, 12},
        {bytes + 768, 13}
    };

    for (const SubField& field : fields)
    {
        if (selectedField && selectedField != field.address)
            continue;

        void* owner = reinterpret_cast<void*>(
            LookupObjectByIndex(self->field_002c >> 22));

        if (field.propertyId <= 6)
        {
            uint8_t propertyKey = static_cast<uint8_t>(field.propertyId);
            NetCogibleDdl_FindProperty(
                reinterpret_cast<uint8_t*>(owner) + 0x0C,
                nullptr, &propertyEntry, &propertyKey);

            int contextSecond = context[1];
            int contextFirst = context[0];
            void* propertyObject = CSF_AT(void*, propertyEntry, 0x10);
            NetDdl_SendSubField(
                propertyObject, nullptr, self,
                reinterpret_cast<int>(field.address), field.propertyId,
                contextFirst, contextSecond);
        }
        else
        {
            int contextSecond = context[1];
            int contextFirst = context[0];
            void* propertyObject =
                NetDdl_GetPropertyObject(owner, nullptr, field.propertyId);
            NetDdl_SendSubField(
                propertyObject, nullptr, self,
                reinterpret_cast<int>(field.address), field.propertyId,
                contextFirst, contextSecond);
        }

        if (selectedField == field.address)
            return 1;
    }

    return NetDdl_FallbackDispatch(
        reinterpret_cast<int>(selectedField), context);
}

HOOK(0x0074E010, C_00858180_SendSelectedSubFields, "auto");
