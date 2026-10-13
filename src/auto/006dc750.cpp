#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_GlobalMemoryStatusImport, 0x0082D240);

CSF_FUNC(int, __cdecl, RwEngineGetTextureMemorySize, 0x00626C90, ());
CSF_FUNC(uint32_t, __cdecl, GetDisplayCapabilityBits, 0x006DC670, ());

static bool __fastcall S_006dc750_QueryMemoryAndTextureInfo(
    S_006dc750* self, void* /*edx*/, uint32_t* output)
{
    void* device = reinterpret_cast<void*>(self->field_005c);
    if (!device)
        return false;

    alignas(8) uint8_t deviceInfo[0x130];
    alignas(8) uint8_t memoryStatus[0x20];

    void** deviceVtable = CSF_AT(void**, device, 0);
    using DeviceQuery = int(__fastcall*)(void**, void*, void*, uint8_t*);
    if (reinterpret_cast<DeviceQuery>(deviceVtable[0x1C / 4])(
            deviceVtable, deviceInfo, device, deviceInfo) < 0)
        return false;

    // Retain the register context while passing the import's one stack argument.
    using MemoryStatusQuery = void(__fastcall*)(void**, void*, uint8_t*);
    reinterpret_cast<MemoryStatusQuery>(g_GlobalMemoryStatusImport)(
        deviceVtable, deviceInfo, memoryStatus);

    const uint32_t physicalMemoryMiB =
        ((CSF_AT(uint32_t, memoryStatus, 0x08) >> 20) + 1) & 0xFFFFFFFE;
    output[1] = physicalMemoryMiB;

    // These adapters add register arguments only; both callees still receive
    // zero stack arguments, matching their original cdecl bindings.
    using TextureMemoryQuery = int(__fastcall*)(uint32_t, void*);
    const int textureMemoryBytes =
        reinterpret_cast<TextureMemoryQuery>(RwEngineGetTextureMemorySize)(
            physicalMemoryMiB, deviceInfo);

    const uint32_t deviceInfoValueA = CSF_AT(uint32_t, deviceInfo, 0xC4);
    output[2] = textureMemoryBytes / 0x100000;
    const uint32_t deviceInfoValueB = CSF_AT(uint32_t, deviceInfo, 0xCC);
    output[3] = deviceInfoValueA;
    output[4] = deviceInfoValueB;

    using CapabilityQuery = uint32_t(__fastcall*)(S_006dc750*, uint32_t);
    output[0] = reinterpret_cast<CapabilityQuery>(GetDisplayCapabilityBits)(
        self, deviceInfoValueA);
    return true;
}

HOOK(0x006DC750, S_006dc750_QueryMemoryAndTextureInfo, "auto");
