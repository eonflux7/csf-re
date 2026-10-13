#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(bool, __cdecl, sub_436BA0, 0x00436BA0, (void* a1));
CSF_FUNC(char, __fastcall, sub_436C90, 0x00436C90, (void* self, void* edx, float* a2));
CSF_FUNC(int*, __fastcall, sub_437390, 0x00437390, (void* self, void* edx, float* a2));
CSF_FUNC(void, __fastcall, sub_437520, 0x00437520, (void* self, void* edx, int* a2, float* a3));
CSF_FUNC(int, __fastcall, sub_437820, 0x00437820, (void* self, void* edx, int a2));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

static uint32_t __fastcall CGuiTacticalMap_UpdateObjectMarkers(
    void* self, void* /*edx*/)
{
    alignas(8) uint8_t testPositionBytes[0xC];
    alignas(8) uint8_t markerPositionBytes[0x10];
    float* testPosition = reinterpret_cast<float*>(testPositionBytes);
    float* markerPosition = reinterpret_cast<float*>(markerPositionBytes);

    // The count occupies the dword immediately after the second vector.
    uint32_t& objectCount = CSF_AT(uint32_t, markerPositionBytes, 0xC);
    objectCount = CSF_AT(uint32_t, g_pCurrentScene, 0x28);

    uint32_t result = objectCount;
    uint32_t index = 0;
    if (result != 0)
    {
        do
        {
            void* object = CSF_AT(void**, g_pCurrentScene, 0x20)[index];
            if (sub_436BA0(object))
            {
                float* position = GetObjectPositionVector(object, nullptr);
                testPosition[0] = position[0];
                testPosition[1] = position[1];
                testPosition[2] = position[2];
                markerPosition[0] = position[0];
                markerPosition[1] = position[1];
                markerPosition[2] = position[2];

                if (sub_436C90(self, nullptr, testPosition))
                {
                    sub_437390(self, nullptr, markerPosition);
                }
                else
                {
                    int* marker = reinterpret_cast<int*>(
                        sub_437820(self, nullptr, reinterpret_cast<int>(object)));
                    sub_437520(self, nullptr, marker, markerPosition);
                }
            }

            result = objectCount;
            ++index;
        } while (index < result);
    }
    return result;
}

HOOK(0x00437900, CGuiTacticalMap_UpdateObjectMarkers, "auto");
