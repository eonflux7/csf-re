#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, FxParticleManager_GrowRetiredParticleList, 0x0051FCD0,
         (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60,
         (void* destination, const void* source, std::size_t byteCount));

static int __fastcall FxParticleManager_ProcessParticleId(
    void* self, void* /*edx*/, int particleId)
{
    int result = particleId;
    if (particleId)
    {
        int index = CSF_AT(int, self, 0x64) - 1;
        if (index >= 0)
        {
            int offset = 36 * index;
            do
            {
                auto* record = CSF_AT(std::uint8_t*, self, 0x5C) + offset;
                if (CSF_AT(int, record, 0) == particleId)
                {
                    void* particle = CSF_AT(void*, record, 0x14);
                    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, particle, 0)[0x2C / 4])(
                            particle, nullptr, 0);

                    void** vtable = CSF_AT(void**, particle, 0);
                    int state = reinterpret_cast<int(__fastcall*)(void*, void*)>(
                        vtable[0x48 / 4])(particle, nullptr);
                    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                        vtable[0x38 / 4])(particle, nullptr, state);

                    record = CSF_AT(std::uint8_t*, self, 0x5C) + offset;
                    void* resource1 = CSF_AT(void*, record, 0x1C);
                    void* resource2 = CSF_AT(void*, record, 0x20);

                    if (CSF_AT(std::uint32_t, self, 0x54) >=
                        CSF_AT(std::uint32_t, self, 0x50))
                    {
                        FxParticleManager_GrowRetiredParticleList(
                            static_cast<std::uint8_t*>(self) + 0x4C,
                            nullptr, CSF_AT(int, self, 0x58));
                    }

                    std::uint32_t retiredCount =
                        CSF_AT(std::uint32_t, self, 0x54);
                    auto* retiredRecord =
                        CSF_AT(std::uint8_t*, self, 0x4C) + 16 * retiredCount;
                    CSF_AT(std::uint32_t, self, 0x54) = retiredCount + 1;
                    CSF_AT(void*, retiredRecord, 0) = particle;
                    CSF_AT(void*, retiredRecord, 4) = nullptr;
                    CSF_AT(void*, retiredRecord, 8) = resource1;
                    CSF_AT(void*, retiredRecord, 0xC) = resource2;

                    int remainingCount = CSF_AT(int, self, 0x64) - 1;
                    CSF_AT(int, self, 0x64) = remainingCount;
                    if (index != remainingCount)
                    {
                        auto* destination =
                            CSF_AT(std::uint8_t*, self, 0x5C) + offset;
                        _memcpy(destination, destination + 36,
                                36 * (remainingCount - index));
                    }
                }

                --index;
                offset -= 36;
                result = offset;
            } while (index >= 0);
        }
    }
    return result;
}

HOOK(0x005200D0, FxParticleManager_ProcessParticleId, "auto");
