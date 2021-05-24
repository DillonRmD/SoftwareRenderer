@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\vsdevcmd.bat" -no_logo

IF NOT EXIST build\ mkdir build\
pushd build\
set CommonCompilerFlags=-Fe:win32_platform.exe -diagnostics:column -nologo -WL -fp:fast -Gm- -GR- -EHa- -Od -FC -Zi -w
set CommonLinkerFlags=-incremental:no -opt:ref user32.lib gdi32.lib winmm.lib

del *.pdb > NUL 2> NUL
echo WAITING FOR PDB > lock.tmp

cl %CommonCompilerFlags% ..\code\*.cpp -link %CommonLinkerFlags%

del lock.tmp
del *.obj

popd