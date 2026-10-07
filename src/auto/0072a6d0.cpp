#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, CGuiPanel_GetPositionSource, 0x006683F0,
         (CGuiPanel* self, void* edx));

static uint32_t* __fastcall CGuiPanel_SynchronizePosition(
    CGuiPanel* self, void* /*edx*/)
{
    using GetSourceData = void* (__fastcall*)(void*, void*);
    using GetPosition = uint32_t* (__fastcall*)(void*, void*);
    using GetPositionOffset =
        uint32_t* (__fastcall*)(void*, void*, uint32_t*);
    using SetPosition =
        uint32_t* (__fastcall*)(void*, void*, uint32_t*, uint32_t);

    void* source = CGuiPanel_GetPositionSource(self, nullptr);
    if (!source)
        return nullptr;

    source = CGuiPanel_GetPositionSource(self, nullptr);
    void* sourceData =
        reinterpret_cast<GetSourceData>(
            CSF_AT(void**, source, 0)[0x30 / 4])(source, nullptr);
    if (!sourceData)
        return nullptr;

    void* verticalSource = CGuiPanel_GetPositionSource(self, nullptr);
    void* horizontalSource = CGuiPanel_GetPositionSource(self, nullptr);

    sourceData =
        reinterpret_cast<GetSourceData>(
            CSF_AT(void**, verticalSource, 0)[0x30 / 4])(
                verticalSource, nullptr);

    const auto getHorizontalData = reinterpret_cast<GetSourceData>(
        CSF_AT(void**, horizontalSource, 0)[0x30 / 4]);
    uint32_t sourceY = CSF_AT(uint32_t, sourceData, 0x18);

    sourceData = getHorizontalData(horizontalSource, nullptr);
    uint32_t sourceX = CSF_AT(uint32_t, sourceData, 0x14);

    uint32_t offsetStorage[2];
    uint32_t* offset =
        reinterpret_cast<GetPositionOffset>(
            CSF_AT(void**, self, 0)[0x160 / 4])(
                self, nullptr, offsetStorage);

    const uint32_t offsetY = offset[1];
    const uint32_t offsetX = offset[0];
    sourceY -= offsetY;

    const auto getCurrentPosition = reinterpret_cast<GetPosition>(
        CSF_AT(void**, self, 0)[0x148 / 4]);
    sourceX -= offsetX;

    uint32_t requestedPosition[2] = {sourceX, sourceY};
    uint32_t* result = getCurrentPosition(self, nullptr);

    if (requestedPosition[0] != result[0] ||
        requestedPosition[1] != result[1])
    {
        result = reinterpret_cast<SetPosition>(
            CSF_AT(void**, self, 0)[0x14C / 4])(
                self, nullptr, requestedPosition, 0);
    }

    return result;
}

HOOK(0x0072A6D0, CGuiPanel_SynchronizePosition, "auto");
