#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original engine functions, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, (void));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, (void));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (int index, void* /*edx*/));

// Float constant (0.85f) used to inset the centered frame.
CSF_GLOBAL(float, g_FrameInsetScale, 0x008405DC);

// Fetches the size record of the texture selected by the global index.
// Reads of the record's first and second dwords are done by the caller.
static int* CMenuLoading_GetReferenceSizeRecord()
{
    void* texture = LookupObjectById(GetGlobalInt(), nullptr);
    void* sizeRecord = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, texture, 0)[6])(texture, nullptr);
    return static_cast<int*>(sizeRecord);
}

// Truncating float-style scale: (int)((double)value * 0.85f).
static int CMenuLoading_ScaleByFrameFactor(int value)
{
    return static_cast<int>(static_cast<std::int64_t>(
        static_cast<double>(value) * g_FrameInsetScale));
}

// Sets one endpoint of the frame outline object (virtual slot = 7 or 9),
// passing a pointer to a two-int point.
static void CMenuLoading_SetFrameEdgePoint(CMenuLoading* self, int slot, int x, int y)
{
    int point[2] = { x, y };
    void* frame = reinterpret_cast<void*>(self->field_004c);
    reinterpret_cast<void(__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, frame, 0)[slot])(frame, nullptr, point);
}

// Commits the current edge on the frame outline object (virtual slot 2).
static int CMenuLoading_CommitFrameEdge(CMenuLoading* self)
{
    void* frame = reinterpret_cast<void*>(self->field_004c);
    return reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, frame, 0)[2])(frame, nullptr);
}

static int __fastcall CMenuLoading_LayoutCenteredFrameEdges(CMenuLoading* self, void* /*edx*/)
{
    const int globalValue = GetGlobalValue_8BC1E0();
    void* globalContext = reinterpret_cast<void*>(static_cast<std::uintptr_t>(globalValue));
    if (CSF_AT(uint8_t, globalContext, 0xB30) == 0)
        return globalValue;
    if (self->field_004c == 0)
        return 0;

    const int referenceWidth = *CMenuLoading_GetReferenceSizeRecord();
    const int referenceHeight = CMenuLoading_GetReferenceSizeRecord()[1];

    const int scaledWidth = CMenuLoading_ScaleByFrameFactor(referenceWidth);
    const int scaledHeight = CMenuLoading_ScaleByFrameFactor(referenceHeight);

    // Signed halving, as in the original (x / 2 with truncation toward zero).
    const int marginX = (referenceWidth - scaledWidth) / 2;
    const int marginY = (referenceHeight - scaledHeight) / 2;
    const int right = marginX + scaledWidth;
    const int bottom = marginY + scaledHeight;

    // Top edge.
    CMenuLoading_SetFrameEdgePoint(self, 7, marginX, marginY);
    CMenuLoading_SetFrameEdgePoint(self, 9, right, marginY);
    CMenuLoading_CommitFrameEdge(self);

    // Bottom edge.
    CMenuLoading_SetFrameEdgePoint(self, 7, marginX, bottom);
    CMenuLoading_SetFrameEdgePoint(self, 9, right, bottom);
    CMenuLoading_CommitFrameEdge(self);

    // Left edge.
    CMenuLoading_SetFrameEdgePoint(self, 7, marginX, marginY);
    CMenuLoading_SetFrameEdgePoint(self, 9, marginX, bottom);
    CMenuLoading_CommitFrameEdge(self);

    // Right edge. Result of the final commit is the function result.
    CMenuLoading_SetFrameEdgePoint(self, 7, right, marginY);
    CMenuLoading_SetFrameEdgePoint(self, 9, right, bottom);
    return CMenuLoading_CommitFrameEdge(self);
}

HOOK(0x005385D0, CMenuLoading_LayoutCenteredFrameEdges, "auto");
