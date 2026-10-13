#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, Object_DestroyPendingState, 0x006B5E90, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(unsigned int, __fastcall, C_00860288_ConfigureObjectRenderRegions, 0x006C6C60, (C_00860288* self, void* , C_0085e7b8* object));
CSF_FUNC(int, __cdecl, GfxTextureLoader_ResetRecordRange, 0x006DEC40, (int* begin, int* end));

static unsigned int __fastcall Object_ApplyPendingState(void* self, void* /*edx*/, int /*unused*/)
{
    uint32_t recordOffset = 0;
    for (uint32_t i = 0; i < CSF_AT(uint16_t, self, 0x1C); ++i)
    {
        void* child = CSF_AT(void**, self, 0x18)[i];
        void* destination =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, child, 0)[0x18 / 4])(child, nullptr);

        void* pending = CSF_AT(void*, self, 0x78);
        auto* record = CSF_AT(uint8_t*, pending, 0x04) + recordOffset;
        uint32_t begin = CSF_AT(uint32_t, record, 0x04);
        uint32_t count = begin
            ? static_cast<uint32_t>(
                static_cast<int32_t>(CSF_AT(uint32_t, record, 0x08) - begin) >> 2)
            : 0;

        for (uint32_t j = 0; j < count; ++j)
        {
            pending = CSF_AT(void*, self, 0x78);
            record = CSF_AT(uint8_t*, pending, 0x04) + recordOffset;
            void* entry = CSF_AT(void**, record, 0x04)[j];
            if (entry)
            {
                void* object =
                    reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, entry, 0)[0x18 / 4])(entry, nullptr);
                if (object)
                {
                    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                        CSF_AT(void**, destination, 0)[0x7C / 4])(
                            destination, nullptr, object);
                }
            }
        }
        recordOffset += 0x10;
    }

    void* pending = CSF_AT(void*, self, 0x78);
    int* records = CSF_AT(int*, pending, 0x04);
    if (records)
    {
        GfxTextureLoader_ResetRecordRange(records, CSF_AT(int*, pending, 0x08));
        if (CSF_AT(void*, pending, 0x04))
            Mem_FreeDebug(CSF_AT(void*, pending, 0x04));
    }
    CSF_AT(uint32_t, pending, 0x04) = 0;
    CSF_AT(uint32_t, pending, 0x08) = 0;
    CSF_AT(uint32_t, pending, 0x0C) = 0;

    pending = CSF_AT(void*, self, 0x78);
    uint32_t begin = CSF_AT(uint32_t, pending, 0x14);
    uint32_t count = begin
        ? static_cast<uint32_t>(
            static_cast<int32_t>(CSF_AT(uint32_t, pending, 0x18) - begin) >> 2)
        : 0;

    for (uint32_t j = 0; j < count; ++j)
    {
        pending = CSF_AT(void*, self, 0x78);
        void* entry = CSF_AT(void**, pending, 0x14)[j];
        if (entry)
        {
            void* object =
                reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, entry, 0)[0x18 / 4])(entry, nullptr);
            if (object)
            {
                void* destination = static_cast<uint8_t*>(self) + 0x28;
                reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, destination, 0)[0x7C / 4])(
                        destination, nullptr, object);
            }
        }
    }

    auto* entries = CSF_AT(uint8_t*, self, 0x78) + 0x10;
    if (CSF_AT(void*, entries, 0x04))
        Mem_FreeDebug(CSF_AT(void*, entries, 0x04));
    CSF_AT(uint32_t, entries, 0x04) = 0;
    CSF_AT(uint32_t, entries, 0x08) = 0;
    CSF_AT(uint32_t, entries, 0x0C) = 0;

    pending = CSF_AT(void*, self, 0x78);
    if (CSF_AT(uint8_t, pending, 0x20))
    {
        void* controller = CSF_AT(void*, self, 0x64);
        if (controller)
        {
            bool active = reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, controller, 0)[0x28 / 4])(controller, nullptr);
            if (!active)
            {
                controller = CSF_AT(void*, self, 0x64);
                if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, controller, 0)[0x2C / 4])(controller, nullptr))
                {
                    controller = CSF_AT(void*, self, 0x64);
                    reinterpret_cast<void(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, controller, 0)[0x20 / 4])(controller, nullptr);
                }
            }

            controller = CSF_AT(void*, self, 0x64);
            if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, controller, 0)[0x28 / 4])(controller, nullptr))
            {
                void* state = CSF_AT(uint8_t*, self, 0x78) + 0x30;
                controller = CSF_AT(void*, self, 0x64);
                reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, controller, 0)[0x60 / 4])(controller, nullptr, state);

                state = CSF_AT(uint8_t*, self, 0x78) + 0x24;
                controller = CSF_AT(void*, self, 0x64);
                reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, controller, 0)[0x4C / 4])(controller, nullptr, state);
            }
        }
        else
        {
            CSF_AT(uint32_t, self, 0x5C) &= ~0x200u;
        }
    }

    pending = CSF_AT(void*, self, 0x78);
    if (pending)
    {
        Object_DestroyPendingState(pending, nullptr);
        Mem_Free(pending);
        CSF_AT(void*, self, 0x78) = nullptr;
    }

    void* owner = static_cast<uint8_t*>(self) - 8;
    bool enabled = (CSF_AT(uint32_t, self, 0x5C) & 0x40) == 0x40;
    reinterpret_cast<void(__fastcall*)(void*, void*, bool)>(
        CSF_AT(void**, owner, 0)[0x3C / 4])(owner, nullptr, enabled);

    enabled = (CSF_AT(uint32_t, self, 0x5C) & 0x100) == 0x100;
    reinterpret_cast<void(__fastcall*)(void*, void*, bool)>(
        CSF_AT(void**, owner, 0)[0x34 / 4])(owner, nullptr, enabled);

    enabled = (CSF_AT(uint32_t, self, 0x5C) & 0x80) == 0x80;
    unsigned int result =
        reinterpret_cast<unsigned int(__fastcall*)(void*, void*, bool)>(
            CSF_AT(void**, owner, 0)[0x48 / 4])(owner, nullptr, enabled);

    if ((CSF_AT(uint8_t, self, 0x5C) & 0x18) != 0)
    {
        auto* renderer = reinterpret_cast<C_00860288*>(GetGlobalValue_6C5760());
        return C_00860288_ConfigureObjectRenderRegions(
            renderer, nullptr, static_cast<C_0085e7b8*>(owner));
    }
    return result;
}

HOOK(0x006B6470, Object_ApplyPendingState, "auto");
