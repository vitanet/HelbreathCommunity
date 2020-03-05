//////////////////////////////////////////////////////////////////////
//
// Sql.cpp: implementation of the Party class.
//
//////////////////////////////////////////////////////////////////////

#include "Sql.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSql::CSql()
{

	/*
	.text:00401C7D                 push    16h             ; size_t
	.text:00401C7F                 push    0               ; int
	.text:00401C81                 mov     eax, [ebp+var_4]
	.text:00401C84                 add     eax, 20h
	.text:00401C87                 push    eax             ; void *
	.text:00401C88                 call    _memset
	.text:00401C8D                 add     esp, 0Ch
	.text:00401C90                 push    16h             ; size_t
	.text:00401C92                 push    0               ; int
	.text:00401C94                 mov     ecx, [ebp+var_4]
	.text:00401C97                 add     ecx, 4
	.text:00401C9A                 push    ecx             ; void *
	.text:00401C9B                 call    _memset
	.text:00401CA0                 add     esp, 0Ch
	.text:00401CA3                 push    16h             ; size_t
	.text:00401CA5                 push    0               ; int
	.text:00401CA7                 mov     edx, [ebp+var_4]
	.text:00401CAA                 add     edx, 11Ch
	.text:00401CB0                 push    edx             ; void *
	.text:00401CB1                 call    _memset
	.text:00401CB6                 add     esp, 0Ch
	.text:00401CB9                 push    16h             ; size_t
	.text:00401CBB                 push    0               ; int
	.text:00401CBD                 mov     eax, [ebp+var_4]
	.text:00401CC0                 add     eax, 138h
	.text:00401CC5                 push    eax             ; void *
	.text:00401CC6                 call    _memset
	.text:00401CCB                 add     esp, 0Ch
	.text:00401CCE                 push    8Ch             ; size_t
	.text:00401CD3                 push    0               ; int
	.text:00401CD5                 mov     ecx, [ebp+var_4]
	.text:00401CD8                 add     ecx, 164h
	.text:00401CDE                 push    ecx             ; void *
	.text:00401CDF                 call    _memset
	.text:00401CE4                 add     esp, 0Ch
	.text:00401CE7                 push    16h             ; size_t
	.text:00401CE9                 push    0               ; int
	.text:00401CEB                 mov     edx, [ebp+var_4]
	.text:00401CEE                 add     edx, 20Ch
	.text:00401CF4                 push    edx             ; void *
	.text:00401CF5                 call    _memset
	.text:00401CFA                 add     esp, 0Ch
	.text:00401CFD                 push    0CAh            ; size_t
	.text:00401D02                 push    0               ; int
	.text:00401D04                 mov     eax, [ebp+var_4]
	.text:00401D07                 add     eax, 378h
	.text:00401D0C                 push    eax             ; void *
	.text:00401D0D                 call    _memset
	.text:00401D12                 add     esp, 0Ch
	.text:00401D15                 push    2Ah             ; size_t
	.text:00401D17                 push    0               ; int
	.text:00401D19                 mov     ecx, [ebp+var_4]
	.text:00401D1C                 add     ecx, 250h
	.text:00401D22                 push    ecx             ; void *
	.text:00401D23                 call    _memset
	.text:00401D28                 add     esp, 0Ch
	.text:00401D2B                 mov     edx, [ebp+var_4]
	.text:00401D2E                 mov     word ptr [edx+54h], 0
	.text:00401D34                 mov     eax, [ebp+var_4]
	.text:00401D37                 mov     word ptr [eax+4Ch], 0
	.text:00401D3D                 mov     ecx, [ebp+var_4]
	.text:00401D40                 mov     word ptr [ecx+44h], 0
	.text:00401D46                 mov     edx, [ebp+var_4]
	.text:00401D49                 mov     word ptr [edx+3Ch], 0
	.text:00401D4F                 mov     eax, [ebp+var_4]
	.text:00401D52                 mov     dword ptr [eax+94h], 0
	.text:00401D5C                 mov     ecx, [ebp+var_4]
	.text:00401D5F                 mov     word ptr [ecx+8Ch], 0
	.text:00401D68                 mov     edx, [ebp+var_4]
	.text:00401D6B                 mov     word ptr [edx+84h], 0
	.text:00401D74                 mov     eax, [ebp+var_4]
	.text:00401D77                 mov     word ptr [eax+7Ch], 0
	.text:00401D7D                 mov     ecx, [ebp+var_4]
	.text:00401D80                 mov     word ptr [ecx+74h], 0
	.text:00401D86                 mov     edx, [ebp+var_4]
	.text:00401D89                 mov     word ptr [edx+6Ch], 0
	.text:00401D8F                 mov     eax, [ebp+var_4]
	.text:00401D92                 mov     word ptr [eax+64h], 0
	.text:00401D98                 mov     ecx, [ebp+var_4]
	.text:00401D9B                 mov     word ptr [ecx+5Ch], 0
	.text:00401DA1                 mov     edx, [ebp+var_4]
	.text:00401DA4                 mov     word ptr [edx+0C4h], 0
	.text:00401DAD                 mov     eax, [ebp+var_4]
	.text:00401DB0                 mov     word ptr [eax+0BCh], 0
	.text:00401DB9                 mov     ecx, [ebp+var_4]
	.text:00401DBC                 mov     word ptr [ecx+0B4h], 0
	.text:00401DC5                 mov     edx, [ebp+var_4]
	.text:00401DC8                 mov     word ptr [edx+0ACh], 0
	.text:00401DD1                 mov     eax, [ebp+var_4]
	.text:00401DD4                 mov     word ptr [eax+0A4h], 0
	.text:00401DDD                 mov     ecx, [ebp+var_4]
	.text:00401DE0                 mov     word ptr [ecx+9Ch], 0
	.text:00401DE9                 mov     edx, [ebp+var_4]
	.text:00401DEC                 mov     dword ptr [edx+330h], 0
	.text:00401DF6                 mov     eax, [ebp+var_4]
	.text:00401DF9                 mov     dword ptr [eax+328h], 0
	.text:00401E03                 mov     ecx, [ebp+var_4]
	.text:00401E06                 mov     dword ptr [ecx+320h], 0
	.text:00401E10                 mov     edx, [ebp+var_4]
	.text:00401E13                 mov     dword ptr [edx+318h], 0
	.text:00401E1D                 mov     eax, [ebp+var_4]
	.text:00401E20                 mov     dword ptr [eax+310h], 0
	.text:00401E2A                 mov     ecx, [ebp+var_4]
	.text:00401E2D                 mov     dword ptr [ecx+308h], 0
	.text:00401E37                 mov     edx, [ebp+var_4]
	.text:00401E3A                 mov     dword ptr [edx+300h], 0
	.text:00401E44                 mov     eax, [ebp+var_4]
	.text:00401E47                 mov     word ptr [eax+348h], 0
	.text:00401E50                 mov     ecx, [ebp+var_4]
	.text:00401E53                 mov     word ptr [ecx+340h], 0
	.text:00401E5C                 mov     edx, [ebp+var_4]
	.text:00401E5F                 mov     word ptr [edx+338h], 0
	.text:00401E68                 mov     eax, [ebp+var_4]
	.text:00401E6B                 mov     dword ptr [eax+368h], 0
	.text:00401E75                 mov     ecx, [ebp+var_4]
	.text:00401E78                 mov     dword ptr [ecx+360h], 0
	.text:00401E82                 mov     edx, [ebp+var_4]
	.text:00401E85                 mov     dword ptr [edx+358h], 0
	.text:00401E8F                 mov     eax, [ebp+var_4]
	.text:00401E92                 mov     dword ptr [eax+350h], 0
	.text:00401E9C                 mov     ecx, [ebp+var_4]
	.text:00401E9F                 mov     word ptr [ecx+15Ch], 0
	.text:00401EA8                 mov     edx, [ebp+var_4]
	.text:00401EAB                 mov     word ptr [edx+154h], 0
	.text:00401EB4                 mov     eax, [ebp+var_4]
	.text:00401EB7                 mov     dword ptr [eax+204h], 0
	.text:00401EC1                 mov     ecx, [ebp+var_4]
	.text:00401EC4                 mov     dword ptr [ecx+1FCh], 0
	.text:00401ECE                 mov     edx, [ebp+var_4]
	.text:00401ED1                 mov     word ptr [edx+1F4h], 0
	.text:00401EDA                 mov     eax, [ebp+var_4]
	.text:00401EDD                 mov     dword ptr [eax+228h], 0
	.text:00401EE7                 mov     ecx, [ebp+var_4]
	.text:00401EEA                 mov     word ptr [ecx+288h], 0FFFFh
	.text:00401EF3                 mov     edx, [ebp+var_4]
	.text:00401EF6                 mov     dword ptr [edx+280h], 0FFFFFFFFh
	.text:00401F00                 mov     eax, [ebp+var_4]
	.text:00401F03                 mov     word ptr [eax+2A0h], 0
	.text:00401F0C                 mov     ecx, [ebp+var_4]
	.text:00401F0F                 mov     word ptr [ecx+298h], 0
	.text:00401F18                 mov     edx, [ebp+var_4]
	.text:00401F1B                 mov     word ptr [edx+290h], 0
	.text:00401F24                 mov     eax, [ebp+var_4]
	.text:00401F27                 mov     word ptr [eax+2C8h], 0
	.text:00401F30                 mov     ecx, [ebp+var_4]
	.text:00401F33                 mov     word ptr [ecx+2C0h], 0
	.text:00401F3C                 mov     edx, [ebp+var_4]
	.text:00401F3F                 mov     word ptr [edx+2B8h], 0
	.text:00401F48                 mov     eax, [ebp+var_4]
	.text:00401F4B                 mov     word ptr [eax+2B0h], 0
	.text:00401F54                 mov     ecx, [ebp+var_4]
	.text:00401F57                 mov     word ptr [ecx+2A8h], 0
	.text:00401F60                 mov     edx, [ebp+var_4]
	.text:00401F63                 mov     dword ptr [edx+2D0h], 0
	.text:00401F6D                 mov     eax, [ebp+var_4]
	.text:00401F70                 mov     dword ptr [eax+2D8h], 0
	.text:00401F7A                 mov     ecx, [ebp+var_4]
	.text:00401F7D                 mov     dword ptr [ecx+2F8h], 0
	.text:00401F87                 mov     edx, [ebp+var_4]
	.text:00401F8A                 mov     dword ptr [edx+2F0h], 0
	.text:00401F94                 mov     eax, [ebp+var_4]
	.text:00401F97                 mov     dword ptr [eax+2E8h], 0
	.text:00401FA1                 mov     ecx, [ebp+var_4]
	.text:00401FA4                 mov     dword ptr [ecx+2E0h], 0
	.text:00401FAE                 mov     edx, [ebp+var_4]
	.text:00401FB1                 mov     dword ptr [edx+0CCh], 0
	.text:00401FBB                 mov     eax, [ebp+var_4]
	.text:00401FBE                 mov     dword ptr [eax+0F4h], 0
	.text:00401FC8                 mov     ecx, [ebp+var_4]
	.text:00401FCB                 mov     word ptr [ecx+0ECh], 0
	.text:00401FD4                 mov     edx, [ebp+var_4]
	.text:00401FD7                 mov     word ptr [edx+0E4h], 0
	.text:00401FE0                 mov     eax, [ebp+var_4]
	.text:00401FE3                 mov     word ptr [eax+0DCh], 0
	.text:00401FEC                 mov     ecx, [ebp+var_4]
	.text:00401FEF                 mov     word ptr [ecx+0D4h], 0
	.text:00401FF8                 mov     edx, [ebp+var_4]
	.text:00401FFB                 mov     word ptr [edx+114h], 0
	.text:00402004                 mov     eax, [ebp+var_4]
	.text:00402007                 mov     word ptr [eax+10Ch], 0
	.text:00402010                 mov     ecx, [ebp+var_4]
	.text:00402013                 mov     word ptr [ecx+104h], 0
	.text:0040201C                 mov     edx, [ebp+var_4]
	.text:0040201F                 mov     word ptr [edx+0FCh], 0
	.text:00402028                 mov     eax, [ebp+var_4]
	.text:0040202B                 mov     dword ptr [eax+448h], 0
	.text:00402035                 mov     ecx, [ebp+var_4]
	.text:00402038                 mov     dword ptr [ecx+450h], 0
	.text:00402042                 mov     edx, [ebp+var_4]
	.text:00402045                 mov     dword ptr [edx+458h], 0
	*/
}

