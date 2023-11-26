/* my_rm.c 구현 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	    
	if (argc < 2) {
   		fprintf(stderr, "Usage: %s <file1> [file2] [file3] ...\n", argv[0]);
 		exit(EXIT_FAILURE);
       }
   
        for (int i = 1; i < argc; i++) {
              if (remove(argv[i]) == 0) {
                   printf("Removed: %s\n", argv[i]);
             } else {
                    perror("Error removing file");
                    exit(EXIT_FAILURE);
             }
        }        
       
	return 0; 
}

