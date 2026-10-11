#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MissionScript_GetEntryCount_46BEA0, 0x0046BEA0, (S_0046bea0* self, void*));
CSF_FUNC(int*, __fastcall, MenuManager_ClearElementArray, 0x0050C230, (int* self, void*));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_FrameInsetScale, 0x008405DC);

static int __fastcall S_0050f2f0_DrawInsetFrameAndEntries(
    S_0050f2f0* self, void* /*edx*/)
{
    int result = GetGlobalValue_8BC1E0();
    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0xB30))
        return result;

    result = static_cast<int>(self->field_00ac);
    if (!result)
        return result;
    result = static_cast<int>(self->field_00b0);
    if (!result)
        return result;

    alignas(8) uint8_t endpointBytes[0x8];
    alignas(8) uint8_t textPositionBytes[0x8];
    alignas(8) uint8_t secondEndpointBytes[0x8];
    alignas(8) uint8_t firstEndpointBytes[0x8];
    int* endpoint = reinterpret_cast<int*>(endpointBytes);
    int* textPosition = reinterpret_cast<int*>(textPositionBytes);
    int* secondEndpoint = reinterpret_cast<int*>(secondEndpointBytes);
    int* firstEndpoint = reinterpret_cast<int*>(firstEndpointBytes);

    void* display = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    const int width = CSF_AT(int, dimensions, 0);
    secondEndpoint[1] = width;

    display = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);
    const int height = CSF_AT(int, dimensions, 4);
    firstEndpoint[1] = height;

    secondEndpoint[1] = static_cast<int>(width * g_FrameInsetScale);
    textPosition[1] = static_cast<int>(height * g_FrameInsetScale);
    const int left = (width - secondEndpoint[1]) / 2;
    const int top = (height - textPosition[1]) / 2;

    firstEndpoint[0] = left;
    firstEndpoint[1] = top;
    void* line = reinterpret_cast<void*>(self->field_00ac);
    reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, line, 0)[0x1C / 4])(line, nullptr, firstEndpoint);

    secondEndpoint[0] = left + secondEndpoint[1];
    firstEndpoint[1] = secondEndpoint[0];
    secondEndpoint[1] = top;
    line = reinterpret_cast<void*>(self->field_00ac);
    reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, line, 0)[0x24 / 4])(line, nullptr, secondEndpoint);
    line = reinterpret_cast<void*>(self->field_00ac);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, line, 0)[8 / 4])(line, nullptr);

    endpoint[1] = top + textPosition[1];
    secondEndpoint[1] = endpoint[1];
    endpoint[0] = left;
    line = reinterpret_cast<void*>(self->field_00ac);
    reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, line, 0)[0x1C / 4])(line, nullptr, endpoint);

    endpoint[0] = firstEndpoint[1];
    endpoint[1] = secondEndpoint[1];
    line = reinterpret_cast<void*>(self->field_00ac);
    reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, line, 0)[0x24 / 4])(line, nullptr, endpoint);
    line = reinterpret_cast<void*>(self->field_00ac);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, line, 0)[8 / 4])(line, nullptr);

    endpoint[0] = left;
    endpoint[1] = top;
    line = reinterpret_cast<void*>(self->field_00ac);
    reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, line, 0)[0x1C / 4])(line, nullptr, endpoint);

    endpoint[1] = secondEndpoint[1];
    endpoint[0] = left;
    line = reinterpret_cast<void*>(self->field_00ac);
    reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, line, 0)[0x24 / 4])(line, nullptr, endpoint);
    line = reinterpret_cast<void*>(self->field_00ac);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, line, 0)[8 / 4])(line, nullptr);

    endpoint[0] = firstEndpoint[1];
    endpoint[1] = top;
    line = reinterpret_cast<void*>(self->field_00ac);
    reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, line, 0)[0x1C / 4])(line, nullptr, endpoint);

    endpoint[0] = firstEndpoint[1];
    endpoint[1] = secondEndpoint[1];
    line = reinterpret_cast<void*>(self->field_00ac);
    reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, line, 0)[0x24 / 4])(line, nullptr, endpoint);
    line = reinterpret_cast<void*>(self->field_00ac);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, line, 0)[8 / 4])(line, nullptr);

    result = static_cast<int>(self->field_0014);
    if (!result)
        return result;

    MenuManager_ClearElementArray(
        reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 0xB4), nullptr);
    void* source = reinterpret_cast<void*>(self->field_0014);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, source, 0)[0xB0 / 4])(
            source, nullptr, reinterpret_cast<uint8_t*>(self) + 0xB4);

    // The packet also identifies this direct-call address as a vtable reference.
    (void)CSF_VTBL(0x0046BEA0);
    result = MissionScript_GetEntryCount_46BEA0(
        reinterpret_cast<S_0046bea0*>(reinterpret_cast<uint8_t*>(self) + 0xB4),
        nullptr);
    textPosition[0] = left;
    textPosition[1] = top;
    if (result)
    {
        uint32_t entryOffset = 0;
        uint32_t remaining = static_cast<uint32_t>(result);
        do
        {
            uint8_t* entry = reinterpret_cast<uint8_t*>(self->field_00b8) + entryOffset;
            const char* text;
            if (CSF_AT(uint32_t, entry, 0x18) < 0x10)
                text = reinterpret_cast<const char*>(entry + 4);
            else
                text = CSF_AT(const char*, entry, 4);

            void* renderer = reinterpret_cast<void*>(self->field_00b0);
            reinterpret_cast<void (__fastcall*)(void*, void*, const char*)>(
                CSF_AT(void**, renderer, 0)[0x3C / 4])(renderer, nullptr, text);

            renderer = reinterpret_cast<void*>(self->field_00b0);
            reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
                CSF_AT(void**, renderer, 0)[0x50 / 4])(renderer, nullptr, textPosition);

            renderer = reinterpret_cast<void*>(self->field_00b0);
            reinterpret_cast<void (__fastcall*)(void*, void*)>(
                CSF_AT(void**, renderer, 0)[8 / 4])(renderer, nullptr);

            renderer = reinterpret_cast<void*>(self->field_00b0);
            result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, renderer, 0)[0xA0 / 4])(renderer, nullptr);
            entryOffset += 28;
            --remaining;
            textPosition[1] += result;
        } while (remaining);
    }
    return result;
}

HOOK(0x0050F2F0, S_0050f2f0_DrawInsetFrameAndEntries, "auto");
