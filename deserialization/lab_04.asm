.686P
.MODEL FLAT, STDCALL
includelib kernel32.lib
ExitProcess PROTO : DWORD
MessageBoxA PROTO : DWORD, : DWORD, : DWORD, :DWORD
.STACK 4096
.DATA
Titel DB "Title", 0
MB_OK EQU 0
HW DD ? 
var DWORD 42
.CODE
main PROC
INVOKE MessageBoxA, HW, OFFSET resultBuffer, OFFSET Titel, MB_OK