CSql::~CSql()
{
/*
	for (i = 0; i < 22; i++) {
		20h[i] = 0;;
	}

	for (i = 0; i < 22; i++) {
		4h[i] = 0;;
	}

	for (i = 0; i < 22; i++) {
		11Ch[i] = 0;;
	}

	for (i = 0; i < 22; i++) {
		138h[i] = 0;;
	}

	for (i = 0; i < 140; i++) {
		164h[i] = 0;;
	}

	for (i = 0; i < 22; i++) {
		20Ch[i] = 0;;
	}
	
	for (i = 0; i < 42; i++) {
		250h[i] = 0;;
	}

*/

}

void CSql::CreateNewCharacter()
{
	/*
	.text:004023A0                 mov     word ptr [eax+0Ch], 0
	.text:004023A6                 mov     ecx, [ebp+var_4]
	.text:004023A9                 mov     word ptr [ecx+1Ch], 0
	.text:004023AF                 mov     edx, [ebp+var_4]
	.text:004023B2                 mov     dword ptr [edx+14h], 0
	.text:004023B9                 mov     eax, [ebp+var_4]
	.text:004023BC                 mov     word ptr [eax+34h], 0
	.text:004023C2                 mov     ecx, [ebp+var_4]
	.text:004023C5                 mov     word ptr [ecx+2Ch], 0
	.text:004023CB                 mov     edx, [ebp+var_4]
	.text:004023CE                 mov     word ptr [edx+24h], 0
	.text:004023D4                 mov     eax, [ebp+var_4]
	.text:004023D7                 mov     word ptr [eax+3Ch], 0
	.text:004023DD                 mov     ecx, [ebp+var_4]
	.text:004023E0                 mov     word ptr [ecx+54h], 0
	.text:004023E6                 mov     edx, [ebp+var_4]
	.text:004023E9                 mov     word ptr [edx+4Ch], 0
	.text:004023EF                 mov     eax, [ebp+var_4]
	.text:004023F2                 mov     word ptr [eax+44h], 0
	.text:004023F8                 mov     ecx, [ebp+var_4]
	.text:004023FB                 mov     dword ptr [ecx+5Ch], 0
	.text:00402402                 mov     edx, [ebp+var_4]
	.text:00402405                 mov     dword ptr [edx+64h], 0
	.text:0040240C                 mov     eax, [ebp+var_4]
	.text:0040240F                 mov     dword ptr [eax+6Ch], 0
	.text:00402416                 mov     ecx, [ebp+var_4]
	.text:00402419                 mov     word ptr [ecx+74h], 30
	.text:0040241F                 mov     edx, [ebp+var_4]
	.text:00402422                 mov     word ptr [edx+7Ch], 40
	*/
}

