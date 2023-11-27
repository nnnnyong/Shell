/* my_cp.c 구현 */

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE  4096

void copyFile(const char *sourcePath, const char *destPath){
	FILE *sourceFile, *destFile;
	char buffer[BUFFER_SIZE];
	size_t bytesRead;


	sourceFile = fopen(sourcePath, "rb");
	if(sourceFile == NULL){
		perror("Error opening source file");
		exit(EXIT_FAILURE);
	}

	destFile = fopen(destPath, "wb");
	if(destFile == NULL){
		perror("Error opening destination file");
		fclose(sourceFile);
		exit(EXIT_FAILURE);
	}

	while ((bytesRead = fread(buffer,1,sizeof(buffer),sourceFile)) > 0){
		fwrite(buffer,1,bytesRead,destFile);
	}

	fclose(sourceFile);
	fclose(destFile);

	printf("파일 복사 성공 : %s to %s\n", sourcePath, destPath);
}

int main(int argc, char *argv[]){ 
    if (argc != 3){
	    fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
	    exit(EXIT_FAILURE);
    }

    copyFile(argv[1], argv[2]);

    return 0;
}

