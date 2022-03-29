global readPort
global writePort

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