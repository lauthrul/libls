perl Configure VC-WIN32 --prefix=.\bin\static
ms\do_nasm
vcvars32.bat
nmake -f ms\nt.mak
nmake -f ms\nt.mak test
nmake -f ms\nt.mak install