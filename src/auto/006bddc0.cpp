#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, RpClumpStreamWrite, 0x0058FDF0, (int a1, int a2));
CSF_FUNC(uint32_t*, __cdecl, RpWorldStreamWrite, 0x0059FD10, (int a1, int a2));
CSF_FUNC(uint32_t*, __cdecl, RwStreamWrite, 0x00629530, (uint32_t* a1, void* a2, unsigned int a3));
CSF_FUNC(int, __cdecl, RwStreamClose, 0x00629810, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(uint32_t*, __cdecl, RwStreamOpen, 0x006298E0, (int a3, int a4, int a5));
CSF_FUNC(uint32_t*, __cdecl, _rwStreamWriteVersionedChunkHeader, 0x00629A80, (uint32_t* a1, int a2, int a3, int a4, uint16_t a5));
CSF_FUNC(int, __cdecl, RwMatrixStreamGetSize, 0x0063BD90, ());
CSF_FUNC(uint32_t*, __cdecl, RwMatrixStreamWrite, 0x0063BDA0, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static bool __fastcall GfxEnvironment_WriteToFile(
    void* self, void* /*edx*/, const char* filename)
{
    if (!filename || !*filename || !CSF_AT(uint32_t, self, 0x04))
        return false;

    uint32_t* stream = RwStreamOpen(2, 2, reinterpret_cast<int>(filename));
    if (!stream)
        return false;

    for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x14); ++i)
    {
        auto* clumps = reinterpret_cast<int*>(CSF_AT(uint32_t, self, 0x0C));
        RpClumpStreamWrite(clumps[i], reinterpret_cast<int>(stream));
    }

    for (uint32_t group = 0; group < 3; ++group)
    {
        for (uint32_t i = 0;
             i < CSF_AT(uint32_t, self, 0x34 + group * 0x10);
             ++i)
        {
            auto* clumps = reinterpret_cast<int*>(
                CSF_AT(uint32_t, self, 0x2C + group * 0x10));
            RpClumpStreamWrite(clumps[i], reinterpret_cast<int>(stream));
        }
    }

    for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x74); ++i)
    {
        auto* clumps = reinterpret_cast<int*>(CSF_AT(uint32_t, self, 0x6C));
        RpClumpStreamWrite(clumps[i], reinterpret_cast<int>(stream));
    }

    for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x64); ++i)
    {
        auto* clumps = reinterpret_cast<int*>(CSF_AT(uint32_t, self, 0x5C));
        RpClumpStreamWrite(clumps[i], reinterpret_cast<int>(stream));
    }

    alignas(8) uint8_t nameBytes[0x1C];
    C_0082d410& name = *reinterpret_cast<C_0082d410*>(nameBytes);

    uint32_t recordOffset = 0;
    for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x84); ++i)
    {
        auto* record = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, self, 0x7C)) + recordOffset;

        uint32_t firstValue = CSF_AT(uint32_t, record, 0x00);
        uint32_t secondValue = CSF_AT(uint32_t, record, 0x04);

        CSF_AT(uint32_t, nameBytes, 0x18) = 15;
        CSF_AT(uint32_t, nameBytes, 0x14) = 0;
        CSF_AT(uint8_t, &name.field_0004, 0) = 0;
        AssignStringSubstring(&name, nullptr, record + 0x08, 0,
                              static_cast<uint32_t>(-1));

        auto* matrix = reinterpret_cast<uint32_t*>(
            CSF_AT(uint32_t, record, 0x24));
        uint32_t value28 = CSF_AT(uint32_t, record, 0x28);
        uint32_t value2C = CSF_AT(uint32_t, record, 0x2C);
        uint32_t value30 = CSF_AT(uint32_t, record, 0x30);
        uint32_t value34 = CSF_AT(uint32_t, record, 0x34);

        int matrixSize = RwMatrixStreamGetSize();
        _rwStreamWriteVersionedChunkHeader(
            stream, 94144,
            CSF_AT(uint32_t, nameBytes, 0x14) + matrixSize + 24 + 4,
            225282, 55);

        RwStreamWrite(stream, &firstValue, 4);
        RwStreamWrite(stream, &secondValue, 4);
        RwStreamWrite(stream, &value28, 4);
        RwStreamWrite(stream, &value2C, 4);
        RwStreamWrite(stream, &value30, 4);
        RwStreamWrite(stream, &value34, 4);
        RwMatrixStreamWrite(matrix, stream);

        uint32_t nameLength = CSF_AT(uint32_t, nameBytes, 0x14);
        RwStreamWrite(stream, &nameLength, 4);
        if (nameLength)
        {
            void* text = CSF_AT(uint32_t, nameBytes, 0x18) < 0x10
                ? static_cast<void*>(&name.field_0004)
                : reinterpret_cast<void*>(name.field_0004);
            RwStreamWrite(stream, text, nameLength);
        }

        if (CSF_AT(uint32_t, nameBytes, 0x18) >= 0x10)
        {
            if (name.field_0004)
                Mem_FreeDebug(reinterpret_cast<void*>(name.field_0004));
        }

        CSF_AT(uint32_t, nameBytes, 0x18) = 15;
        CSF_AT(uint32_t, nameBytes, 0x14) = 0;
        CSF_AT(uint8_t, &name.field_0004, 0) = 0;
        recordOffset += 56;
    }

    bool success = RpWorldStreamWrite(
        CSF_AT(int, self, 0x04), reinterpret_cast<int>(stream)) != nullptr;
    RwStreamClose(stream, nullptr);
    return success;
}

HOOK(0x006BDDC0, GfxEnvironment_WriteToFile, "auto");
