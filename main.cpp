#include "config.h"
#include <time.h>
#include <sys/stat.h>

using namespace std;
using namespace TgBot;

int main(int argc, char **argv) {
  
  // argv[1] path to configuration directory
  // argv[2] verbouse 1- True; 0-False

  int _verbouse = (argv[2][0] != 48 ? 1: 0);

  
  // Pass "/.../.../.../chad_ID/" as argument to this program
  int PATH_LENGTH = strlen(argv[1]);

  char * CONFIG_ENV_VARIABLES_PATH = (char *) malloc(sizeof(char) * PATH_LENGTH);
  strcpy(CONFIG_ENV_VARIABLES_PATH, argv[1]);
  
  string token = "YOUR TOKEN HERE";
  Bot bot(token);

  unsigned long long int  _chat_id = get_folder_name(CONFIG_ENV_VARIABLES_PATH, PATH_LENGTH);


  // Get directory files
  directory_files * _files = list_files_in_directory(CONFIG_ENV_VARIABLES_PATH);

  for(int i = 0; i < _files->_count; i++){

    // Create and assign file path
    char * _file_path = (char *) malloc(sizeof(CONFIG_ENV_VARIABLES_PATH) + SIZE_FILE_NAME);
    strcpy(_file_path, CONFIG_ENV_VARIABLES_PATH);
    strcat(_file_path, "/");
    strcat(_file_path, _files->_p);

    // Get File ne NO extension
    char * file_name_no_extension = get_file_name(_files->_p);

    // Get File last modified
    long long int _last_modified = get_file_last_modified_time(_file_path);
    //printf("\nLast Modified: [%lld], Name: [%s]\n", _last_modified, file_name_no_extension);
    
    //Create environment variable with File Name
    char _last_modified_char[11] ;
    sprintf(_last_modified_char, "%lld", _last_modified);
    create_environment_variable(file_name_no_extension, _last_modified_char);

    //Fork process
    int _new_pid = fork();
    if(_new_pid < 0 ){
      printf("ERROR creating new procees \n");
    }
    else if(_new_pid == 0)
    {
      printf("\n----------NEW Child process ( %d )----------\n", getpid());
      printf("########################################################\n\n");
      fflush(stdout); // flush output of child process
      
      send_message_to_chat(_chat_id, file_name_no_extension, &bot, _file_path, _verbouse);
    }
    else{
      printf("\n\n########################################################");
      printf("\n----------Parent process    ( %d )----------\n", getpid());
    }
    
    _files->_p += SIZE_FILE_NAME;
  } 

  printf("\n\t( %d )Parent Process exiting\n", getpid());
  
  return 0;
}

//g++ main.cpp config.h config.cpp -o telegram_bot --std=c++11 -I/usr/local/include -lTgBot -lboost_system -lssl -lcrypto -lpthread
