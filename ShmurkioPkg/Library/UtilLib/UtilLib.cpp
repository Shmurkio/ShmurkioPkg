#include <Library/UtilLib.hpp>

void*
EFIAPI
Util::GetImageBase(
    _In_ void* Address
)
{
    if (reinterpret_cast<uint64_t>(Address) < EFI_PAGE_SIZE)
    {
        return nullptr;
    }

    uint64_t ImageBase = reinterpret_cast<uint64_t>(Address);
    ImageBase &= ~(EFI_PAGE_SIZE - 1);

    while (ImageBase >= EFI_PAGE_SIZE)
    {
        EFI_IMAGE_DOS_HEADER* DosHeader = reinterpret_cast<EFI_IMAGE_DOS_HEADER*>(ImageBase);

        if (DosHeader->e_magic == EFI_IMAGE_DOS_SIGNATURE)
        {
            if (DosHeader->e_lfanew >= sizeof(EFI_IMAGE_DOS_HEADER) && DosHeader->e_lfanew < 1024)
            {
                EFI_IMAGE_NT_HEADERS64* PeHeader = reinterpret_cast<EFI_IMAGE_NT_HEADERS64*>(ImageBase + DosHeader->e_lfanew);

                if (PeHeader->Signature == EFI_IMAGE_NT_SIGNATURE)
                {
                    return reinterpret_cast<void*>(ImageBase);
                }
            }
        }

        ImageBase -= EFI_PAGE_SIZE;
    }

    return nullptr;
}

uint64_t
EFIAPI
Util::GetImageSize(
    _In_ void* ImageBase
)
{
    if (!ImageBase)
    {
        return 0;
    }

    EFI_IMAGE_DOS_HEADER* DosHeader = reinterpret_cast<EFI_IMAGE_DOS_HEADER*>(ImageBase);

    if (DosHeader->e_magic != EFI_IMAGE_DOS_SIGNATURE)
    {
        return 0;
    }

    EFI_IMAGE_NT_HEADERS64* NtHeaders = reinterpret_cast<EFI_IMAGE_NT_HEADERS64*>(reinterpret_cast<uint64_t>(ImageBase) + DosHeader->e_lfanew);

    if (NtHeaders->Signature != EFI_IMAGE_NT_SIGNATURE)
    {
        return 0;
    }

    return NtHeaders->OptionalHeader.SizeOfImage;
}

#define IN_RANGE(x, a, b) (x >= a && x <= b)
#define GET_BITS(x) (IN_RANGE((x&(~0x20)),'A','F')?((x&(~0x20))-'A'+0xA):(IN_RANGE(x,'0','9')?x-'0':0))
#define GET_BYTE(a, b) (GET_BITS(a) << 4 | GET_BITS(b))

uint64_t
__cdecl
Util::FindPattern(
    _In_ void* BaseAddress,
    _In_ uint64_t Size,
    _In_ const char* Pattern
)
{
    uint8_t* FirstMatch = nullptr;
    const char* CurrentPattern = Pattern;
    uint8_t* Start = static_cast<uint8_t*>(BaseAddress);
    uint8_t* End = Start + Size;

    for (uint8_t* Current = Start; Current < End; Current++)
    {
        uint8_t Byte = CurrentPattern[0];

        if (!Byte)
        {
            return reinterpret_cast<uint64_t>(FirstMatch);
        }

        if (Byte == '\?' ||* static_cast<uint8_t*>(Current) == GET_BYTE(Byte, CurrentPattern[1]))
        {
            if (!FirstMatch)
            {
                FirstMatch = Current;
            }

            if (!CurrentPattern[2])
            {
                return reinterpret_cast<uint64_t>(FirstMatch);
            }

            if (Byte == '\?')
            {
                CurrentPattern += 2;
            }
            else
            {
                CurrentPattern += 3;
            }
        }
        else
        {
            CurrentPattern = Pattern;
            FirstMatch = nullptr;
        }
    }

    return 0;
}

extern "C"
{
    unsigned long long
    EFIAPI
    __readcr0(
        void
    );

    void
    EFIAPI
    __writecr0(
        _In_ unsigned long long Data
    );
}

void*
EFIAPI
Util::CopyToReadOnly(
    _In_ void* Destination,
    _In_ const void* Source,
    _In_ const uint64_t Size
)
{
    uint64_t Cr0 = __readcr0();
    uint64_t OrigCr0 = Cr0;
    Cr0 &= ~(1 << 16);
    __writecr0(Cr0);
    void* Return = CopyMem(Destination, Source, Size);
    __writecr0(OrigCr0);
    return Return;
}