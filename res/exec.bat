@echo off & color a & setlocal EnableDelayedExpansion
:: This file is written by runsisi@163.com @HUST copyright (C) 2011

echo *************** 该批处理用于在Windows下安装grub4dos ****************
echo * 运行该批处理需要两个参数：第一个参数为安装到的目标盘符，第二个参 *
echo * 数为install或uninstall，如：exec.bat d install                   *	
echo ********** 作者：runsisi@163.com @HUST copyright (C) 2011 **********

set /a argc=0
for %%i in (%*) do (
	set /a argc+=1
)

:: TODO: Validate the first parameter
if %argc% lss 2 (
	goto :exit3
)
if not %2 == install if not %2 == uninstall (
	goto :exit3
)

set drv=%1
set cmd=%2

set title="GRUB4WIN"

:: Set needed file name
set ldr=grldr
set ldr.mbr=grldr.mbr
set menu.lst=menu.lst

set backup_dir=RUNSISI.{0FC3E696-387C-4a04-8CDF-EF501BAE29AD}
set backup_boot_file=boot.{0FC3E696-387C-4a04-8CDF-EF501BAE29AD}
set backup_postfix=.{0FC3E696-387C-4a04-8CDF-EF501BAE29AD}

:: Determine system version
:: OsName=2000 XP 2003 Vista Win7
set VerNo=5.0 5.1 5.2 6.0 6.1

set /a index=0
for %%i in (%VerNo%) do (
	call :determine_ver %%i
	if !errorlevel! == 0 (
		:: Not supported system
		goto supported
	)
)
:: Unsupported OS
goto exit2

:: Supported OS
:supported

:: Set OS name
if %index% leq 2 (
	set OS=XP
) else (
	set OS=WIN7
)

:: Get system root drive
set root=%systemroot:~0,3%

if %cmd% == install (
	goto install
) else (
	goto uninstall
)

:install
:: Determine if needed files are exist, or we exit the installation
if not exist %ldr% set dummy=0
if not exist %ldr.mbr% set dummy=0
if not exist %menu.lst% set dummy=0
if defined dummy (
	goto exit1
)

:: Do not add more than two line comments under if statement!
:: Do not add comment at the last line of else statement!
:: Pay attention to variable expansion!!!
:: TODO: When to use !var! and when to use %var%?
:: Back up the original boot info
if %OS% == XP (
	:: Backup the files with the same name
	call :backup_file_xp %ldr% %ldr.mbr% %menu.lst%
	:: Backup boot.ini
	call :del_system_attrib %root%boot.ini
	copy /y %root%boot.ini %drv%:\%backup_dir%\%backup_boot_file%
	:: Edit boot.ini
	echo,>>%root%boot.ini
	echo,%root%grldr.mbr=%title%>>%root%boot.ini
	call :add_system_attrib %root%boot.ini
	:: Copy grub4dos files to destination drive root
	call :copy_grub_file_xp %ldr% %ldr.mbr% %menu.lst%
	:: %drv%:\%menu.lst% will have normal attribute
	call :add_system_attrib %root%%ldr% %root%%ldr.mbr%
) else (
	:: Backup the files with the same name
	call :backup_file_win7 %ldr% %ldr.mbr% %menu.lst%
	bcdedit /export %drv%:\%backup_dir%\%backup_boot_file%
	:: Add a new entry
	for /f "tokens=1,2,3" %%b in ('bcdedit /create /d %title% /application bootsector') do (
		:: If the third word is "The"
		if "%%b" == "The" (
			set id=%%d
		) else (
			set id=%%c
		)
	)
	bcdedit /set !id! device partition=%drv%:
	bcdedit /set !id! path \%ldr.mbr%
	bcdedit /displayorder !id! /addlast
	:: Copy grub4dos files to destination drive root
	call :copy_grub_file_win7 %ldr% %ldr.mbr% %menu.lst%
	:: %drv%:\%menu.lst% will have normal attribute
	call :add_system_attrib %drv%:\%ldr% %drv%:\%ldr.mbr%
)


:: Delete grub files
call :del_grub_file %ldr% %ldr.mbr% %menu.lst%

:: Installation finished, exit
goto exit0

