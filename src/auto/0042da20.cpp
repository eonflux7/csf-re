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

// Hud widget/sprite helpers (bound by address)
CSF_FUNC(int, __fastcall, sub_6C8620, 0x006C8620, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_6C8740, 0x006C8740, (void* self, void* edx, int a2, float a3));

static void __stdcall Hud_DrawDebugStatValue(int a1, int a2)
{
    if (!a2)
        return;
    if (!a1)
        return;

    void* stat_source = reinterpret_cast<void*>(a2);
    void* hud = reinterpret_cast<void*>(a1);

    // stat_source->vtbl[168/4]()
    void* stat_provider = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, stat_source, 0)[42])(stat_source, nullptr);
    if (!stat_provider)
        return;

    // stat_source->vtbl[172/4]()
    void* sprite = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, stat_source, 0)[43])(stat_source, nullptr);
    if (!sprite)
        return;

    if (!static_cast<unsigned char>(sub_6C8620(sprite, nullptr)))
        return;

    // stat_provider->vtbl[152/4](100)
    int value_bits = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, stat_provider, 0)[38])(stat_provider, nullptr, 100);
    if (value_bits == -1)
        return;


    // The pushed value_bits remains on the stack as 0x6C8740's duration.
    void* target = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, stat_provider, 0)[35])(stat_provider, nullptr);

    // hud->vtbl[164/4](hud, out_buffer): fills a 12-byte record and returns a pointer to it
    unsigned char record_buffer[12];
    void* record = reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, hud, 0)[41])(hud, nullptr, record_buffer);
    int record_field = CSF_AT(int, record, 4);

    // This slot consumes the record's +4; the earlier duration is still pending.
    int formatted = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, target, 0)[6])(target, nullptr, record_field);

    float record_y;
    std::memcpy(&record_y, &value_bits, sizeof(record_y));
    sub_6C8740(sprite, nullptr, formatted, record_y);
}

HOOK(0x0042DA20, Hud_DrawDebugStatValue, "auto");
