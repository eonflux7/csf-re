#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_GLOBAL(const char*, g_psz_Gfx_Textures, 0x00899D78);

static constexpr uint32_t kTableSlotStride = 80;
static constexpr uint32_t kSlotTagOffset = 0x48;
static constexpr uint32_t kSlotDataOffset = 0x34;
static constexpr uint32_t kSlotTagThreshold = 0x10;

// Vtable slot 4 (+0x10) on the accessor and on the object it returns.
using VirtualGetObjectFn = void*(__fastcall*)(void* self, void* edx);
// Vtable slot 4 (+0x10) on the returned object: takes a const char*.
using VirtualSetTextureFn = void(__fastcall*)(void* self, void* edx, const char* text);
// Vtable slot 7 (+0x1C): takes one 32-bit value, returns eax.
using VirtualSetSlotValueFn = uint32_t(__fastcall*)(void* self, void* edx, uint32_t value);

static uint32_t __fastcall SceneTable_ClearSlotState(void* self, void* /*edx*/, uint32_t slotIndex)
{
    uint32_t tableAddr = CSF_AT(uint32_t, self, 0x04);
    uint32_t result = tableAddr;

    uint32_t beginAddr = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(tableAddr), 0x04);
    if (beginAddr != 0)
    {
        uint32_t endAddr = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(tableAddr), 0x08);
        int32_t byteSpan = static_cast<int32_t>(endAddr) - static_cast<int32_t>(beginAddr);
        uint32_t slotCount = static_cast<uint32_t>(byteSpan / static_cast<int32_t>(kTableSlotStride));
        result = slotCount;

        if (slotIndex < slotCount)
        {
            void* globalInt = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt()));
            void* accessor = reinterpret_cast<void*>(
                static_cast<uintptr_t>(GlobalInt_ResolveAccessor(globalInt, nullptr)));

            void* textureObj = reinterpret_cast<VirtualGetObjectFn>(CSF_AT(void**, accessor, 0)[4])(accessor, nullptr);

            reinterpret_cast<VirtualSetTextureFn>(CSF_AT(void**, textureObj, 0)[4])(
                textureObj, nullptr, g_psz_Gfx_Textures);

            // The table pointer is reloaded here: the calls above may have changed it.
            uint32_t currentTableAddr = CSF_AT(uint32_t, self, 0x04);
            uint32_t slotBaseAddr = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(currentTableAddr), 0x04)
                                    + kTableSlotStride * slotIndex;
            uint8_t* slot = reinterpret_cast<uint8_t*>(slotBaseAddr);

            uint32_t slotTag = CSF_AT(uint32_t, slot, kSlotTagOffset);
            uint32_t slotValue = (slotTag < kSlotTagThreshold)
                                     ? slotBaseAddr + kSlotDataOffset
                                     : CSF_AT(uint32_t, slot, kSlotDataOffset);

            return reinterpret_cast<VirtualSetSlotValueFn>(CSF_AT(void**, textureObj, 0)[7])(
                textureObj, nullptr, slotValue);
        }
    }

    return result;
}

HOOK(0x0051B660, SceneTable_ClearSlotState, "auto");
