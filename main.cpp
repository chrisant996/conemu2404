// Copyright (c) 2021 Christopher Antos
// License: http://opensource.org/licenses/MIT

#include "windows.h"
#include "stdio.h"

//------------------------------------------------------------------------------
struct DATAPOINT
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    const char* str;
};

//------------------------------------------------------------------------------
static void out(HANDLE h, const char* str, DATAPOINT** p)
{
    DWORD didWrite;
    WriteConsoleA(h, str, DWORD(strlen(str)), &didWrite, nullptr);

    if (p)
    {
        GetConsoleScreenBufferInfo(h, &(*p)->csbi);
        if (str[0] == '\r' && !str[1])
            str = "\\r";
        (*p)->str = str;
        (*p)++;
    }
}

//------------------------------------------------------------------------------
static void out(HANDLE h, const char* str, DATAPOINT*& p)
{
    out(h, str, &p);
}

//------------------------------------------------------------------------------
static void out(HANDLE h, const char* str)
{
    out(h, str, nullptr);
}

//------------------------------------------------------------------------------
void test_wrap(bool vtp)
{
    const HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD old_mode = 0;
    DWORD new_mode = 0;
    GetConsoleMode(h, &old_mode);
    if (vtp)
        new_mode = old_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    else
        new_mode = old_mode & ~ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(h, new_mode);

    printf("console mode = 0x%x\n", new_mode);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(h, &csbi))
        printf("csbi.dwSize = %d,%d\n", csbi.dwSize.X, csbi.dwSize.Y);

    printf("\n");

    DATAPOINT data[20];
    DATAPOINT* p = data;

    // out(h, "\x1b[?12l\x1b[?25h");
    // out(h, "\x1b[1;32;40mD:\\Programming\\Web\\direct_sales_platform\\@frontend \x1b[1;37;40m(develop -> origin)\r\n");
    // out(h, "\x1b[1;39;40mλ \x1b[0m\x1b[m");
    // out(h, "\x1b[1;39;40mp \x1b[0m\x1b[m", p);
    // out(h, "p ", p);

    // out(h, "\x1b[m\\\x1b[m");
    // out(h, "\x08");
    // out(h, "\x1b[K");
    // out(h, "\x1bg");

    while (csbi.dwCursorPosition.X < csbi.dwSize.X - 3)
    {
        // out(h, "\x1b[ma\x1b[m");
        out(h, "a");
        GetConsoleScreenBufferInfo(h, &csbi);
    }

    for (size_t i = 0; i < 5; i++)
    {
        if (i == 2)
            MessageBoxA(0, "break into debugger", "Repro", MB_OK);
        // out(h, "\x1b[mb\x1b[m");
        out(h, "b", p);
    }

    out(h, " ", p);
    out(h, "\r", p);

    for (size_t i = 3; i--;)
    {
        // out(h, "\x1b[mc\x1b[m");
        out(h, "c", p);
    }

    out(h, "\r\n\r\n");

    for (DATAPOINT* walk = data; walk < p; walk++)
        printf("#%d:\t\b\bdwCursorPosition = %3d,%3d;  str = \"%s\"\n", int(walk - data), walk->csbi.dwCursorPosition.X, walk->csbi.dwCursorPosition.Y, walk->str);

    SetConsoleMode(h, old_mode);
}

//------------------------------------------------------------------------------
int main(int argc, char** argv)
{
    if (argc <= 1)
    {
usage:
        puts("This program is for investigating ConEmu #2404:\n"
             "https://github.com/Maximus5/ConEmu/issues/2404\n"
             "\n"
             "Run 'repro_x64 --vtp' to repro issue; sets ENABLE_VIRTUAL_TERMINAL_PROCESSING.\n"
             "Run 'repro_x64 --novtp' to not repro; omits ENABLE_VIRTUAL_TERMINAL_PROCESSING.\n");
        return 1;
    }

    bool vtp = true;
    if (strcmp(argv[1], "--vtp") == 0)
        vtp = true;
    else if (strcmp(argv[1], "--novtp") == 0)
        vtp = false;
    else
        goto usage;

    test_wrap(vtp);
    return 0;
}
