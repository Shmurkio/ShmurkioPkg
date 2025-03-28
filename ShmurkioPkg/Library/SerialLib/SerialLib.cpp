#include <Library/SerialLib.hpp>

EFI_STATUS
EFIAPI
Serial::Write(
    _Out_ const char* Format,
    ...
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    if (!SerialIo)
    {
        Status = gBS->LocateProtocol(&gEfiSerialIoProtocolGuid, nullptr, reinterpret_cast<void**>(&SerialIo));

        if (EFI_ERROR(Status))
        {
            return Status;
        }
    }

    VA_LIST Args = nullptr;
    char Buffer[256];
    uint64_t BufferSize = sizeof(Buffer);
    VA_START(Args, Format);
    BufferSize = AsciiVSPrint(Buffer, BufferSize, Format, Args);
    VA_END(Args);

    char ProcessedBuffer[2 * sizeof(Buffer)];
    uint64_t ProcessedBufferSize = 0;

    for (int i = 0; i < BufferSize; i++)
    {
        ProcessedBuffer[ProcessedBufferSize++] = Buffer[i];
        
        if (Buffer[i] == '\n' && (i == 0 || Buffer[i - 1] != '\r'))
        {
            ProcessedBuffer[ProcessedBufferSize++] = '\r';
        }
    }
    
    Status = SerialIo->Write(SerialIo, &ProcessedBufferSize, ProcessedBuffer);
    return Status;
}