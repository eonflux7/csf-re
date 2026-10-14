#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Bound by address: RwRasterGetPluginOffset(1036) returns a plugin-data base address.
CSF_FUNC(int, __cdecl, RwRasterGetPluginOffset, 0x00627430, (int a1));

// Plugin data slot id used by the original (decimal 1036 = 0x40C).
static constexpr int kWaterRasterPluginId = 1036;

// Offset inside the water object that holds the per-raster plugin displacement.
static constexpr uint32_t kRasterPluginDisplacementOffset = 0xF4;

// Vtable slot 0x30 / 4 = 12. The original does `mov ecx, [obj]` (the vtable), `lea edx, [buf]`, `push edx`,
// `push obj`, `call [ecx+0x30]`: the callee is stdcall (object and buffer on the stack, callee pops 8), and ecx still
// holds the vtable and edx the buffer at the call. A __fastcall view of the same call puts those two values in
// ecx/edx and pushes the same two stack arguments, so ecx/edx match the original's registers at the call.
using RasterPluginQueryFn = int(__fastcall*)(void* vtableInEcx, void* bufferInEdx, void* obj, void* outBuffer);
static constexpr int kRasterPluginQuerySlot = 0x30 / 4;

static bool __fastcall Water_RasterPluginRequiresFallback(void* self, void* /*edx*/)
{
    // Original calls the plugin lookup first, then reads the displacement field.
    int pluginBase = RwRasterGetPluginOffset(kWaterRasterPluginId);

    int displacement = CSF_AT(int, self, 0xF4);

    // Read the dword at (pluginBase + displacement); this value is the plugin object pointer.
    uint8_t* pluginBytes = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(pluginBase));
    int pluginObjectValue = *reinterpret_cast<int*>(pluginBytes + displacement);

    // Zeroed 8-dword output buffer (0x20 bytes) filled by the plugin query.
    int queryBuffer[8] = {};

    void* pluginObject = reinterpret_cast<void*>(static_cast<uintptr_t>(pluginObjectValue));
    void** vtable = CSF_AT(void**, pluginObject, 0);
    int queryResult = reinterpret_cast<RasterPluginQueryFn>(vtable[kRasterPluginQuerySlot])(vtable, queryBuffer, pluginObject, queryBuffer);

    if (queryResult != 0)
        return true;

    // Original tests [ebp-0x10], which is queryBuffer[4].
    return queryBuffer[4] == 0;
}

HOOK(0x00709930, Water_RasterPluginRequiresFallback, "auto");
