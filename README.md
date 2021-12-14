# Repro for [ConEmu#2404](https://github.com/Maximus5/ConEmu/issues/2404)

## Preparation; build the program:
1. Clone this repo.
2. Open the `conemu2404.sln` (or use `premake5` to create makefiles for another compiler).
3. Choose either "x64" or "Win32" platform, depending on whether you will use 64 bit or 32 bit ConEmu.
4. Press <kbd>F7</kbd> to compile the repro program.

## Launch and debug the repro:
5. Launch ConEmu.
6. Press <kbd>Ctrl</kbd>+<kbd>T</kbd> to open the New Console Dialog.
7. In the "Startup command or {Task} name" box, type the full path to the compiled repro program, and append a `--vtp` argument.
   - E.g. `c:\repos\conemu2404\.build\vs2019\bin\debug\repro_x64.exe --vtp`
8. Click the "Start" bottom at the bottom of the dialog.
9.  The program will print some initial diagnostic info, and then a string of "`a`" characters almost to the end of the line.
10. The program will print two "`b`" characters, and pop up a message box saying to break into the debugger.
11. Attach the debugger.
12. Set whatever breakpoints are needed for investigation, and then click the "OK" button in the message box to continue.
    - E.g. a breakpoint on `ExtWriteText` will enable stepping through the ConEmu code.
13. The next character printed is the 3rd "`b`", which should be on the same line as the preceding text.  The cursor position should not change.
14. The next character is the 4th "`b`", which is the first point at which an observable symptom appears.

## Expected result:
- The 4th "`b`" should wrap and appear on the first column of the next line.
- The rest of the text should appear on the second line:  another "`b`", then a space, then a carriage return which resets to column 1, then 3 "`c`" characters.

If the terminal is 40 characters wide, it should end up looking like this:
```
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbb
ccc
```

## Actual result:
- The 4th and 5th "`b`" characters and the space are all printed in the rightmost column of the same line, without wrapping.
- Then the carriage return resets to column 1 -- but since the text did not wrap correctly, the cursor stays on the same line.
- Then the 3 "`c`" characters are printed, and they overwrite the first 3 "`a`" characters of the line.

If the terminal is 40 characters wide, it mistakenly ends up looking like this:
```
cccaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbb
```
