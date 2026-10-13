#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiTacticalMap_FindPositionSprite, 0x00437230,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, CGuiTacticalMap_FindObjectSprite, 0x004372B0,
         (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, CGuiTacticalMap_PositionSprite, 0x00437520,
         (void* self, void* edx, int* a2, float* a3));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30,
         (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430,
         (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall CGuiTacticalMap_ResolveOrPositionSprite(
    CGuiTacticalMap* self, void* /*edx*/, int recordAddress, char queryOnly,
    uint32_t* outPosition, uint32_t* outPair, int** outSprite)
{
    auto* record = reinterpret_cast<uint8_t*>(recordAddress);
    int* sprite = nullptr;
    alignas(8) uint8_t positionBytes[0xC] = {};
    alignas(8) uint8_t rectangleBytes[0x10];

    const uint32_t kind = CSF_AT(uint32_t, record, 0);
    if (kind == 1)
    {
        sprite = reinterpret_cast<int*>(
            CGuiTacticalMap_FindPositionSprite(self, nullptr, recordAddress, 17));
        std::memcpy(positionBytes, record + 4, 0xC);
    }
    else if (kind == 2)
    {
        sprite = reinterpret_cast<int*>(
            CGuiTacticalMap_FindObjectSprite(self, nullptr, recordAddress));

        if (CSF_AT(uint8_t, record, 0x28))
        {
            std::memcpy(positionBytes, record + 0x2C, 0xC);
        }
        else
        {
            auto* object = reinterpret_cast<C_008342e8*>(
                CHandleTable_Resolve(
                    g_pCurrentScene, nullptr,
                    reinterpret_cast<int>(record + 0x10)));

            if (object)
            {
                const bool missingRequiredFlags =
                    reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, object, 0)[0x7C / 4])(object, nullptr);

                if (!missingRequiredFlags)
                {
                    float* position = GetObjectPositionVector(object, nullptr);
                    std::memcpy(positionBytes, position, 0xC);
                }
            }
        }
    }

    if (queryOnly)
    {
        outPosition[0] = CSF_AT(uint32_t, positionBytes, 0);
        outPosition[1] = CSF_AT(uint32_t, positionBytes, 4);
        outPosition[2] = CSF_AT(uint32_t, positionBytes, 8);
        outPair[0] = 0;
        outPair[1] = 0;
        *outSprite = sprite;
    }
    else if (sprite)
    {
        uint32_t* rectangle =
            reinterpret_cast<uint32_t* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, sprite, 0)[0x6C / 4])(sprite, nullptr);

        std::memcpy(rectangleBytes, rectangle, 0xC);
        CSF_AT(uint32_t, rectangleBytes, 0xC) = 1132396544;

        reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, sprite, 0)[0x68 / 4])(
                sprite, nullptr, reinterpret_cast<uint32_t*>(rectangleBytes));

        CGuiTacticalMap_PositionSprite(
            self, nullptr, sprite, reinterpret_cast<float*>(positionBytes));
    }
}

HOOK(0x004375D0, CGuiTacticalMap_ResolveOrPositionSprite, "auto");
