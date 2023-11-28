/* my_ln.c 구현 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char cmd;
    char *src;
    char *target;

    if (argc < 3){
	    fprintf(stderr, "Usage:file_link [l,u,s] ...\n");
	    fprintf(stderr,"	file_link l[ink] src target\n");
	    fprintf(stderr,"  file_link u[nlink] filename\n");
	    fprintf(stderr," file_link s[ymlink] src target\n");
	    exit(1);
    }

    cmd = (char) *argv[1];
    if (cmd == 'I'){
	    if(argc < 4){
		    fprintf(stderr,"file_link I src target [link] \n");
		    exit(1);
	    }
	    src = argv[2];
	    target = argv[3];
	    if (link(src,target) < 0){
		    perror("link");
		    exit(1);
	    }
    }
    else if (cmd == 's'){
	    if(argc<4){
		    fprintf(stderr,"file_link I src target [link]\n");
		    exit(1);
	    }
	    src = argv[2];
	    target = argv[3];
	    if(symlink(src,target) < 0){
		    perror("symlink");
		    exit(1);
	    }
    }
    else if (cmd == 'u'){
	    src = argv[2];
	    if(unlink(src) < 0){
		    perror("unlink");
		    exit(1);
	    }
    }
    else{
	    fprintf(stderr,"Unknown command...\n");
    }
}

