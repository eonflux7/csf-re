#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwStreamRead, 0x006293E0, (uint32_t* a1, void* a2, unsigned int a3));
CSF_FUNC(int, __cdecl, RwStreamClose, 0x00629810, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(uint32_t*, __cdecl, RwStreamOpen, 0x006298E0, (int a3, int a4, int a5));
CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));
CSF_FUNC(int, __fastcall, SceneMissionSetup_SetBounds, 0x006AB4E0, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6, int a7));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __stdcall, GlobalInt_FindNameId, 0x006C1770, (uint8_t* a1));
CSF_FUNC(char, __fastcall, S_006deed0_ReorderTextureRecords, 0x006DE3C0, (S_006deed0* self, void*));
CSF_FUNC(void, __fastcall, S_006de5b0_ResizeRecords, 0x006DED70, (void* self, void* edx, unsigned int a2));
CSF_FUNC(char, __fastcall, S_006deed0_LoadTexture, 0x006DF270, (S_006deed0* self, void* , unsigned int textureId));

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(double, g_ParticleRadiusDivisorSquared, 0x00846168);
CSF_GLOBAL(uint32_t, g_ParticleSignatureFirst, 0x00861914);
CSF_GLOBAL(uint32_t, g_ParticleSignatureSecond, 0x00861918);
CSF_GLOBAL(uint16_t, g_ParticleSignatureLast, 0x0086191C);

