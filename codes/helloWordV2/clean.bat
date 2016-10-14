SET mypath=%~dp0
SET mypath=%mypath:~0,-1%
set mypathList=%mypath:\=,%

for %%i in (%mypathList%) do (
   SET projectDirName=%%i
)
IF  EXIST "%~dp0%projectDirName%.sln"  del "%~dp0%projectDirName%.sln"
IF  EXIST  "%~dp0ALL_BUILD.vcxproj" del "%~dp0ALL_BUILD.vcxproj"
IF  EXIST  "%~dp0%projectDirName%.vcxproj" del "%~dp0%projectDirName%.vcxproj"
IF  EXIST  "%~dp0%projectDirName%.vcxproj.filters" del "%~dp0%projectDirName%.vcxproj.filters"
IF  EXIST  "%~dp0ALL_BUILD.vcxproj.filters" del "%~dp0ALL_BUILD.vcxproj.filters"
IF  EXIST  "%~dp0Makefile" del "%~dp0Makefile"
IF  EXIST  "%~dp0CMakeCache.txt" del "%~dp0CMakeCache.txt"
IF  EXIST  "%~dp0.vs" del "%~dp0.vs" /s /q
IF  EXIST  "%~dp0cmake_install.cmake" del "%~dp0cmake_install.cmake"
IF  EXIST  "%~dp0CMakeFiles" rd "%~dp0CMakeFiles" /s /q
IF  EXIST  "%~dp0Debug" rd "%~dp0Debug" /s /q
IF  EXIST  "%~dp0Win32" rd "%~dp0Win32" /s /q
IF  EXIST  "%~dp0%projectDirName%.dir" rd "%~dp0%projectDirName%.dir" /s /q
IF  EXIST  "%~dp0bin" rd "%~dp0bin" /s /q
IF  EXIST  "%~dp0Build" rd "%~dp0Build" /s /q
IF  EXIST  "%~dp0build" rd "%~dp0build" /s /q
IF  EXIST  "%~dp0Release" rd "%~dp0Release" /s /q
IF  EXIST  "%~dp0%projectDirName%.VC.db" del "%~dp0%projectDirName%.VC.db" /s /q
IF  EXIST  "%~dp0%projectDirName%.xcodeproj" del "%~dp0%projectDirName%.xcodeproj" /s /q
IF  EXIST  "%~dp0CMakeScripts" del "%~dp0CMakeScripts" /s /q
IF  EXIST  "%~dp0RelWithDebInfo" del "%~dp0CRelWithDebInfo" /s /q





