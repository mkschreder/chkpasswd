

/* check_password.c

   Read a user name and password and check if they are valid.

   This program uses the shadow password file. Some UNIX implementations
   don't support this feature.
*/
/* Compile with -lcrypt */
#if ! defined(__sun)
#define _BSD_SOURCE     /* Get getpass() declaration from <unistd.h> */
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE   /* Get crypt() declaration from <unistd.h> */
#endif
#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <shadow.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>

//#include "tlpi_hdr.h"

int main(int argc, char *argv[]){
    char *username, *password, *encrypted, *p;
    struct passwd *pwd;
    struct spwd *spwd;

	if(argc != 3){
		fprintf(stderr, "Usage: chkpasswd <user> <password>\n"); 
		exit(EXIT_FAILURE); 
	}

	username = argv[1]; 
	password = argv[2]; 

	// retreive the user passwords
    pwd = getpwnam(username);
    if (pwd == NULL){
        fprintf(stderr, "couldn't get password record");
		exit(EXIT_FAILURE); 
	}

    spwd = getspnam(username);
    if (spwd == NULL && errno == EACCES){
        fprintf(stderr, "Permission Denied!\n");
		exit(EXIT_FAILURE); 
	}

    if (spwd != NULL)
        pwd->pw_passwd = spwd->sp_pwdp;     /* Use the shadow password */
	
	// check the password against the one in the password files
    encrypted = crypt(password, pwd->pw_passwd);
	
    if (encrypted == NULL){
        perror("crypt");
		exit(EXIT_FAILURE); 
	}

    if(strcmp(encrypted, pwd->pw_passwd) == 0){
        fprintf(stderr, "Incorrect password\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