:uninstall
:: Restore boot info
if %OS% == XP (
	:: Override previous boot.ini
	call :del_system_attrib %root%boot.ini
	move /y %drv%:\%backup_dir%\%backup_boot_file% %root%boot.ini
	call :add_system_attrib %boot%boot.ini
	:: Delete installed grub files
	call :del_installed_grub_file_xp %ldr% %ldr.mbr% %menu.lst%
	:: Restore backup files
	call :restore_file_xp %ldr% %ldr.mbr% %menu.lst%
) else (
	:: Import backuped bcd
	bcdedit /import %drv%:\%backup_dir%\%backup_boot_file%
	del %drv%:\%backup_dir%\%backup_boot_file%
	:: Delete installed grub files
	call :del_installed_grub_file_win7 %ldr% %ldr.mbr% %menu.lst%
	:: Restore backup files
	call :restore_file_win7 %ldr% %ldr.mbr% %menu.lst%
	:: Delete bcd log file
	call :del_boot_log
)

:: Uninstallation finished, exit
goto exit0

:: ***********************************************************************************************
:: Helper functions
:determine_ver
ver | find /i "%1" > nul && exit /b 0
set /a index+=1
exit /b 1
goto :eof

:: exist can find system attribute file, but del/copy/move can not, 
:: note that del/copy/move can find system attribute directory!
:: del has a switch to delete files with specific attribute
:: Pay attention to the comment position, do not add more than two 
:: line comments under if statement!
:backup_file_xp
for %%i in (%*) do (
	if exist %root%%%i (
		call :del_system_attrib %root%%%i
		move /y %root%%%i %drv%:\%backup_dir%\%%i%backup_postfix%
	)
)
goto :eof

:backup_file_win7
for %%i in (%*) do (
	if exist %drv%:\%%i (
		call :del_system_attrib %drv%:\%%i
		move /y %drv%:\%%i %drv%:\%backup_dir%\%%i%backup_postfix%
	)
)
goto :eof

:: Restore backuped files which has the the same name with grub files
:restore_file_xp
for %%i in (%*) do (
	set file=%drv%:\%backup_dir%\%%i%backup_postfix%
	if exist !file! (
		move /y !file! %root%%%i
	)
)
goto :eof

:restore_file_win7
for %%i in (%*) do (
	set file=%drv%:\%backup_dir%\%%i%backup_postfix%
	if exist !file! (
		move /y !file! %drv%:\%%i
	)
)
goto :eof

:: Copy grub files to target drive root
:copy_grub_file_xp
for %%i in (%*) do (
	copy /y %%i %root%
)
goto :eof

:copy_grub_file_win7
for %%i in (%*) do (
	copy /y %%i %drv%:\
)
goto :eof

:: Delete grub files in installed target drive root
:del_installed_grub_file_xp
for %%i in (%*) do (
	if exist %root%%%i (
		call :del_system_attrib %root%%%i
		del %root%%%i
	)
)
goto :eof

:del_installed_grub_file_win7
for %%i in (%*) do (
	if exist %drv%:\%%i (
		call :del_system_attrib %drv%:\%%i
		del %drv%:\%%i
	)
)
goto :eof

:: Delete grub files in backup directory
:del_grub_file 
for %%i in (%*) do (
	if exist %%i (
		del %%i
	)
)
goto :eof

:add_system_attrib
for %%i in (%*) do (
	attrib +r +h +s %%i > nul
)
goto :eof

:del_system_attrib
for %%i in (%*) do (
	attrib -h -s -r %%i > nul
)
goto :eof

:: For can not find system attribute files
:del_boot_log
for /f %%i in ('dir /ah /b') do (
	echo %%i | find /i "boot.{0FC3E696-387C-4a04-8CDF-EF501BAE29AD}.LOG"
	if !errorlevel! == 0 (
		call :del_system_attrib %%i
		del %%i
	)
)
goto :eof

:: ***********************************************************************************************

:: Set exit code
:: Success = 0, FileNotFound = 1, NotSupportedSystem = 2, InvalidParameter = 3
:exit0
exit 0
:exit1
exit 1
:exit2
exit 2
:exit3
exit 3