#include <iostream>
#include <vector>
#include <windows.h>
#include <locale.h>

using namespace std;

#pragma pack(push, 1)

const BYTE RarSignature[] = { 0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0x00 };

typedef struct
{
    WORD header_crc;
    BYTE header_type;
    WORD header_flags;
    WORD header_size;

} RarVolumeHeaderFormat;

typedef struct
{
    DWORD PackSize;
    DWORD UnpSize;
    BYTE HostOS;
    DWORD FileCRC;
    DWORD FileTime;
    BYTE UnpVer;
    BYTE Method;
    WORD NameSize;
    DWORD FileAttr;

} FILE_HEAD;

#pragma pack(pop)

int Rar4ListContents(const BYTE *dataBuffer, unsigned int bufferSize)
{
    int numberOfFiles = 0;
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

   // Проверить сигнатуру файла
    if (memcmp(dataBuffer, RarSignature, sizeof(RarSignature)) != 0)
    {
        cout << "Ошибка: Некорректная сигнатура архива." << endl;
        return -1;
    }

    // Отступить от начала файла на длину сигнатуры (7 байт)
    unsigned int fileOffset = sizeof(RarSignature);

    // Цикл по архиву
    while (fileOffset < bufferSize)
    {
        // Получаем указатель на заголовок текущей записи
        RarVolumeHeaderFormat *volumeHeader = (RarVolumeHeaderFormat*)&dataBuffer[fileOffset];

        // Проверяем тип записи.
        if (volumeHeader->header_type == 0x73)
        {
            // MAIN_HEAD, пропускаем
            fileOffset += volumeHeader->header_size;
        }
        else if (volumeHeader->header_type == 0x74)
        {
            // FILE_HEAD, выводим имя файла
            const FILE_HEAD *fileHeader = (FILE_HEAD*) &dataBuffer[fileOffset + sizeof(RarVolumeHeaderFormat)];

            // Имя файла следует сразу после заголовка FILE_HEAD
            string fileName((char*) &dataBuffer[fileOffset + sizeof(FILE_HEAD)+sizeof(RarVolumeHeaderFormat)], fileHeader->NameSize);

            // Выводим имя файла
            cout << "Имя файла: " << fileName << endl;
            numberOfFiles++;

            // Запись состоит из заголовка и упакованных данных
            fileOffset += volumeHeader->header_size;
            fileOffset += fileHeader->PackSize;
        }
        else if (volumeHeader->header_type == 0x7B)
        {
            // Штатное завершение архива
            break;
        }
        else
        {
            // Другие виды заголовков не обрабатываются и инициируют завершение обработки
            break;
        }
    }

    return numberOfFiles;
}

int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    // Открыть файл
    HANDLE fileHandle = CreateFileA("Example.rar", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    // Проверка успешного открытия файла
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        cout << "Ошибка открытия файла" << endl;
        return 1;
    }

    // Получить размер файла
    DWORD fileSize = GetFileSize(fileHandle, NULL);
    if (fileSize == INVALID_FILE_SIZE)
    {
        cout << "Ошибка определения размера файла" << endl;
        CloseHandle(fileHandle);
        return 1;
    }

    vector<BYTE> dataBuffer(fileSize);
    DWORD bytesRead;
    bool readOk = ReadFile(fileHandle, dataBuffer.data(), fileSize, &bytesRead, NULL);

    if (readOk && bytesRead == fileSize)
    {
     // Файл успешно считан
    int numberOfFiles = Rar4ListContents(dataBuffer.data(), dataBuffer.size());
    cout << "Всего файлов в архиве: " << numberOfFiles << endl;
    }


    CloseHandle(fileHandle);
    return 0;
}
