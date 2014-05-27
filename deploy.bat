@ECHO off

CALL :deploy Debug
CALL :deploy Release
GOTO:eof

:deploy
SET Configuration=%~1
SET ConfigPath=bin\%~1
ECHO Deploying %Configuration%
:: Asset folders
MKLINK "%ConfigPath%\Models\" "assets\Models" /J
MKLINK "%ConfigPath%\Textures\" "assets\Textures\" /J
MKLINK "%ConfigPath%\Sounds\" "assets\Sounds\" /J
:: Shaders
MKLINK "%ConfigPath%\Shaders\" "src\Shaders\" /J
:: DLLs
MKLINK "%ConfigPath%\glfw3.dll" "libs\glfw-3.0.4\lib\%Configuration%\glfw3.dll" /H
MKLINK "%ConfigPath%\fmodex.dll" "libs\FMOD\lib\fmodex.dll" /H
IF %~1==Debug (
	MKLINK "%ConfigPath%\glew32d.dll" "libs\glew-1.10.0\bin\%Configuration%\Win32\glew32d.dll" /H
)
IF %~1==Release (
	MKLINK "%ConfigPath%\glew32.dll" "libs\glew-1.10.0\bin\%Configuration%\Win32\glew32.dll" /H
)
GOTO:eof