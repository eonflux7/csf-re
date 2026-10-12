#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GameConfig_IsStateCodeAllowed, 0x00755810, (std::uint32_t stateCode));

static int __fastcall C_00830d68_ForwardScaledPosition(C_00830d68* self, void* /*edx*/, void* a2, int* a3)
{
    int result = GetGlobalState_50D190();
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0xD0) == 0)
    {
        if (a2 != nullptr && a3 != nullptr)
        {
            if (CSF_AT(int, a2, 0x78) == 1)
            {
                result = GameConfig_IsStateCodeAllowed(CSF_AT(std::uint32_t, a2, 0x50));
                if (static_cast<std::uint8_t>(result) != 0)
                {
                    int mode = CSF_AT(int, a2, 0x1D8);
                    if (mode == 1 || mode == 2)
                    {
                        // Virtual slot 0x6C / 4 = 27 on a3: returns pointer to {x, y, z, w(float)}
                        void** vtbl = CSF_AT(void**, a3, 0);
                        auto getVector = reinterpret_cast<const std::uint8_t* (__fastcall*)(void*, void*)>(vtbl[27]);
                        const std::uint8_t* src = getVector(a3, nullptr);

                        std::uint32_t vec[4];
                        std::memcpy(&vec[0], src, 12);

                        float srcW = 0.0f;
                        std::memcpy(&srcW, src + 12, 4);

                        float scale = 0.0f;
                        std::memcpy(&scale, &self->field_0040, 4);

                        float scaled = srcW * scale;
                        std::memcpy(&vec[3], &scaled, 4);

                        // Virtual slot 0x68 / 4 = 26 on a3 with the scaled vector
                        auto forward = reinterpret_cast<int(__fastcall*)(void*, void*, std::uint32_t*)>(
                            CSF_AT(void**, a3, 0)[26]);
                        return forward(a3, nullptr, vec);
                    }
                }
            }
        }
    }
    return result;
}

HOOK(0x0043A7D0, C_00830d68_ForwardScaledPosition, "auto");
