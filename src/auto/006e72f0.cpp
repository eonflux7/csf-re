#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __cdecl, GfxMatrix_ClosestPointOnOrientedBox, 0x006AD550,
         (float* result, int point, float* box));
CSF_FUNC(int, __fastcall, CollisionHits_Insert, 0x006E6E00,
         (void* self, void* edx, int a2, void* a3));
CSF_FUNC(char, __fastcall, CollisionHits_Grow, 0x006E6F40,
         (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static char __fastcall C_00861d48_CollectCollisionHits(
    C_00861d48* self, void* /*edx*/, int query, int outputAddress, float radius)
{
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        self->vtbl[0x58 / 4])(self, nullptr);

    alignas(8) uint8_t limitBytes[0x4];
    CSF_AT(int, limitBytes, 0) = 2139095039;

    void* bounds = reinterpret_cast<void*>(self->field_0024);
    if (!reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*, float)>(
            CSF_AT(void**, bounds, 0)[0x10 / 4])(
            bounds, nullptr, query, limitBytes, radius))
        return 0;

    void* output = reinterpret_cast<void*>(outputAddress);
    CSF_AT(uint8_t, output, 0x10) = 1;

    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        self->vtbl[0x5C / 4])(self, nullptr);

    uint32_t remaining = self->field_0030;
    uint32_t childOffset = 0;
    uint32_t comparisonOffset = 0;
    char found = 0;

    alignas(8) uint8_t boxBytes[0x3C];
    alignas(8) uint8_t closestPointBytes[0xC];
    alignas(8) uint8_t recordBytes[0x2C];
    alignas(8) uint8_t hitBytes[0x2C];

    while (remaining != 0)
    {
        CSF_AT(uint32_t, hitBytes, 0x00) = 0;
        CSF_AT(uint32_t, hitBytes, 0x04) = 0;
        CSF_AT(uint32_t, hitBytes, 0x08) = 0;
        CSF_AT(uint32_t, hitBytes, 0x0C) = 0;
        CSF_AT(uint32_t, hitBytes, 0x10) = 0;
        CSF_AT(uint32_t, hitBytes, 0x14) = 0;
        // The original leaves this output uninitialized before the query.
        CSF_AT(float, hitBytes, 0x18) = 0;

        void* child = CSF_AT(
            void*, reinterpret_cast<uint8_t*>(self->field_0028) + childOffset, 0);

        if (reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*, float)>(
                CSF_AT(void**, child, 0)[0x14 / 4])(
                child, nullptr, query, hitBytes, radius))
        {
            bool insert = false;
            uint32_t insertionIndex = 0xFFFFFFFF;
            uint32_t index = 0;

            if (CSF_AT(uint32_t, output, 0x08) != 0)
            {
                do
                {
                    if (insert)
                        break;

                    uint8_t* records = CSF_AT(uint8_t*, output, 0);
                    if (CSF_AT(float, hitBytes, 0x18) <
                        CSF_AT(float, records + comparisonOffset, 0x24))
                    {
                        insert = true;
                        insertionIndex = index;
                    }
                    ++index;
                }
                while (index < CSF_AT(uint32_t, output, 0x08));
            }

            if (radius != g_fZero)
            {
                child = CSF_AT(
                    void*, reinterpret_cast<uint8_t*>(self->field_0028) + childOffset, 0);
                reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, child, 0)[0x1C / 4])(
                    child, nullptr, boxBytes);

                float* closest = GfxMatrix_ClosestPointOnOrientedBox(
                    reinterpret_cast<float*>(closestPointBytes),
                    reinterpret_cast<int>(hitBytes),
                    reinterpret_cast<float*>(boxBytes));
                std::memcpy(hitBytes, closest, 0xC);
            }

            CSF_AT(int, recordBytes, 0x1C) = -1;
            CSF_AT(int, recordBytes, 0x20) = -1;
            CSF_AT(float, recordBytes, 0x24) = CSF_AT(float, hitBytes, 0x18);
            std::memcpy(recordBytes, hitBytes, 0xC);
            CSF_AT(int, recordBytes, 0x18) = 0;
            std::memcpy(recordBytes + 0xC, hitBytes + 0xC, 0xC);

            if (insert)
            {
                CollisionHits_Insert(
                    output, nullptr, static_cast<int>(insertionIndex), recordBytes);
            }
            else
            {
                if (CSF_AT(uint32_t, output, 0x08) >=
                    CSF_AT(uint32_t, output, 0x04))
                {
                    CollisionHits_Grow(output, nullptr, CSF_AT(int, output, 0x0C));
                }

                uint32_t count = CSF_AT(uint32_t, output, 0x08);
                uint8_t* records = CSF_AT(uint8_t*, output, 0);
                CSF_AT(uint32_t, output, 0x08) = count + 1;
                std::memcpy(records + 40 * count, recordBytes, 0x28);
            }
            found = 1;
        }

        comparisonOffset += 40;
        childOffset += 16;
        --remaining;
    }

    return found;
}

HOOK(0x006E72F0, C_00861d48_CollectCollisionHits, "auto");