static char __fastcall S_006deed0_LoadParticleFile(
    S_006deed0* self, void* /*edx*/, int filename)
{
    uint32_t* stream = RwStreamOpen(2, 1, filename);
    if (!stream)
        return 0;

    alignas(8) char signature[12];
    std::memcpy(signature, &g_ParticleSignatureFirst, 4);
    std::memcpy(signature + 4, &g_ParticleSignatureSecond, 4);
    std::memcpy(signature + 8, &g_ParticleSignatureLast, 2);

    int version = 1;
    RwStreamRead(stream, signature, 10);
    if (std::memcmp(signature, CSF_STR(0x00861914, "PARTICLES"), 10) != 0)
    {
        RwStreamClose(stream, nullptr);
        return 0;
    }

    RwStreamRead(stream, &version, 4);
    if (version != 4)
    {
        RwStreamClose(stream, nullptr);
        return 0;
    }

    int groupCount = 0;
    int bound1 = 0;
    int bound2 = 0;
    int bound3 = 0;
    int bound4 = 0;
    int bound5 = 0;
    int bound6 = 0;

    RwStreamRead(stream, &groupCount, 4);
    RwStreamRead(stream, &CSF_AT(float, self, 0x24), 4);
    if (!groupCount || CSF_AT(float, self, 0x24) == g_fZero)
    {
        RwStreamClose(stream, nullptr);
        return 0;
    }

    RwStreamRead(stream, &CSF_AT(uint32_t, self, 0x10), 4);
    RwStreamRead(stream, &CSF_AT(float, self, 0x14), 4);
    RwStreamRead(stream, &CSF_AT(uint8_t, self, 0x1C), 1);
    RwStreamRead(stream, &CSF_AT(uint8_t, self, 0x1D), 1);
    RwStreamRead(stream, &CSF_AT(uint8_t, self, 0x1E), 1);
    RwStreamRead(stream, &CSF_AT(uint8_t, self, 0x1F), 1);
    RwStreamRead(stream, &CSF_AT(uint32_t, self, 0x20), 4);
    RwStreamRead(stream, &bound1, 4);
    RwStreamRead(stream, &bound2, 4);
    RwStreamRead(stream, &bound3, 4);
    RwStreamRead(stream, &bound4, 4);
    RwStreamRead(stream, &bound5, 4);
    RwStreamRead(stream, &bound6, 4);

    alignas(8) char source[256];
    alignas(8) char resourceName[32];
    RwStreamRead(stream, source, 256);
    if (std::strlen(source))
    {
        void* accessor = reinterpret_cast<void*>(
            GlobalInt_ResolveAccessor(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        void* resources =
            (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, accessor, 0)[0x10 / 4]))(accessor, nullptr);

        (reinterpret_cast<void (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, resources, 0)[0x10 / 4]))(
                resources, nullptr, CSF_STR(0x00840DD4, "Gfx\\Textures\\"));

        if ((reinterpret_cast<bool (__fastcall*)(void*, void*, char*)>(
                CSF_AT(void**, resources, 0)[0x1C / 4]))(
                    resources, nullptr, source))
        {
            NormalizeResourceName(source, resourceName, 32);
            CSF_AT(uint32_t, self, 0x28) =
                GlobalInt_FindNameId(reinterpret_cast<uint8_t*>(resourceName));
        }
        else
        {
            CSF_AT(uint32_t, self, 0x28) = 0;
        }
    }

    SceneMissionSetup_SetBounds(
        reinterpret_cast<uint8_t*>(self) + 0x2C, nullptr,
        bound1, bound2, bound3, bound4, bound5, bound6);
    CSF_AT(float, self, 0x18) =
        CSF_AT(float, self, 0x14) / std::sqrt(g_ParticleRadiusDivisorSquared);
    S_006deed0_LoadTexture(self, nullptr, groupCount);

    uint32_t groupOffset = 0;
    for (int group = 0; group < groupCount; ++group, groupOffset += 16)
    {
        unsigned int recordCount = 0;
        RwStreamRead(stream, &recordCount, 4);
        S_006de5b0_ResizeRecords(
            reinterpret_cast<void*>(self->field_0004 + groupOffset),
            nullptr, recordCount);

        uint32_t recordOffset = 0;
        for (int record = 0; record < static_cast<int32_t>(recordCount);
             ++record, recordOffset += 28)
        {
            alignas(8) uint32_t recordData[7];
            std::memset(recordData, 0, 20);
            recordData[6] = static_cast<uint32_t>(-1);
            recordData[5] = static_cast<uint32_t>(-1082130432);
            RwStreamRead(stream, &recordData[0], 4);
            RwStreamRead(stream, &recordData[1], 4);
            RwStreamRead(stream, &recordData[2], 4);
            RwStreamRead(stream, &recordData[3], 4);
            RwStreamRead(stream, &recordData[4], 4);
            RwStreamRead(stream, &recordData[6], 4);

            std::memcpy(
                reinterpret_cast<void*>(
                    CSF_AT(uint32_t,
                        reinterpret_cast<void*>(self->field_0004 + groupOffset),
                        0x4) + recordOffset),
                recordData, 0x1C);
        }
    }

    RwStreamClose(stream, nullptr);

    uint32_t groups = self->field_0004;
    int loadedGroupCount = groups
        ? (static_cast<int32_t>(self->field_0008 - groups) >> 4)
        : 0;
    int groupsToProcess = loadedGroupCount - 1;

    groupOffset = 0;
    for (int group = 0; group < groupsToProcess; ++group, groupOffset += 16)
    {
        uint32_t records = CSF_AT(
            uint32_t, reinterpret_cast<void*>(groups + groupOffset), 0x4);
        unsigned int recordCount = records
            ? (CSF_AT(uint32_t,
                   reinterpret_cast<void*>(groups + groupOffset), 0x8)
               - records) / 28
            : 0;

        uint32_t recordOffset = 0;
        for (int record = 0; record < static_cast<int32_t>(recordCount);
             ++record, recordOffset += 28)
        {
            if (CSF_AT(float,
                    reinterpret_cast<void*>(
                        CSF_AT(uint32_t,
                            reinterpret_cast<void*>(groups + groupOffset), 0x4)
                        + recordOffset),
                    0x14) != g_MissingMetricValue)
                continue;

            int chainLength = 0;
            int link = record;
            uint32_t chainGroupOffset = groupOffset;
            if (recordOffset != static_cast<uint32_t>(-28))
            {
                do
                {
                    uint32_t chainRecords = CSF_AT(
                        uint32_t,
                        reinterpret_cast<void*>(groups + chainGroupOffset), 0x4);
                    link = CSF_AT(int,
                        reinterpret_cast<void*>(
                            chainRecords + 28 * static_cast<uint32_t>(link)),
                        0x18);
                    ++chainLength;
                    chainGroupOffset += 16;
                }
                while (link != -1);
            }

            float step = g_TransitionOne / static_cast<double>(chainLength);
            link = record;
            int position = 0;
            chainGroupOffset = groupOffset;
            if (recordOffset != static_cast<uint32_t>(-28))
            {
                do
                {
                    uint32_t chainRecords = CSF_AT(
                        uint32_t,
                        reinterpret_cast<void*>(groups + chainGroupOffset), 0x4);
                    uint32_t linkOffset = 28 * static_cast<uint32_t>(link);
                    CSF_AT(float,
                        reinterpret_cast<void*>(chainRecords + linkOffset),
                        0x14) = static_cast<double>(position) * step;
                    ++position;

                    groups = self->field_0004;
                    link = CSF_AT(int,
                        reinterpret_cast<void*>(
                            CSF_AT(uint32_t,
                                reinterpret_cast<void*>(groups + chainGroupOffset),
                                0x4) + linkOffset),
                        0x18);
                    chainGroupOffset += 16;
                }
                while (link != -1);
            }
        }
    }

    S_006deed0_ReorderTextureRecords(self, nullptr);
    return 1;
}

HOOK(0x006DF290, S_006deed0_LoadParticleFile, "auto");
