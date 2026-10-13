#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (void* context));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, CoverTask_ProcessRecord, 0x004C9B10, (S_0041e5f0* self, void* , char* record));
CSF_FUNC(int, __fastcall, AiStateFactory_HasObjectReference, 0x004DB5F0, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_ObjectReferenceManager, 0x008B0550);

static bool AiSearchOrders_BeginArchiveField(void* context)
{
    const bool ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
    }
    return ready;
}

static void AiSearchOrders_SerializeByte(void* context, uint8_t* value)
{
    if (AiSearchOrders_BeginArchiveField(context))
    {
        void* stream = CSF_AT(void*, context, 4);
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint8_t*, void*)>(
                CSF_AT(void**, stream, 0)[0x44 / 4])(
                    stream, nullptr, value, context);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }
}

static void AiSearchOrders_SerializeInteger(void* context, uint32_t* value)
{
    if (AiSearchOrders_BeginArchiveField(context))
    {
        void* stream = CSF_AT(void*, context, 4);
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*, void*)>(
                CSF_AT(void**, stream, 0)[0x38 / 4])(
                    stream, nullptr, value, context);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }
}

static void AiSearchOrders_SerializeFloat(void* context, void* value)
{
    if (AiSearchOrders_BeginArchiveField(context))
    {
        void* stream = CSF_AT(void*, context, 4);
        const int result =
            reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, stream, 0)[0x54 / 4])(
                    stream, nullptr, value, context);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }
}

static void AiSearchOrders_SerializeReference(void* context, void* member)
{
    uint8_t present = 0;
    if (CSF_AT(uint8_t, context, 0x14) != 0)
    {
        present = static_cast<uint8_t>(
            AiStateFactory_HasObjectReference(
                g_ObjectReferenceManager, nullptr,
                reinterpret_cast<int>(member)));
    }

    AiSearchOrders_SerializeByte(context, &present);

    if (present != 0)
    {
        const bool ready = CSF_AT(uint8_t, context, 0) == 0;
        alignas(8) uint8_t referenceBytes[0x4];
        CSF_AT(void*, referenceBytes, 0) = member;
        CSF_AT(uint32_t, context, 0x1C) = 0;
        if (ready)
        {
            CSF_AT(uint8_t, context, 0x15) = 1;
            CSF_AT(uint8_t, context, 0x16) = 0;
            Archive_SerializeObjectRef(
                referenceBytes, reinterpret_cast<bool*>(context));
        }
    }
}

static int __fastcall TOrden_Serialize(
    TOrden* self, void* /*edx*/, int archiveAddress)
{
    void* context = reinterpret_cast<void*>(archiveAddress);
    const uint32_t savedRecord = CSF_AT(uint32_t, context, 0x1C);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* stream = CSF_AT(void*, context, 4);
        // The original tests the whole AL byte, not a canonical C++ bool.
        failed =
            !reinterpret_cast<uint8_t (__fastcall*)(void*, void*, int, void*)>(
                CSF_AT(void**, stream, 0)[0x80 / 4])(
                    stream, nullptr, 0, context);
    }
    CSF_AT(uint8_t, context, 0) = failed;
    AudioSoundManager_ProcessArchiveContext(context);

    AiSearchOrders_SerializeReference(context, self->pad_0004);
    AiSearchOrders_SerializeReference(context, self->pad_0004 + 4);
    AiSearchOrders_SerializeReference(context, self->pad_0004 + 8);
    AiSearchOrders_SerializeReference(context, self->pad_0004 + 12);
    AiSearchOrders_SerializeReference(context, self->pad_0004 + 16);
    AiSearchOrders_SerializeReference(context, self->pad_0004 + 24);
    AiSearchOrders_SerializeReference(context, self->pad_0004 + 20);
    AiSearchOrders_SerializeReference(context, self->pad_0004 + 28);
    AiSearchOrders_SerializeReference(context, self->pad_0004 + 32);
    AiSearchOrders_SerializeReference(context, self->pad_0004 + 36);
    AiSearchOrders_SerializeReference(context, self->pad_0004 + 40);
    AiSearchOrders_SerializeReference(context, self->pad_0004 + 44);
    AiSearchOrders_SerializeReference(context, self->pad_0004 + 48);
    AiSearchOrders_SerializeReference(context, self->pad_0004 + 84);

    if (AiSearchOrders_BeginArchiveField(context))
    {
        CoverTask_ProcessRecord(
            reinterpret_cast<S_0041e5f0*>(&self->field_0064),
            nullptr, reinterpret_cast<char*>(context));
    }

    if (AiSearchOrders_BeginArchiveField(context))
    {
        Archive_SerializeString(
            reinterpret_cast<int>(self->pad_0094),
            reinterpret_cast<S_00691860*>(context));
    }

    AiSearchOrders_SerializeByte(context, self->pad_00ac + 4);
    AiSearchOrders_SerializeByte(context, self->pad_00ac + 5);
    AiSearchOrders_SerializeInteger(context, &self->field_00b4);
    AiSearchOrders_SerializeInteger(context, &self->field_00a0);
    AiSearchOrders_SerializeFloat(context, self->pad_00ac);
    // Original: vtbl[0x54/4](stream, this + 1, context); this + 1 is sizeof(TOrden) = 0xC0, not field_0080.
    AiSearchOrders_SerializeFloat(context, self + 1);

    CSF_AT(uint32_t, context, 0x1C) = savedRecord;

    if (CSF_AT(uint8_t, context, 0) != 0)
    {
        CSF_AT(uint8_t, context, 0) = 1;
        return 1;
    }

    void* stream = CSF_AT(void*, context, 4);
    const int result =
        reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, stream, 0)[0x7C / 4])(
                stream, nullptr, context);

    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, context, 0) = 1;
        return 1;
    }

    CSF_AT(uint8_t, context, 0) = 0;
    return result;
}

HOOK(0x004CCDB0, TOrden_Serialize, "auto");