void CSql::Bank()
{
	/*
	.text:00402480                 mov     word ptr [eax+0Ch], 0
	.text:00402486                 mov     ecx, [ebp+var_4]
	.text:00402489                 mov     word ptr [ecx+24h], 0
	.text:0040248F                 mov     edx, [ebp+var_4]
	.text:00402492                 mov     dword ptr [edx+1Ch], 0
	.text:00402499                 mov     eax, [ebp+var_4]
	.text:0040249C                 mov     word ptr [eax+3Ch], 0
	.text:004024A2                 mov     ecx, [ebp+var_4]
	.text:004024A5                 mov     word ptr [ecx+34h], 0
	.text:004024AB                 mov     edx, [ebp+var_4]
	.text:004024AE                 mov     word ptr [edx+2Ch], 0
	.text:004024B4                 mov     eax, [ebp+var_4]
	.text:004024B7                 mov     word ptr [eax+44h], 0
	.text:004024BD                 mov     ecx, [ebp+var_4]
	.text:004024C0                 mov     word ptr [ecx+5Ch], 0
	.text:004024C6                 mov     edx, [ebp+var_4]
	.text:004024C9                 mov     word ptr [edx+54h], 0
	.text:004024CF                 mov     eax, [ebp+var_4]
	.text:004024D2                 mov     word ptr [eax+4Ch], 0
	.text:004024D8                 mov     ecx, [ebp+var_4]
	.text:004024DB                 mov     dword ptr [ecx+64h], 0
	.text:004024E2                 mov     edx, [ebp+var_4]
	.text:004024E5                 mov     dword ptr [edx+6Ch], 0
	*/
}

