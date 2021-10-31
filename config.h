#include <stdio.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <dirent.h>


#include <tgbot/tgbot.h>

#define LENGTH(x) sizeof(x) / (sizeof(x[0]))
#define ENV_CHAR_VARIABLE_LEGTH 16
#define SIZE_ENV_CHAR_VARIABLE sizeof(char[16])
#define SIZE_FILE_NAME sizeof(char[256])
#define CONTENT_BUFFER_SIZE 1024*1024*4


struct directory_files{
  char * _p;
  int _count; 
};

int create_environment_variable(char * _name, char * _val);
void send_message_to_chat(unsigned long long int _chat_id, char * env_var_name, TgBot::Bot *b, char * _file_path, int _flag);
directory_files * list_files_in_directory(char * _dir_path );
long long int get_file_last_modified_time(char *_file_path);
char *get_file_name(char *_file_name_w_extension);
unsigned long long int get_folder_name( char * _path, int _size);
