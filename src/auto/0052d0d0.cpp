#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, InitStringFromLiteral, 0x00692720, (void* returnSlot, const char* text, int count));
CSF_FUNC(int, __fastcall, MenuAccount_IsGateClear_696B60, 0x00696B60, (S_00696af0* self, void* , char* a2));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (S_004f21d0* self, void* , uint8_t* a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void, __fastcall, C_00720DF0_ForwardCallToChildVtblSlot16, 0x00720DF0,
         (void* self, void* edx, const char* text));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);
CSF_GLOBAL(float, g_LayoutWidthAdjustment, 0x00833E44);
CSF_GLOBAL(unsigned int, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(float, g_ColorBlack, 0x008A7DA0);
CSF_GLOBAL(float, g_ColorBlack_G, 0x008A7DA4);
CSF_GLOBAL(float, g_ColorBlack_B, 0x008A7DA8);
CSF_GLOBAL(float, g_ColorBlack_A, 0x008A7DAC);

static void* __fastcall C_008414e8_ResetProgressLayoutState(
    C_008414e8* self, void* /*edx*/)
{
    // Keep the original shared workspace: string, color, position and counter.
    alignas(8) uint8_t workspace[0x38];
    auto& text = *reinterpret_cast<S_00696af0*>(workspace);
    auto* stringObject = reinterpret_cast<S_004f21d0*>(workspace);
    auto* position = reinterpret_cast<int*>(workspace + 0x2C);
    auto* zeroVector = reinterpret_cast<float*>(workspace + 0x24);
    auto* color = reinterpret_cast<float*>(workspace + 0x1C);
    int& remaining = CSF_AT(int, workspace, 0x34);

    void* layout = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    ((void*(__fastcall*)(void*, void*))CSF_AT(void**, layout, 0)[0x18 / 4])(
        layout, nullptr);

    layout = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* dimensions =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, layout, 0)[0x18 / 4])(
            layout, nullptr);
    const float widthRatio =
        static_cast<float>(CSF_AT(int, dimensions, 0)) /
        static_cast<float>(g_ScreenWidth);
    position[0] = 0;
    position[1] = 0;
    CSF_AT(float, workspace, 0x34) = widthRatio;

    layout = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, layout, 0)[0x18 / 4])(
            layout, nullptr);
    position[1] = CSF_AT(int, dimensions, 4);
    const int64_t widthAdjustment = static_cast<int64_t>(
        CSF_AT(float, workspace, 0x34) * g_LayoutWidthAdjustment);
    CSF_AT(uint32_t, workspace, 0x2C) =
        self->field_0128 - static_cast<uint32_t>(widthAdjustment);

    uint32_t* childSlot = &self->field_0024;
    remaining = 4;
    do
    {
        zeroVector[0] = 0.0f;
        zeroVector[1] = 0.0f;
        void* child = reinterpret_cast<void*>(*childSlot);
        ((void(__fastcall*)(void*, void*, float*))
            CSF_AT(void**, child, 0)[0x98 / 4])(
                child, nullptr, zeroVector);

        child = reinterpret_cast<void*>(*childSlot);
        ((void(__fastcall*)(void*, void*, int*))
            CSF_AT(void**, child, 0)[0xA0 / 4])(
                child, nullptr, position);
        ++childSlot;
        --remaining;
    } while (remaining != 0);

    CSF_AT(uint32_t, workspace, 0x14) = 0;
    *reinterpret_cast<uint16_t*>(&text.field_0004) = 0;
    text.field_0018 = 7;

    childSlot = &self->field_0024;
    uint8_t* record = reinterpret_cast<uint8_t*>(self) + 0x44;
    remaining = 3;
    do
    {
        TextInputSubobject_Init(stringObject, nullptr, &g_byte_82E65C);
        InitStringFromLiteral(
            stringObject, CSF_AT(const char*, record, 0), 4);
        if (static_cast<uint8_t>(MenuAccount_IsGateClear_696B60(
                &text, nullptr, reinterpret_cast<char*>(&g_byte_82E65C))) != 0)
        {
            InitStringFromLiteral(
                stringObject, CSF_AT(const char*, record, 0), 8);
        }

        const char* caption = reinterpret_cast<const char*>(text.field_0004);
        if (text.field_0018 < 8)
            caption = reinterpret_cast<const char*>(&text.field_0004);
        C_00720DF0_ForwardCallToChildVtblSlot16(
            reinterpret_cast<void*>(*childSlot), nullptr, caption);

        if (CSF_AT(uint32_t, record, 4) != 0)
        {
            void* child = reinterpret_cast<void*>(*childSlot);
            void* renderer =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, child, 0)[0x110 / 4])(child, nullptr);
            const uint32_t font = CSF_AT(uint32_t, record, 4);
            ((void(__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, renderer, 0)[0x24 / 4])(
                    renderer, nullptr, font);
        }

        if (g_ColorBlack != CSF_AT(float, record, 8) ||
            g_ColorBlack_G != CSF_AT(float, record, 12) ||
            g_ColorBlack_B != CSF_AT(float, record, 16) ||
            g_ColorBlack_A != CSF_AT(float, record, 20))
        {
            color[0] = CSF_AT(float, record, 8) * g_ColorChannelScale;
            color[1] = CSF_AT(float, record, 12) * g_ColorChannelScale;
            color[2] = CSF_AT(float, record, 16) * g_ColorChannelScale;
            color[3] = 255.0f;

            void* child = reinterpret_cast<void*>(*childSlot);
            void* renderer =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, child, 0)[0x110 / 4])(child, nullptr);
            ((void(__fastcall*)(void*, void*, float*))
                CSF_AT(void**, renderer, 0)[0x60 / 4])(
                    renderer, nullptr, color);
        }

        uint32_t first = CSF_AT(uint32_t, record, 8);
        uint32_t second = CSF_AT(uint32_t, record, 12);
        CSF_AT(uint32_t, record, 44) = first;
        first = CSF_AT(uint32_t, record, 16);
        CSF_AT(uint32_t, record, 48) = second;
        second = CSF_AT(uint32_t, record, 20);
        CSF_AT(uint32_t, record, 52) = first;
        CSF_AT(uint32_t, record, 56) = second;

        CSF_AT(uint32_t, record, 60) = CSF_AT(uint32_t, record, 24);
        CSF_AT(uint32_t, record, 64) = CSF_AT(uint32_t, record, 28);
        first = CSF_AT(uint32_t, record, 32);
        second = CSF_AT(uint32_t, record, 36);
        CSF_AT(uint32_t, record, 68) = first;
        CSF_AT(uint32_t, record, 72) = second;

        ++childSlot;
        record += 76;
        --remaining;
    } while (remaining != 0);

    if (text.field_0018 >= 8)
    {
        void* block = reinterpret_cast<void*>(text.field_0004);
        if (block)
            return reinterpret_cast<void*>(Mem_FreeDebug(block));
        return block;
    }
    return nullptr;
}

HOOK(0x0052D0D0, C_008414e8_ResetProgressLayoutState, "auto");
