#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpClumpClone, 0x0058FC60, (int a1));
CSF_FUNC(int, __cdecl, RwFrameTransform, 0x00623860, (int a1, int a2, int a3));
CSF_FUNC(int, __fastcall, GetField04Bit1, 0x006B8160, (void* self, void*));
CSF_FUNC(int, __fastcall, GetReferencedField14, 0x006B8270, (void* self, void*));
CSF_FUNC(int16_t, __fastcall, SetWordValue, 0x006B8370, (uint16_t* self, void* , int16_t value));
CSF_FUNC(int, __fastcall, GfxObjectProps_SetSecondaryRecordValue, 0x006B8380, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(void, __fastcall, GfxObjectProps_SetPlanoAlpha, 0x006B9840, (S_006b8020* self, void* , float alpha));
CSF_FUNC(void, __fastcall, GfxObjectProps_SetFloatPropertyAt20, 0x006B9890, (S_006b8020* self, void* , float value));
CSF_FUNC(void, __fastcall, GfxObjectProps_SetPropertyFloat24, 0x006B98E0, (S_006b8020* self, void* , float value));
CSF_FUNC(C_0082d410*, __fastcall, GfxObjectProps_AssignPropertyRecord, 0x006BA560, (S_006b8020* self, void* , C_0082d410* sourceRecord));
CSF_FUNC(int, __fastcall, C_008605d8_GetClumpForRecordIndex, 0x006BD260, (void* self, void* edx, int a2));
CSF_FUNC(char, __cdecl, GfxEnvironment_SetSectorObjectFlag4, 0x006C0160, (int a1, char a2));
CSF_FUNC(int, __cdecl, SceneLoader_CollectSectorObjects, 0x006C0B90, (int clumpHandle, int collectorHandle));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_00860288_AddEntryAndRegister, 0x006C6D40, (C_00860288* self, void* , int a1));
CSF_FUNC(int, __fastcall, GfxEnvironment_SetAttachmentRecordValue, 0x007283A0, (void* self, void* edx, int a2));

CSF_GLOBAL(int*, g_SectorObjects, 0x008A7E50);
CSF_GLOBAL(std::uint32_t, g_SectorObjectCount, 0x008A7E58);

static int __fastcall C_008605d8_BuildClumpFromRecord(
    C_008605d8* self, void* /*edx*/, int recordAddress)
{
    auto* record = reinterpret_cast<std::uint8_t*>(recordAddress);
    const int sourceClump = C_008605d8_GetClumpForRecordIndex(
        self, nullptr, static_cast<int>(CSF_AT(std::uint32_t, record, 0x00) - 1000));
    if (!sourceClump)
        return sourceClump;

#ifdef CSF_STANDALONE
    // Diagnostics (CSF_RW_TRACE): the game atomic plugin words (+0, +4 = property record) of the source clump's
    // atomics, before the clone; the clone's follow below.
    static int traced;
    const bool trace = std::getenv("CSF_RW_TRACE") && traced < 4;
    if (trace) {
        ++traced;
        SceneLoader_CollectSectorObjects(sourceClump, reinterpret_cast<int>(&g_SectorObjects));
        const uint32_t off = *reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB478));
        for (std::uint32_t i = 0; i < g_SectorObjectCount && i < 4; ++i)
            std::fprintf(stderr, "csf: CLONE source record=%d atomic=%08X plugin+%X: %08X %08X\n", CSF_AT(int, record, 0),
                         g_SectorObjects[i], off, CSF_AT(uint32_t, reinterpret_cast<void*>(g_SectorObjects[i] + off), 0),
                         CSF_AT(uint32_t, reinterpret_cast<void*>(g_SectorObjects[i] + off), 4));
    }
#endif
    const int clump = RpClumpClone(sourceClump);
    SceneLoader_CollectSectorObjects(clump, reinterpret_cast<int>(&g_SectorObjects));
#ifdef CSF_STANDALONE
    if (trace) {
        const uint32_t off = *reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB478));
        std::fprintf(stderr, "csf: CLONE source=%08X clone=%08X atomics=%u\n", sourceClump, clump, g_SectorObjectCount);
        for (std::uint32_t i = 0; i < g_SectorObjectCount && i < 4; ++i)
            std::fprintf(stderr, "csf: CLONE copy atomic=%08X plugin+%X: %08X %08X\n", g_SectorObjects[i], off,
                         CSF_AT(uint32_t, reinterpret_cast<void*>(g_SectorObjects[i] + off), 0),
                         CSF_AT(uint32_t, reinterpret_cast<void*>(g_SectorObjects[i] + off), 4));
    }
#endif

    for (std::uint32_t index = 0; index < g_SectorObjectCount; ++index)
    {
        auto* attachment = reinterpret_cast<S_006b8020*>(
            GfxObjectProps_GetObjectAttachment(g_SectorObjects[index]));

        SetWordValue(
            reinterpret_cast<std::uint16_t*>(attachment), nullptr,
            static_cast<std::int16_t>(CSF_AT(int, record, 0x00)));
        GfxObjectProps_SetSecondaryRecordValue(
            attachment, nullptr, CSF_AT(int, record, 0x04));
        GfxObjectProps_SetPlanoAlpha(
            attachment, nullptr, CSF_AT(float, record, 0x28));
        GfxObjectProps_SetFloatPropertyAt20(
            attachment, nullptr, CSF_AT(float, record, 0x2C));
        GfxObjectProps_SetPropertyFloat24(
            attachment, nullptr, CSF_AT(float, record, 0x30));
        GfxObjectProps_AssignPropertyRecord(
            attachment, nullptr, reinterpret_cast<C_0082d410*>(record + 0x08));
        GfxEnvironment_SetAttachmentRecordValue(
            attachment, nullptr, CSF_AT(int, record, 0x34));

        const int sectorObject = g_SectorObjects[index];
        void* currentAttachment = reinterpret_cast<void*>(
            GfxObjectProps_GetObjectAttachment(sectorObject));

        if (!GetReferencedField14(currentAttachment, nullptr) &&
            !static_cast<std::uint8_t>(GetField04Bit1(currentAttachment, nullptr)))
        {
            auto* registry = reinterpret_cast<C_00860288*>(GetGlobalValue_6C5760());
            C_00860288_AddEntryAndRegister(registry, nullptr, sectorObject);
        }

        GfxEnvironment_SetSectorObjectFlag4(sectorObject, 0);
    }

    const int transform = CSF_AT(int, record, 0x24);
    const int frame = CSF_AT(int, reinterpret_cast<void*>(clump), 0x04);
    RwFrameTransform(frame, transform, 0);
    return clump;
}

HOOK(0x006BD690, C_008605d8_BuildClumpFromRecord, "auto");
