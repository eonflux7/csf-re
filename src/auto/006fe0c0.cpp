#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, S_006fdfe0_FindWorldFromClump, 0x0058DF40, (int a1));
CSF_FUNC(int, __cdecl, RpWorldRemoveClump, 0x0058E310, (int a1, int a2));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, C_6B81C0_GetFirstItem, 0x006B81C0, (void* self, void*));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __cdecl, GfxFrames_GetProviderFrameObject, 0x006C0500, (int clump));

CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);

static char __fastcall S_006fe0c0_InitWaterBase(
    S_006fe0c0* self, void* /*edx*/, C_0085ec18* clump, char registerFrame)
{
    if (self->field_0004)
    {
        const std::uint32_t previousClump = self->field_0008;
        self->field_0004 = 0;
        if (previousClump)
        {
            const int world =
                S_006fdfe0_FindWorldFromClump(static_cast<int>(previousClump));
            if (world)
                RpWorldRemoveClump(world, static_cast<int>(self->field_0008));
        }
    }

    self->field_0008 = reinterpret_cast<std::uint32_t>(clump);
    CSF_AT(std::uint32_t, self, 0xC4) = 0;
    CSF_AT(std::uint8_t, self, 0xC0) = 0;

    if (clump)
    {
        const int frame =
            GfxFrames_GetProviderFrameObject(reinterpret_cast<int>(clump));
        void* attachment =
            reinterpret_cast<void*>(GfxObjectProps_GetObjectAttachment(frame));

        bool hasName = false;
        if (C_6B81C0_GetFirstItem(attachment, nullptr))
        {
            void* item =
                reinterpret_cast<void*>(C_6B81C0_GetFirstItem(attachment, nullptr));
            const std::uint8_t* name;
            if (CSF_AT(std::uint32_t, item, 0x18) < 0x10u)
                name = reinterpret_cast<std::uint8_t*>(item) + 4;
            else
                name = CSF_AT(const std::uint8_t*, item, 0x04);

            hasName = name[0] != g_byte_82E65C;
        }

        void* nameStorage = reinterpret_cast<std::uint8_t*>(self) + 0x98;
        if (hasName)
        {
            const void* item =
                reinterpret_cast<const void*>(C_6B81C0_GetFirstItem(attachment, nullptr));
            AssignStringSubstring(nameStorage, nullptr, item, 0, -1);
        }
        else
        {
            AssignStringRange_429B60(
                nameStorage, nullptr,
                const_cast<char*>(CSF_STR(0x0086300C, "Unnamed")), 7);
        }

        if (registerFrame)
        {
            const int providerFrame =
                GfxFrames_GetProviderFrameObject(static_cast<int>(self->field_0008));
            const int managerId = GetGlobalInt();
            void* manager =
                reinterpret_cast<void*>(ResolveClumpManager(managerId, nullptr));

            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, manager, 0)[0x8C / 4])(
                    manager, nullptr, providerFrame);
        }
    }

    reinterpret_cast<void(__fastcall*)(S_006fe0c0*, void*, std::uint8_t*)>(
        reinterpret_cast<void**>(self->field_0000)[0x0C / 4])(
            self, nullptr, reinterpret_cast<std::uint8_t*>(self) + 0x0C);

    self->field_0004 = 1;
    return 1;
}

HOOK(0x006FE0C0, S_006fe0c0_InitWaterBase, "auto");
