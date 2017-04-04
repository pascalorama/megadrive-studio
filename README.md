# Megadrive-Studio

Megadrive Studio , is a debugger for the Megadrive/Genesis System
it focus on SNASM68K, as it can load symbols from the .map outputed by that compiler. But it can load any megadrive game

The project is oooooooold , it was programmed in 2005, and is based on Genesis Plus from Charles McDonald.

you can :
- See disassembly code
- Step into code
- Step a frame
- Dump 68k, z80, vram, vsram, ram
- vdp registers calculator 

# How to compile
This is an really old program 2005, that means centuries in IT world.
Hopefuylly, i saved the libraries and I successfully recompile it using Visual Studio 2010. All the libraries are available

- DirectX SDK : http://pascalorama.com/mdstudiopre/dxsdk_jun2007.exe
- wxWindows : http://pascalorama.com/mdstudiopre/wxMSW-2.6.3-1.zip
- wxDockit : http://pascalorama.com/mdstudiopre/wxDockIt_2.1.zip

define a system variable (WXWIN) pointing to the wx folder
for example on my system : c:\sdk\wx

compile in this order : wxwindow , wxdockit , mdstudio

for wxwindows and wxdockit use the solution in theirs respective folder : build\msw