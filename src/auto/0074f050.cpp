#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(char, __stdcall, NetDdl_FallbackDispatch, 0x0065E720, (int a1, int* a2));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(void, __fastcall, NetDdl_SendSubField, 0x0066CF50, (void* self, void* edx, void* a2, int a3, int a4, int a5, int a6));

static char __fastcall C_00858180_DispatchSubFields(
    C_00858180* self, void* /*edx*/, C_00858180* selectedField, int arguments)
{
    int* sendArguments = reinterpret_cast<int*>(arguments);
    void* property = reinterpret_cast<void*>(arguments);
    uint8_t propertyId;

    void* firstField = &self->field_0044;
    if (!selectedField || selectedField == firstField)
    {
        int object = LookupObjectByIndex(self->field_002c >> 22);
        propertyId = 1;
        NetCogibleDdl_FindProperty(
            reinterpret_cast<uint8_t*>(object) + 0x0C,
            nullptr, &property, &propertyId);

        int secondArgument = sendArguments[1];
        int firstArgument = sendArguments[0];
        NetDdl_SendSubField(
            CSF_AT(void*, property, 0x10), nullptr, self,
            reinterpret_cast<int>(firstField), 1,
            firstArgument, secondArgument);

        if (selectedField == firstField)
            return 1;
    }

    void* secondField = reinterpret_cast<uint8_t*>(self) + 0x58;
    if (!selectedField || selectedField == secondField)
    {
        int object = LookupObjectByIndex(self->field_002c >> 22);
        propertyId = 2;
        NetCogibleDdl_FindProperty(
            reinterpret_cast<uint8_t*>(object) + 0x0C,
            nullptr, &property, &propertyId);

        int secondArgument = sendArguments[1];
        int firstArgument = sendArguments[0];
        NetDdl_SendSubField(
            CSF_AT(void*, property, 0x10), nullptr, self,
            reinterpret_cast<int>(secondField), 2,
            firstArgument, secondArgument);

        if (selectedField == secondField)
            return 1;
    }

    void* thirdField = reinterpret_cast<uint8_t*>(self) + 168;
    if (!selectedField || selectedField == thirdField)
    {
        int object = LookupObjectByIndex(self->field_002c >> 22);
        propertyId = 3;
        NetCogibleDdl_FindProperty(
            reinterpret_cast<uint8_t*>(object) + 0x0C,
            nullptr, &property, &propertyId);

        int secondArgument = sendArguments[1];
        int firstArgument = sendArguments[0];
        NetDdl_SendSubField(
            CSF_AT(void*, property, 0x10), nullptr, self,
            reinterpret_cast<int>(thirdField), 3,
            firstArgument, secondArgument);

        if (selectedField == thirdField)
            return 1;
    }

    return NetDdl_FallbackDispatch(
        reinterpret_cast<int>(selectedField), sendArguments);
}

HOOK(0x0074F050, C_00858180_DispatchSubFields, "auto");
