@echo off

call getcomp.bat rosbe

set opts=-std=c11 -mconsole -Os -s -Wall -Wextra
set linkinc=-lwinmm
set compiles=main\faith_ringtone_tool_synth4.c
set compiles=%compiles% common\winmm_audio.c common\winmm_midi.c
set errlog=.\faith_synth4.log
set out=..\bin\faith_synth4.exe
call :comp

exit /B 0





:comp
del %out%
gcc -o %out% %compiles% %opts% %linkinc% 2> %errlog%
IF %ERRORLEVEL% NEQ 0 (
    echo oops!
    notepad %errlog%
    goto :end
)
for %%R in (%errlog%) do if %%~zR lss 1 del %errlog%
exit /B 0

:end