void CSql::CharacterInfo()
{

/*
	.text:00402540                 mov     dword ptr [eax+1Ch], 0
	.text:00402547                 mov     ecx, [ebp+var_4]
	.text:0040254A                 mov     word ptr [ecx+14h], 0
	.text:00402550                 mov     edx, [ebp+var_4]
	.text:00402553                 mov     word ptr [edx+0Ch], 0
*/


}

void CSql::Guild()
{

/*
	.text:0040258D                 push    2Ah             ; size_t
	.text:0040258F                 push    0               ; int
	.text:00402591                 mov     eax, [ebp+var_4]
	.text:00402594                 add     eax, 4
	.text:00402597                 push    eax             ; void *
	.text:00402598                 call    _memset
	.text:0040259D                 add     esp, 0Ch
	.text:004025A0                 push    16h             ; size_t
	.text:004025A2                 push    0               ; int
	.text:004025A4                 mov     ecx, [ebp+var_4]
	.text:004025A7                 add     ecx, 3Ch
	.text:004025AA                 push    ecx             ; void *
	.text:004025AB                 call    _memset
	.text:004025B0                 add     esp, 0Ch
	.text:004025B3                 push    16h             ; size_t
	.text:004025B5                 push    0               ; int
	.text:004025B7                 mov     edx, [ebp+var_4]
	.text:004025BA                 add     edx, 58h
	.text:004025BD                 push    edx             ; void *
	.text:004025BE                 call    _memset
	.text:004025C3                 add     esp, 0Ch
	.text:004025C6                 mov     eax, [ebp+var_4]
	.text:004025C9                 mov     dword ptr [eax+34h], 0
	.text:004025D0                 mov     ecx, [ebp+var_4]
	.text:004025D3                 mov     word ptr [ecx+74h], 0

*/

}