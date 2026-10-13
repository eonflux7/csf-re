#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __cdecl, Hud_UnregisterObject, 0x004754D0, (int a1));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));

static int __fastcall C_0082d46c_Destroy(C_0082d46c* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0082D46C);
    CSF_AT(void**, self, 0x04) = CSF_VTBL(0x0082D464);

    int globalObject = GetGlobalInt();
    void* renderHandle = reinterpret_cast<void*>(
        RenderHandle_FromObject(reinterpret_cast<void*>(globalObject), nullptr));
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, renderHandle, 0)[0x34 / 4])(
            renderHandle, nullptr, 0);
    Hud_UnregisterObject(reinterpret_cast<int>(self));

    uint32_t arrayAddress = self->field_0028;
    if (arrayAddress)
    {
        void* allocation = reinterpret_cast<void*>(arrayAddress - 4);
        uint32_t count = CSF_AT(uint32_t, allocation, 0);
        if (static_cast<int32_t>(count - 1) >= 0)
        {
            uint8_t* cursor =
                reinterpret_cast<uint8_t*>(arrayAddress + count * 0x14 + 4);
            do
            {
                cursor -= 0x14;
                --count;
                CSF_AT(void**, cursor, 0) = CSF_VTBL(0x0082D410);
            }
            while (count);
        }
        CoreMemory_Free(allocation);
        self->field_0028 = 0;
    }

    if (self->field_0018)
    {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0018));
        self->field_0018 = 0;
    }

    arrayAddress = self->field_0008;
    if (arrayAddress)
    {
        void* allocation = reinterpret_cast<void*>(arrayAddress - 4);
        uint32_t count = CSF_AT(uint32_t, allocation, 0);
        if (static_cast<int32_t>(count - 1) >= 0)
        {
            uint8_t* cursor =
                reinterpret_cast<uint8_t*>(arrayAddress + count * 0x14 + 4);
            do
            {
                cursor -= 0x14;
                --count;
                CSF_AT(void**, cursor, 0) = CSF_VTBL(0x0082D410);
            }
            while (count);
        }
        int result = CoreMemory_Free(allocation);
        self->field_0008 = 0;
        return result;
    }

    return 0;
}

HOOK(0x00401460, C_0082d46c_Destroy, "auto");
