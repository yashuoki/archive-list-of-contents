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

int extractRarContents(const BYTE *dataBuffer, unsigned int bufferSize)
{
    int numberOfFiles = 0;
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    return numberOfFiles;
}

int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    // ������� ����
    HANDLE fileHandle = CreateFileA("Example.rar", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    // �������� ��������� �������� �����
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        cout << "������ �������� �����" << endl;
        return 1;
    }

    // �������� ������ �����
    DWORD fileSize = GetFileSize(fileHandle, NULL);
    if (fileSize == INVALID_FILE_SIZE)
    {
        cout << "������ ����������� ������� �����" << endl;
        CloseHandle(fileHandle);
        return 1;
    }

    vector<BYTE> dataBuffer(fileSize);
    DWORD bytesRead;
    bool readOk = ReadFile(fileHandle, dataBuffer.data(), fileSize, &bytesRead, NULL);

    if (readOk && bytesRead == fileSize)
    {
     // ���� ������� ������
    int numberOfFiles = extractRarContents(dataBuffer.data(), dataBuffer.size());
    cout << "����� ������ � ������: " << numberOfFiles << endl;
    }


    CloseHandle(fileHandle);
    return 0;
}
