perl Configure VC-WIN32 --prefix=..\..\bin\dll
ms\do_nasm
vcvars32.bat
nmake -f ms\ntdll.mak
nmake -f ms\ntdll.mak test
nmake -f ms\ntdll.mak install