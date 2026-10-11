#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(char, __fastcall, SetByteAtOffset20, 0x00506D70, (uint8_t* self, void* , char a2));
CSF_FUNC(void*, __fastcall, Registry_Initialize, 0x0069D070, (void* self, void* edx, char* a2, void* a3));
CSF_FUNC(bool, __fastcall, Registry_ReadValue, 0x0069D1B0, (S_0069d0f0* self, void* edx, const char* valueName, uint8_t* data));
CSF_FUNC(char, __fastcall, Registry_Destroy, 0x0069CFA0, (void* self, void* edx));

// The original passes a value name followed by a 0x10C-byte record
// on the stack. Explicit DWORD arguments preserve its ret 272 ABI.
CSF_FUNC(char, __fastcall, Registry_WriteValue, 0x0069D290, (
    void* self, void* edx, const char* valueName,
    uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3,
    uint32_t d4, uint32_t d5, uint32_t d6, uint32_t d7,
    uint32_t d8, uint32_t d9, uint32_t d10, uint32_t d11,
    uint32_t d12, uint32_t d13, uint32_t d14, uint32_t d15,
    uint32_t d16, uint32_t d17, uint32_t d18, uint32_t d19,
    uint32_t d20, uint32_t d21, uint32_t d22, uint32_t d23,
    uint32_t d24, uint32_t d25, uint32_t d26, uint32_t d27,
    uint32_t d28, uint32_t d29, uint32_t d30, uint32_t d31,
    uint32_t d32, uint32_t d33, uint32_t d34, uint32_t d35,
    uint32_t d36, uint32_t d37, uint32_t d38, uint32_t d39,
    uint32_t d40, uint32_t d41, uint32_t d42, uint32_t d43,
    uint32_t d44, uint32_t d45, uint32_t d46, uint32_t d47,
    uint32_t d48, uint32_t d49, uint32_t d50, uint32_t d51,
    uint32_t d52, uint32_t d53, uint32_t d54, uint32_t d55,
    uint32_t d56, uint32_t d57, uint32_t d58, uint32_t d59,
    uint32_t d60, uint32_t d61, uint32_t d62, uint32_t d63,
    uint32_t d64, uint32_t d65, uint32_t d66
));

CSF_GLOBAL(uint8_t, g_RegistryRootSelector, 0x0082E65C);

static char __cdecl AppMain_ConsumeScanFlag()
{
    alignas(8) uint8_t registryBytes[0x40];
    alignas(8) uint8_t dataBytes[0x10C] = {};
    auto* registry = reinterpret_cast<S_0069d0f0*>(registryBytes);

    auto* settings = reinterpret_cast<uint8_t*>(GetGlobalValue_75FDF0());
    const uint32_t pathCapacity = CSF_AT(uint32_t, settings, 0xEC);
    auto* pathStorage = settings + 0xD4;
    char* path;
    if (pathCapacity < 0x10)
        path = reinterpret_cast<char*>(pathStorage + 4);
    else
        path = CSF_AT(char*, pathStorage, 4);

    Registry_Initialize(registry, nullptr, path, &g_RegistryRootSelector);

    CSF_AT(uint32_t, dataBytes, 0x104) = 4;
    CSF_AT(uint32_t, dataBytes, 0x108) = 4;
    if (Registry_ReadValue(registry, nullptr, CSF_STR(0x0083F8EC, "Scan"), dataBytes)
        && CSF_AT(uint32_t, dataBytes, 0))
    {
        auto* application = reinterpret_cast<uint8_t*>(GetGlobalValue_8B056C());
        SetByteAtOffset20(application, nullptr, 1);
    }

    CSF_AT(uint32_t, dataBytes, 0) = 0;
    uint32_t writeData[0x10C / sizeof(uint32_t)];
    std::memcpy(writeData, dataBytes, 0x10C);
    Registry_WriteValue(
        registry, nullptr, CSF_STR(0x0083F8EC, "Scan"),
        writeData[0], writeData[1], writeData[2], writeData[3],
        writeData[4], writeData[5], writeData[6], writeData[7],
        writeData[8], writeData[9], writeData[10], writeData[11],
        writeData[12], writeData[13], writeData[14], writeData[15],
        writeData[16], writeData[17], writeData[18], writeData[19],
        writeData[20], writeData[21], writeData[22], writeData[23],
        writeData[24], writeData[25], writeData[26], writeData[27],
        writeData[28], writeData[29], writeData[30], writeData[31],
        writeData[32], writeData[33], writeData[34], writeData[35],
        writeData[36], writeData[37], writeData[38], writeData[39],
        writeData[40], writeData[41], writeData[42], writeData[43],
        writeData[44], writeData[45], writeData[46], writeData[47],
        writeData[48], writeData[49], writeData[50], writeData[51],
        writeData[52], writeData[53], writeData[54], writeData[55],
        writeData[56], writeData[57], writeData[58], writeData[59],
        writeData[60], writeData[61], writeData[62], writeData[63],
        writeData[64], writeData[65], writeData[66]);
    return Registry_Destroy(registry, nullptr);
}

HOOK(0x005091A0, AppMain_ConsumeScanFlag, "auto");
