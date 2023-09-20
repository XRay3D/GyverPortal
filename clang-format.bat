set CLANG_FORMAT="C:\Qt\Tools\QtCreator\bin\clang\bin\clang-format.exe"
set CODE_STYLE=%CD%\code_style.txt

REM for %%P in (GGEasy GTE_Win plugins static_libs) do (
REM 	cd %%P 
	for /R %%S in (*.cpp *.c *.hpp *.h  *.ino) do (
	    %CLANG_FORMAT% -i --style="file:%CODE_STYLE%" %%S
	)
REM 	cd ..
REM )
REM pause
