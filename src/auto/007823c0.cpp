#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
CSF_FUNC(void*, __fastcall, Clear, 0x007822B0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, InitFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(uint8_t, __fastcall, Load, 0x00694770, (void* self, void* edx, uint32_t filename));
CSF_FUNC(void*, __fastcall, Property, 0x006932B0, (void* self, void* edx, const char* key));
CSF_FUNC(int32_t, __fastcall, ChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, Component, 0x00692BD0, (void* self, void* edx, uint32_t index));
CSF_FUNC(uint32_t, __fastcall, Integer, 0x00693970, (void* self, void* edx, const char* key));
CSF_FUNC(double, __fastcall, Float, 0x00693990, (void* self, void* edx, const char* key));
CSF_FUNC(uint32_t*, __cdecl, Alloc, 0x006910F0, (size_t bytes));
CSF_FUNC(uint32_t*, __stdcall, Buy, 0x00782220, (uint32_t* next, uint32_t* prev, uint32_t** value));
CSF_FUNC(uint32_t, __fastcall, Inc, 0x0073FE80, (void* self, void* edx, uint32_t count));
CSF_FUNC(uint32_t, __fastcall, Close, 0x00694380, (void* self, void* edx));
CSF_GLOBAL(const char*, Key008A9BDC, 0x008A9BDC);
CSF_GLOBAL(const char*, Key008A9BE0, 0x008A9BE0);
CSF_GLOBAL(const char*, Key008A9BE4, 0x008A9BE4);
CSF_GLOBAL(const char*, Key008A9BE8, 0x008A9BE8);
CSF_GLOBAL(const char*, Key008A9BEC, 0x008A9BEC);
CSF_GLOBAL(const char*, Key008A9BF0, 0x008A9BF0);
static uint8_t __fastcall ParseDistances(uint8_t* self, void*, uint32_t filename) {
    Clear(self, nullptr);
    alignas(4) uint8_t file[64];
    InitFile(file, nullptr);
    uint8_t loaded = Load(file, nullptr, filename);
    if (loaded) {
        void* root = *reinterpret_cast<void**>(file);
        void* distances = Property(root, nullptr, Key008A9BDC);
        if (distances) {
            int32_t count = ChildCount(distances, nullptr);
            for (int32_t i = 0; i < count; ++i) {
                void* entry = Component(distances, nullptr, i);
                uint32_t key = Integer(entry, nullptr, Key008A9BE4);
                float distance = static_cast<float>(Float(entry, nullptr, Key008A9BE8));
                float min = static_cast<float>(Float(entry, nullptr, Key008A9BEC));
                float alpha = static_cast<float>(Float(entry, nullptr, Key008A9BF0));
                uint32_t* record = Alloc(16);
                record[0] = key;
                reinterpret_cast<float*>(record)[1] = distance;
                reinterpret_cast<float*>(record)[2] = min;
                reinterpret_cast<float*>(record)[3] = alpha;
                auto* head = *reinterpret_cast<uint32_t**>(self + 8);
                uint32_t* value = record;
                uint32_t* node = Buy(head, reinterpret_cast<uint32_t*>(head[1]), &value);
                Inc(self + 4, nullptr, 1);
                head[1] = reinterpret_cast<uintptr_t>(node);
                *reinterpret_cast<uint32_t**>(node[1]) = node;
            }
        }
        root = *reinterpret_cast<void**>(file);
        void* scale = Property(root, nullptr, Key008A9BE0);
        if (scale) {
            void* entry = Component(scale, nullptr, 0);
            *reinterpret_cast<float*>(self + 16) = static_cast<float>(Float(entry, nullptr, Key008A9BE0));
        } else *reinterpret_cast<uint32_t*>(self + 16) = 0x3F000000u;
        Close(file, nullptr);
        return loaded;
    }
    *reinterpret_cast<uint32_t*>(self + 16) = 0x3F800000u;
    Close(file, nullptr);
    return 0;
}
HOOK(0x007823C0, ParseDistances, "stl");
