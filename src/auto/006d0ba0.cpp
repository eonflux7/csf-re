#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
CSF_FUNC(uint32_t*, __fastcall, Find, 0x006D0920, (void* self, void* edx, uint32_t width, uint32_t height, uint32_t depth, uint32_t flags));
CSF_FUNC(uint32_t*, __cdecl, Alloc, 0x006910F0, (size_t bytes));
CSF_FUNC(uint32_t*, __stdcall, Buy, 0x006D07C0, (uint32_t* next, uint32_t* prev, uint32_t** value));
CSF_FUNC(uint32_t, __fastcall, Inc, 0x0073FE80, (void* self, void* edx, uint32_t count));
CSF_FUNC(uint32_t, __cdecl, RasterCreate, 0x00627770, (uint32_t width, uint32_t height, uint32_t depth, uint32_t flags));
CSF_FUNC(uint32_t, __cdecl, TextureCreate, 0x00631780, (uint32_t raster));
static uint32_t __fastcall CreateManagedRaster(uint32_t* self, void*, uint32_t width, uint32_t height,
    uint32_t depth, uint32_t flags, uint32_t* texture) {
    uint32_t* record = Find(self, nullptr, width, height, depth, flags);
    if (!record) {
        uint32_t* block = Alloc(28);
        if (block) { block[6] = 0; record = block; }
        record[2] = width; record[3] = height;
        reinterpret_cast<uint8_t*>(record)[21] = static_cast<uint8_t>(flags);
        record[4] = depth;
        auto* head = reinterpret_cast<uint32_t*>(self[11]);
        uint32_t* value = record;
        value = Buy(head, reinterpret_cast<uint32_t*>(head[1]), &value);
        Inc(self + 10, nullptr, 1);
        head[1] = reinterpret_cast<uintptr_t>(value);
        *reinterpret_cast<uint32_t**>(value[1]) = value;
        record[0] = RasterCreate(width, height, depth, static_cast<uint8_t>(flags) ? 5 : 0);
        record[1] = TextureCreate(record[0]);
    }
    ++record[6];
    if (texture) *texture = record[1];
    return record[0];
}
HOOK(0x006D0BA0, CreateManagedRaster, "stl");
