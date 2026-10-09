#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CScene_RemoveObjectWithFlags, 0x00452800,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, CScene_RemoveObjectFromCategory, 0x004528D0,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneLoader_RemoveObjectReference, 0x00453560,
         (void* self, void* edx, int a2));
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0,
         (void* self, void* edx, void* record, char flag));

static uint32_t __fastcall CScene_ClearObjectReferences(
    C_008312b0* self, void* /*edx*/)
{
    uint32_t result = CSF_AT(uint32_t, self, 0x48);

    if (result != 0)
    {
        alignas(4) uint8_t record[0x34];

        CSF_AT(void**, record, 0x00) = CSF_VTBL(0x0082D478);
        CSF_AT(void**, record, 0x08) = CSF_VTBL(0x0082D448);
        CSF_AT(int, record, 0x0C) = -1;
        CSF_AT(int, record, 0x10) = -1;
        CSF_AT(void**, record, 0x14) = CSF_VTBL(0x0082D448);
        CSF_AT(int, record, 0x18) = -1;
        CSF_AT(int, record, 0x1C) = -1;
        CSF_AT(int, record, 0x04) = 19;
        CSF_AT(int, record, 0x20) = 0;
        CSF_AT(int, record, 0x24) = 0;
        CSF_AT(int, record, 0x2C) = 0;
        CSF_AT(int, record, 0x28) = 0;
        CSF_AT(uint8_t, record, 0x30) = 0;

        for (int32_t index = static_cast<int32_t>(result - 1);
             index >= 0; --index)
        {
            uint8_t* object = CSF_AT(uint8_t**, self, 0x40)[index];
            uint8_t* reference = object + 0x1C;
            CSF_AT(int, record, 0x0C) = CSF_AT(int, object, 0x20);
            CSF_AT(int, record, 0x10) = CSF_AT(int, reference, 0x08);

            DispatchRecordToGlobalObject(
                CSF_AT(void*, self, 0x184), nullptr, record, 1);
            result = SceneLoader_RemoveObjectReference(
                self, nullptr,
                static_cast<int>(reinterpret_cast<intptr_t>(reference)));
        }

        CSF_AT(uint32_t, self, 0x48) = 0;
    }

    uint32_t count = CSF_AT(uint32_t, self, 0x38);
    if (count != 0)
    {
        for (int32_t index = static_cast<int32_t>(count - 1);
             index >= 0; --index)
        {
            uint8_t* reference =
                CSF_AT(uint8_t**, self, 0x30)[index] + 0x1C;

            CScene_RemoveObjectFromCategory(
                self, nullptr,
                static_cast<int>(reinterpret_cast<intptr_t>(reference)), 2);
            result = CScene_RemoveObjectWithFlags(
                self, nullptr,
                static_cast<int>(reinterpret_cast<intptr_t>(reference)), 16);
        }

        CSF_AT(uint32_t, self, 0x38) = 0;
    }

    return result;
}

HOOK(0x004539B0, CScene_ClearObjectReferences, "auto");
