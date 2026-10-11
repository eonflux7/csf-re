#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, RpClumpStreamRead, 0x0058FFC0, (int a1));
CSF_FUNC(int*, __cdecl, RpWorldStreamRead, 0x0059FEA0, (int a1));
CSF_FUNC(int, __cdecl, RwStreamRead, 0x006293E0, (uint32_t* a1, void* a2, unsigned int a3));
CSF_FUNC(int, __cdecl, RwStreamSkip, 0x00629700, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwStreamClose, 0x00629810, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(uint32_t*, __cdecl, RwStreamOpen, 0x006298E0, (int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, RwStreamReadChunkHeaderInfo, 0x00629CE0, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(uint32_t*, __cdecl, RwMatrixStreamRead, 0x0063BF20, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void, __cdecl, GameSleepMilliseconds, 0x0069E450, (uint32_t dwMilliseconds));

CSF_FUNC(int, __cdecl, GfxEnvironment_ForEachWorldMaterial, 0x006BBEF0, (int a1));
CSF_FUNC(int, __fastcall, C_008605d8_AllocateStreamBuffers, 0x006BC360, (void* self, void* edx, uint32_t a2));
CSF_FUNC(char, __fastcall, C_008605d8_FlushStreamBuffers, 0x006BC3A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008605d8_AcquireStreamBuffer, 0x006BC400, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, C_008605d8_SelectNamedObject, 0x006BCD90, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, C_008605d8_ReadEnvironmentChunk, 0x006BD010, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_008605d8_FinalizeEnvironment, 0x006BDB50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxEnvironment_AddObjectDescriptor, 0x006BE5C0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char, __fastcall, C_008605d8_AddClump, 0x006BE640, (void* self, void* edx, int a1));
CSF_FUNC(uint32_t*, __fastcall, GfxEnvironment_AppendRecord, 0x006BEA90, (void* self, void* edx, uint32_t* a3));

static char __fastcall C_008605d8_LoadEnvironmentStream(
    C_008605d8* self, void* /*edx*/, unsigned int source, char buffered)
{
    char success = 1;

    if (!buffered && self->field_017c)
        reinterpret_cast<void(__cdecl*)()>(self->field_017c)();

    uint32_t* stream = RwStreamOpen(2, 1, static_cast<int>(source));
    if (!stream)
        return 0;

    alignas(8) uint8_t headerBytes[0x14];
    auto* header = reinterpret_cast<uint32_t*>(headerBytes);

    while (RwStreamReadChunkHeaderInfo(stream, header))
    {
        bool notify = false;

        switch (CSF_AT(uint32_t, headerBytes, 0))
        {
        case 0xBu:
        {
            C_008605d8_FlushStreamBuffers(self, nullptr);
            int* world = RpWorldStreamRead(reinterpret_cast<int>(stream));
            self->field_0004 = reinterpret_cast<uint32_t>(world);
            if (!world)
                success = 0;
            notify = !buffered;
            break;
        }

        case 0x10u:
            if (buffered)
            {
                if (!self->field_00b4)
                    C_008605d8_AllocateStreamBuffers(self, nullptr, 0x80000u);

                if (CSF_AT(uint32_t, headerBytes, 4) > self->field_00b8)
                {
                    C_008605d8_FlushStreamBuffers(self, nullptr);
                    C_008605d8_AllocateStreamBuffers(
                        self, nullptr, CSF_AT(uint32_t, headerBytes, 4));
                }

                int slot = C_008605d8_AcquireStreamBuffer(
                    self, nullptr, CSF_AT(uint32_t, headerBytes, 4));
                while (slot == -1)
                {
                    GameSleepMilliseconds(0xAu);
                    slot = C_008605d8_AcquireStreamBuffer(
                        self, nullptr, CSF_AT(uint32_t, headerBytes, 4));
                }

                RwStreamRead(stream, CSF_AT(void*, self, 0xC4 + 12 * slot),
                             CSF_AT(uint32_t, headerBytes, 4));
                CSF_AT(uint8_t, self, 0xC4 + 12 * slot + 8) = 1;
            }
            else
            {
                int clump = RpClumpStreamRead(reinterpret_cast<int>(stream));
                if (!clump)
                {
                    success = 0;
                    goto finish;
                }
                C_008605d8_AddClump(self, nullptr, clump);
                notify = true;
            }
            break;

        case 0x23u:
            break;

        case 0x16FBFu:
            success = C_008605d8_ReadEnvironmentChunk(self, nullptr, stream);
            break;

        case 94144:
        {
            // Preserve the contiguous descriptor, including its embedded string.
            alignas(8) uint8_t descriptorBytes[0x40] = {};
            C_0082d410& name =
                *reinterpret_cast<C_0082d410*>(descriptorBytes + 8);
            CSF_AT(int, descriptorBytes, 0) = 0;
            CSF_AT(int, descriptorBytes, 4) = -1;
            CSF_AT(uint32_t, descriptorBytes, 0x20) = 15;
            CSF_AT(int, descriptorBytes, 0x1C) = 0;
            reinterpret_cast<uint8_t*>(&name.field_0004)[0] = 0;
            CSF_AT(uint32_t, descriptorBytes, 0x24) = 0;
            CSF_AT(int, descriptorBytes, 0x28) = 0;
            CSF_AT(int, descriptorBytes, 0x2C) = 0;
            CSF_AT(int, descriptorBytes, 0x30) = 0;
            CSF_AT(int, descriptorBytes, 0x34) = 0;
            source = 0;

            RwStreamRead(stream, descriptorBytes, 4);
            RwStreamRead(stream, descriptorBytes + 4, 4);
            RwStreamRead(stream, descriptorBytes + 0x28, 4);
            RwStreamRead(stream, descriptorBytes + 0x2C, 4);
            RwStreamRead(stream, descriptorBytes + 0x30, 4);
            RwStreamRead(stream, descriptorBytes + 0x34, 4);

            alignas(8) uint8_t matrixHeaderBytes[0x14];
            RwStreamReadChunkHeaderInfo(
                stream, reinterpret_cast<uint32_t*>(matrixHeaderBytes));
            CSF_AT(uint32_t, descriptorBytes, 0x24) =
                reinterpret_cast<uint32_t>(RwMatrixStreamRead(stream, nullptr));

            RwStreamRead(stream, &source, 4);
            if (source)
            {
                alignas(8) uint8_t nameBytes[0x100];
                auto* text = reinterpret_cast<char*>(nameBytes);
                RwStreamRead(stream, text, source);
                text[source] = 0;
                AssignStringRange_429B60(
                    &name, nullptr, text,
                    static_cast<unsigned int>(std::strlen(text)));
            }

            GfxEnvironment_AddObjectDescriptor(
                &self->field_007c, nullptr,
                reinterpret_cast<int>(descriptorBytes), 0);

            if (!buffered && self->field_017c)
                reinterpret_cast<void(__cdecl*)()>(self->field_017c)();

            if (CSF_AT(uint32_t, descriptorBytes, 0x20) >= 0x10 &&
                name.field_0004)
            {
                Mem_FreeDebug(reinterpret_cast<void*>(name.field_0004));
            }
            CSF_AT(uint32_t, descriptorBytes, 0x20) = 15;
            CSF_AT(int, descriptorBytes, 0x1C) = 0;
            reinterpret_cast<uint8_t*>(&name.field_0004)[0] = 0;
            break;
        }

        case 94145:
        {
            source = 0;
            if (self->field_0184)
                Mem_FreeDebug(reinterpret_cast<void*>(self->field_0184));
            self->field_0184 = 0;
            self->field_0188 = 0;
            self->field_018c = 0;

            RwStreamRead(stream, &source, 4);
            alignas(8) uint8_t recordBytes[0x18];
            for (unsigned int i = 0; i < source; ++i)
            {
                RwStreamRead(stream, recordBytes, 24);
                GfxEnvironment_AppendRecord(
                    reinterpret_cast<uint8_t*>(self) + 0x180, nullptr,
                    reinterpret_cast<uint32_t*>(recordBytes));
            }
            notify = !buffered;
            break;
        }

        default:
            RwStreamSkip(reinterpret_cast<int>(stream),
                         static_cast<int>(CSF_AT(uint32_t, headerBytes, 4)));
            break;
        }

        if (notify && self->field_017c)
            reinterpret_cast<void(__cdecl*)()>(self->field_017c)();

        if (!success)
            break;
    }

finish:
    RwStreamClose(stream, nullptr);
    C_008605d8_FlushStreamBuffers(self, nullptr);
    if (success)
    {
        C_008605d8_FinalizeEnvironment(self, nullptr);
        GfxEnvironment_ForEachWorldMaterial(static_cast<int>(self->field_0004));
        C_008605d8_SelectNamedObject(
            self, nullptr,
            reinterpret_cast<int>(CSF_STR(0x00860614, "MAIN")));
    }
    return success;
}

HOOK(0x006BEC60, C_008605d8_LoadEnvironmentStream, "auto");
