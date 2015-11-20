all: chkpasswd

chkpasswd: chkpassword.c
	$(CC) -o chkpasswd chkpassword.c -lcrypt 

clean: 
	rm -f chkpasswd
