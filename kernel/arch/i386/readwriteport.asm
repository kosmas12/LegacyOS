global readPort
global writePort
global readPort32
global writePort32

; unsigned char readPort(unsigned short port)
readPort:
	mov edx, [esp + 4]
	in al, dx
	ret

; writePort(unsigned short port, unsigned char data)
writePort:
	mov   edx, [esp + 4]
	mov   al, [esp + 4 + 4]
	out   dx, al
	ret

; unsigned int readPort(unsigned short port)
readPort32:
	mov edx, [esp + 4]
	in eax, dx
	ret

; writePort32(unsigned short port, unsigned int data)
writePort32:
	mov   edx, [esp + 4]
	mov   eax, [esp + 4 + 4]
	out   dx, eax
	ret