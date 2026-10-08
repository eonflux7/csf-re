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

CSF_FUNC(void*, __cdecl, GetGlobalState, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (int objectId, void* edx));
CSF_FUNC(void*, __fastcall, GetMenuChildAccessor, 0x006683F0, (void* self, void* edx));

// Returns the function pointer stored at the given byte offset of the object's vtable.
static void* VtblSlot(void* obj, int byteOffset)
{
    return CSF_AT(void**, obj, 0)[byteOffset / 4];
}

static int __fastcall CMenuMain_UpdateLayoutAlignment(CMenuMain* self, void* /*edx*/)
{
    uint8_t* screenSubObject = reinterpret_cast<uint8_t*>(self) + 4;

    void* childAccessor = GetMenuChildAccessor(screenSubObject, nullptr);
    int isChildReady = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        VtblSlot(childAccessor, 0x30))(childAccessor, nullptr);
    if (isChildReady == 0)
        return 0;

    if (self->field_0034 != 0)
    {
        // Build an 8-dword viewport record from the global state object and the current screen size.
        void* globalState = GetGlobalState();
        void* stateSettings = CSF_AT(void*, globalState, 0x10);
        void* sourceRect = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            VtblSlot(stateSettings, 0x30))(stateSettings, nullptr);

        uint32_t viewportRect[8];
        std::memcpy(viewportRect, sourceRect, sizeof(viewportRect));

        void* widthObject = LookupObjectById(GetGlobalInt(), nullptr);
        uint32_t* widthData = static_cast<uint32_t*>(reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            VtblSlot(widthObject, 0x18))(widthObject, nullptr));
        uint32_t halfWidth = widthData[0] >> 1;

        void* heightObject = LookupObjectById(GetGlobalInt(), nullptr);
        uint32_t* heightData = static_cast<uint32_t*>(reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            VtblSlot(heightObject, 0x18))(heightObject, nullptr));
        uint32_t halfHeight = heightData[1] >> 1;

        viewportRect[5] = halfWidth;
        viewportRect[6] = halfHeight;

        void* globalStateAgain = GetGlobalState();
        void* stateSettingsAgain = CSF_AT(void*, globalStateAgain, 0x10);
        int updateResult = reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*)>(
            VtblSlot(stateSettingsAgain, 0x40))(stateSettingsAgain, nullptr, viewportRect);

        self->field_0034 = 0;
        return updateResult;
    }

    // Align the layout object with the offset between two child widgets.
    void* accessorA = GetMenuChildAccessor(screenSubObject, nullptr);
    void* accessorB = GetMenuChildAccessor(screenSubObject, nullptr);

    void* childA = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        VtblSlot(accessorA, 0x30))(accessorA, nullptr);
    int32_t childAPosY = CSF_AT(int32_t, childA, 0x18);

    void* childB = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        VtblSlot(accessorB, 0x30))(accessorB, nullptr);
    int32_t childBPosX = CSF_AT(int32_t, childB, 0x14);

    void* layoutObject = reinterpret_cast<void*>(self->field_0018);
    uint8_t outBuffer[8];
    int32_t* offsetVec = static_cast<int32_t*>(reinterpret_cast<void*(__fastcall*)(void*, void*, void*)>(
        VtblSlot(layoutObject, 0x160))(layoutObject, nullptr, outBuffer));

    int32_t moveVector[2];
    moveVector[0] = childBPosX - offsetVec[0];
    moveVector[1] = childAPosY - offsetVec[1];

    void* layoutObjectAgain = reinterpret_cast<void*>(self->field_0018);
    return reinterpret_cast<int(__fastcall*)(void*, void*, int32_t*, int)>(
        VtblSlot(layoutObjectAgain, 0x14C))(layoutObjectAgain, nullptr, moveVector, 1);
}

HOOK(0x0053A7D0, CMenuMain_UpdateLayoutAlignment, "auto");
