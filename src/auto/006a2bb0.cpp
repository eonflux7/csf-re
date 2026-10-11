#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CoreZlibLog_DecompressBlock, 0x006A3150,
         (void* self, void* edx, int a2, uint32_t* a3, int a4, int a5));

static int __fastcall C_0085ee28_ReadCompressedBlock(
    C_0085ee28* self, void* /*edx*/)
{
    void* stream = CSF_AT(void*, reinterpret_cast<void*>(self->field_0004), 0x4);

    ((void(__fastcall*)(void*, void*, int, uint32_t))
        CSF_AT(void**, stream, 0)[0x14 / 4])(
        stream, nullptr, 0, self->field_0014);

    int compressedSize = 0;
    ((void(__fastcall*)(void*, void*, void*, int))
        CSF_AT(void**, stream, 0)[0x8 / 4])(
        stream, nullptr, &compressedSize, 4);

    if (!compressedSize)
        return -1;

    ((void(__fastcall*)(void*, void*, void*, int))
        CSF_AT(void**, stream, 0)[0x8 / 4])(
        stream, nullptr, &self->field_0018, 4);

    ((void(__fastcall*)(void*, void*, void*, int))
        CSF_AT(void**, stream, 0)[0x8 / 4])(
        stream, nullptr, &CSF_AT(uint8_t, self, 0x1020), compressedSize);

    uint32_t decompressedSize = 4096;
    if (!CoreZlibLog_DecompressBlock(
            reinterpret_cast<void*>(self->field_2420), nullptr,
            reinterpret_cast<int>(&CSF_AT(uint8_t, self, 0x20)),
            &decompressedSize,
            reinterpret_cast<int>(&CSF_AT(uint8_t, self, 0x1020)),
            compressedSize))
        return -1;

    uint32_t position =
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, stream, 0)[0x10 / 4])(stream, nullptr);

    uint32_t previousProcessedSize = self->field_001c;
    self->field_0014 = position;
    self->field_001c = self->field_0010 + previousProcessedSize;
    self->field_0010 = 0;
    return compressedSize;
}

HOOK(0x006A2BB0, C_0085ee28_ReadCompressedBlock, "auto");
