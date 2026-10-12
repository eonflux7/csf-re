#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MpRules_ConstructPromotionEntries, 0x005585B0, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(unsigned int, __fastcall, MpRules_InsertPromotionEntries, 0x00558890, (void* self, void* edx, int a1, unsigned int a3, uint32_t* arg8));
CSF_FUNC(unsigned int, __fastcall, MpRules_AppendPromotionEntry, 0x00558D30, (void* self, void* edx, uint32_t* a3));
CSF_FUNC(int*, __fastcall, MpRules_AppendRolePromotions, 0x005596D0, (void* self, void* edx, int* a3));
CSF_FUNC(uint32_t*, __fastcall, MpRules_StoreTeamPromotions, 0x00559E00, (void* self, void* edx, int* a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));

// The packet also classifies the child-count call address as a vtable reference.
[[maybe_unused]] static const auto kPropertyNodeVtable = CSF_VTBL(0x00692B80);

static void MpRules_LoadRolePromotions(
    std::uint8_t* node,
    int role,
    int* roleRecord,
    S_005594b0* teamPromotions,
    std::uint32_t* entry,
    bool useAppendMethod)
{
    auto& promotions = *reinterpret_cast<S_005587c0*>(
        reinterpret_cast<std::uint8_t*>(roleRecord) + 4);

    std::memset(&promotions.field_0004, 0, 12);
    *roleRecord = role;

    std::uint32_t begin = 0;
    std::uint32_t end = 0;
    unsigned int index = 0;

    if (MenuExtras_GetChildCount(node, nullptr))
    {
        do
        {
            auto* component = reinterpret_cast<std::uint8_t*>(
                GetComponent(node, nullptr, index));

            if (component)
            {
                CSF_AT(int, entry, 0) = ReadIntegerProperty(
                    component, nullptr,
                    const_cast<char*>(CSF_STR(0x00844968, ".PM_FRAGS")));
                CSF_AT(int, entry, 4) = ReadIntegerProperty(
                    component, nullptr,
                    const_cast<char*>(CSF_STR(0x0084495C, ".PM_ITEM")));

                if (useAppendMethod)
                {
                    MpRules_AppendPromotionEntry(&promotions, nullptr, entry);
                }
                else if (
                    begin &&
                    static_cast<std::uint32_t>(
                        static_cast<std::int32_t>(end - begin) >> 3) <
                    static_cast<std::uint32_t>(
                        static_cast<std::int32_t>(
                            promotions.field_000c - begin) >> 3))
                {
                    MpRules_ConstructPromotionEntries(
                        static_cast<int>(end), 1,
                        reinterpret_cast<int>(entry),
                        reinterpret_cast<int>(&promotions),
                        static_cast<int>(index));
                    end += 8;
                    promotions.field_0008 = end;
                }
                else
                {
                    MpRules_InsertPromotionEntries(
                        &promotions, nullptr, static_cast<int>(end), 1u, entry);
                    end = promotions.field_0008;
                    begin = promotions.field_0004;
                }
            }

            ++index;
        }
        while (index < MenuExtras_GetChildCount(node, nullptr));
    }

    MpRules_AppendRolePromotions(teamPromotions, nullptr, roleRecord);

    if (useAppendMethod)
    {
        if (promotions.field_0004)
            Mem_FreeDebug(reinterpret_cast<void*>(promotions.field_0004));
    }
    else if (begin)
    {
        Mem_FreeDebug(reinterpret_cast<void*>(begin));
    }
}

static void MpRules_FreeTeamPromotions(
    S_005594b0* promotions, bool reloadEnd, std::uint32_t cachedEnd)
{
    if (!promotions->field_0004)
        return;

    if (promotions->field_0004 !=
        (reloadEnd ? promotions->field_0008 : cachedEnd))
    {
        std::uint32_t nestedAddress = promotions->field_0004 + 8;
        do
        {
            auto* nested = reinterpret_cast<std::uint8_t*>(nestedAddress);
            if (CSF_AT(std::uint32_t, nested, 0))
                Mem_FreeDebug(CSF_AT(void*, nested, 0));

            if (reloadEnd)
                cachedEnd = promotions->field_0008;

            CSF_AT(std::uint32_t, nested, 0) = 0;
            CSF_AT(std::uint32_t, nested, 4) = 0;
            CSF_AT(std::uint32_t, nested, 8) = 0;
            nestedAddress += 20;
        }
        while (nestedAddress - 8 != cachedEnd);
    }

    Mem_FreeDebug(reinterpret_cast<void*>(promotions->field_0004));
}

