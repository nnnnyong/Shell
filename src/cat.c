/* my_cat.c 구현 */

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

void catFile(const char *filename){
	FILE *file;
	char buffer[BUFFER_SIZE];
	size_t bytesRead;

	file = fopen(filename, "r");
	if (file == NULL){
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}

	while ((bytesRead = fread(buffer,1,sizeof(buffer),file)) > 0 ){
		fwrite(buffer,1,bytesRead,stdout);
	}

	fclose(file);

}

int main(int argc, char *argv[]){
	if (argc < 2){
		fprintf(stderr,"Usage: %s <file1> [file2] [file3] ...\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	for (int i = 1; i < argc; i++){
		catFile(argv[i]);
	}

	return 0;
}

