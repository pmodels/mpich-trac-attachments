#include <stdio.h>
#include <windows.h>

HANDLE StdIn;

DWORD CALLBACK StdInRead(PVOID arg)
{
    DWORD ret;
    UCHAR empty;

    if(!ReadFile(StdIn, &empty, 1, &ret, NULL)) {
        printf("ReadFile failed with %d. Bytes read = %d.\n", GetLastError(), ret);
    } else {
        /*Printed on < Windows 6.2. Bytes read = 0.*/
        printf("ReadFile is doing fine. Bytes read = %d.\n", ret);
    }
    return 0;
}

int main(int argc, char **argv)
{
    HANDLE thread;

    StdIn = GetStdHandle(STD_INPUT_HANDLE);
    WaitForSingleObject(
        thread = CreateThread(NULL, 0, StdInRead, NULL, 0, NULL), 
        2000);
    CloseHandle(StdIn);
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);

    return 0;
}