static char __fastcall C_00844928_LoadPromotionRanks(
    C_00844928* self, void* /*edx*/, S_00692b80* configuration)
{
    auto* ranks = FindProperty(
        reinterpret_cast<std::uint8_t*>(configuration), nullptr,
        const_cast<char*>(CSF_STR(0x0084498C, ".PROMOTION_RANKS")));
    if (!ranks)
        return 0;

    // Preserve the contiguous ID/vector records passed to the game containers.
    alignas(8) std::uint8_t frame[0x50]{};
    auto* roleRecord = reinterpret_cast<int*>(frame);
    auto* alliedRecord = reinterpret_cast<int*>(frame + 0x14);
    auto& alliedPromotions =
        *reinterpret_cast<S_005594b0*>(frame + 0x18);
    auto* germanRecord = reinterpret_cast<int*>(frame + 0x28);
    auto& germanPromotions =
        *reinterpret_cast<S_005594b0*>(frame + 0x2C);
    auto* entry = reinterpret_cast<std::uint32_t*>(frame + 0x3C);

    auto* allies = FindProperty(
        ranks, nullptr,
        const_cast<char*>(CSF_STR(0x00844980, ".ALIADOS")));
    std::uint32_t alliedEnd = 0;
    std::memset(&alliedPromotions.field_0004, 0, 12);

    if (allies)
    {
        auto* commando = FindProperty(
            allies, nullptr,
            const_cast<char*>(CSF_STR(0x00844974, ".COMANDO")));
        *alliedRecord = 1;
        if (commando)
        {
            MpRules_LoadRolePromotions(
                commando, 1, roleRecord, &alliedPromotions, entry, true);
        }

        auto* spy = FindProperty(
            allies, nullptr,
            const_cast<char*>(CSF_STR(0x00844954, ".ESPIA")));
        if (spy)
        {
            MpRules_LoadRolePromotions(
                spy, 2, roleRecord, &alliedPromotions, entry, true);
        }

        auto* sniper = FindProperty(
            allies, nullptr,
            const_cast<char*>(CSF_STR(0x00844944, ".FRANCOTIRADOR")));
        if (sniper)
        {
            MpRules_LoadRolePromotions(
                sniper, 3, germanRecord, &alliedPromotions, entry, false);
        }

        MpRules_StoreTeamPromotions(
            reinterpret_cast<std::uint8_t*>(self) + 0x1D8,
            nullptr, alliedRecord);
        alliedEnd = alliedPromotions.field_0008;
    }

    auto* germans = FindProperty(
        ranks, nullptr,
        const_cast<char*>(CSF_STR(0x00844938, ".ALEMANES")));
    std::memset(&germanPromotions.field_0004, 0, 12);

    if (germans)
    {
        auto* commando = FindProperty(
            germans, nullptr,
            const_cast<char*>(CSF_STR(0x00844974, ".COMANDO")));
        *germanRecord = 2;
        if (commando)
        {
            MpRules_LoadRolePromotions(
                commando, 1, roleRecord, &germanPromotions, entry, false);
        }

        auto* spy = FindProperty(
            germans, nullptr,
            const_cast<char*>(CSF_STR(0x00844954, ".ESPIA")));
        if (spy)
        {
            MpRules_LoadRolePromotions(
                spy, 2, roleRecord, &germanPromotions, entry, false);
        }

        auto* sniper = FindProperty(
            germans, nullptr,
            const_cast<char*>(CSF_STR(0x00844944, ".FRANCOTIRADOR")));
        if (sniper)
        {
            MpRules_LoadRolePromotions(
                sniper, 3, roleRecord, &germanPromotions, entry, false);
        }

        MpRules_StoreTeamPromotions(
            reinterpret_cast<std::uint8_t*>(self) + 0x1D8,
            nullptr, germanRecord);
        alliedEnd = alliedPromotions.field_0008;
    }

    MpRules_FreeTeamPromotions(&germanPromotions, true, 0);
    MpRules_FreeTeamPromotions(&alliedPromotions, false, alliedEnd);
    return 1;
}

HOOK(0x0055A1B0, C_00844928_LoadPromotionRanks, "auto");
