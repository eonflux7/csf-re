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

    if (!sub_6C8620(sprite, nullptr))
        return;

    // stat_provider->vtbl[152/4](100)
    int value_bits = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, stat_provider, 0)[38])(stat_provider, nullptr, 100);
    if (value_bits == -1)
        return;


    // stat_provider->vtbl[140/4](value_bits): the entry for that index
    void* target = reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, stat_provider, 0)[35])(stat_provider, nullptr, value_bits);

    // hud->vtbl[164/4](hud, out_buffer): fills a 12-byte record and returns a pointer to it
    unsigned char record_buffer[12];
    void* record = reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, hud, 0)[41])(hud, nullptr, record_buffer);
    int record_field = CSF_AT(int, record, 4);

    // target->vtbl[24/4](): no argument. The record's +4 the original pushes before it is 0x6C8740's float (it pops 8
    // and scales it by 0.5 onto a y coordinate); passing it to the slot left the stack 4 bytes off (the flaky
    // loadsave 1.sav crash: eip on the stack, ebp = 2)
    int formatted = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, target, 0)[6])(target, nullptr);

    float record_y;
    std::memcpy(&record_y, &record_field, sizeof(record_y));
    sub_6C8740(sprite, nullptr, formatted, record_y);
}

HOOK(0x0042DA20, Hud_DrawDebugStatValue, "auto");
