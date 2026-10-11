#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiCore_InitializeQuery, 0x0044E030,
         (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCore_InitializeFallbackQuery, 0x004B93B0,
         (void* self, void* edx));
CSF_FUNC(bool, __fastcall, AiCore_TestPrimaryQuery, 0x004E7DC0,
         (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(bool, __fastcall, AiCore_TestFallbackQuery, 0x004F14D0,
         (void* self, void* edx, int a2, uint32_t* a3));

static bool __fastcall S_004ad990_TestAvailableQuery(
    S_004ad990* self, void* /*edx*/)
{
    alignas(8) uint8_t queryBytes[0x34];
    C_0083124c& query = *reinterpret_cast<C_0083124c*>(queryBytes);
    bool result = false;

    if (self->field_002c == UINT32_MAX)
    {
        if (self->field_0030 == UINT32_MAX)
            return result;

        if (self->field_002c == UINT32_MAX)
        {
            if (self->field_0030 != UINT32_MAX)
            {
                if ((self->field_0054 & 0x80000000) == 0)
                {
                    void* primaryTester = CSF_AT(
                        void*, reinterpret_cast<void*>(self->field_0000), 0x3C);

                    AiCore_InitializeQuery(&query, nullptr);
                    result = AiCore_TestPrimaryQuery(
                        primaryTester, nullptr,
                        reinterpret_cast<int>(
                            reinterpret_cast<uint8_t*>(self) + 0x4C),
                        reinterpret_cast<uint32_t*>(&query));
                    if (result)
                        return result;
                }

                if ((self->field_003c & 0x80000000) == 0)
                {
                    void* fallbackTester = CSF_AT(
                        void*, reinterpret_cast<void*>(self->field_0000), 0x40);

                    AiCore_InitializeFallbackQuery(&query.field_000c, nullptr);
                    return AiCore_TestFallbackQuery(
                        fallbackTester, nullptr,
                        reinterpret_cast<int>(
                            reinterpret_cast<uint8_t*>(self) + 0x34),
                        reinterpret_cast<uint32_t*>(&query.field_000c));
                }
            }
            return result;
        }
    }

    void* primaryTester = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0000), 0x3C);
    const uint32_t flags = self->field_0054;

    query.vtbl = CSF_VTBL(0x0083124C);
    CSF_AT(void**, &query, 0x14) = CSF_VTBL(0x0082D448);
    query.field_0018 = UINT32_MAX;
    query.field_001c = UINT32_MAX;
    std::memset(&query.field_0020, 0, 12);
    std::memset(&query.field_0004, 255, 16);
    query.field_002c = 2139095039;
    query.field_0030 = 0;

    if ((flags & 0x80000000) != 0)
    {
        return AiCore_TestPrimaryQuery(
            primaryTester, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x34),
            reinterpret_cast<uint32_t*>(&query));
    }

    return AiCore_TestPrimaryQuery(
        primaryTester, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x4C),
        reinterpret_cast<uint32_t*>(&query));
}

HOOK(0x004BD800, S_004ad990_TestAvailableQuery, "auto");
