#include "config.h"
#include <boost/system/system_error.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>


using namespace std;
using namespace TgBot;

int create_environment_variable(char * _name, char * _val){

  int set_env = setenv(_name, _val, 1);
  if(set_env == 0){
    printf("Environment variable [%s] set !-successfully-! with value [%s]\n", _name, _val);
  }
  else{
    printf("Environment variable [%s] set !-UNsuccessfully-! with value [%s]\n", _name, _val);
  }

  fflush(stdout);
  
  return set_env;
}

void send_message_to_chat(unsigned long long int _chat_id, char * env_var_name, TgBot::Bot *b, char * _file_path, int _flag) {

  while(1){
    
    char * current_env_var_value = getenv(env_var_name);
    if(current_env_var_value == NULL){
      _flag ? printf("Culd not get environment variable [%s]'s value !\n", env_var_name) : printf("");
      break;
    }else{
      
      // Get Last Modified Timestamp [int]
      long long int _last_modified = get_file_last_modified_time(_file_path);
      _flag ? printf("Get Last Modified Timestamp [%ld]\n", _last_modified) : printf("");
      char * _last_modified_char = (char *) malloc(11 * sizeof(char));
      sprintf(_last_modified_char, "%lld", _last_modified);

      // If there was a modification -> send message
      if(strcmp(current_env_var_value, _last_modified_char) != 0){
      
	//Prepare File content buffer
	_flag ? printf("Prepare File content buffer\n") : printf("");
	char * _content_buffer = (char *) malloc(CONTENT_BUFFER_SIZE * sizeof(char)); 
	char tmp_char;
	int _content_position = 0;
	string _message; 
	
	//Open file for read
	_flag ? printf("Opening file\n") : printf("");
	  
	FILE * _fp = fopen(_file_path, "r");
	
	// Get File contents
	for(int i = 0; (i < CONTENT_BUFFER_SIZE); i++){
	  tmp_char = fgetc(_fp);

	  if(tmp_char != EOF){
	    _content_buffer[i] = tmp_char;
	  }
	  else{
	    _content_buffer[i] = 0x00;
	    break;
	  }
	  
	}
	_flag ? printf("---File contents---\n%s\n---File contents---\n", _content_buffer) : printf("");
	
	// Close the File
	_flag ? printf("Closing File\n") : printf("");
	fclose(_fp);
	
	// Append _content_buffer to _message
	_flag ? printf("Appending _content_buffer to _message\n") : printf("");
	_message.append(_content_buffer);
	
	//Send Message [catch exceptions]
	_flag ? printf("Sending Message\n") : printf("");
	try{
	  b->getApi().sendMessage(_chat_id, _message.c_str());
	}
	catch(boost::system::system_error &err){
	  boost::system::error_code ec =  err.code();
	  printf("Error message: %s\n", ec.message().c_str());
	}
	catch(TgException &err){
	  printf("Error message: %s\n", err.what());
	}

	// Assign new Modified Time to Environment Variable
	_flag ? printf("Resetting modified time variable\n") : printf("");
	setenv(env_var_name, _last_modified_char, 1);

	// Free up space
	_flag ? printf("Freeing up memory\n") : printf("");
	free(_content_buffer);
	free(_last_modified_char);
	
      }
      else{
	_flag ? printf("[ No changes to %s  since %s ]\n", _file_path, current_env_var_value) : printf("");
      }
    }

    fflush(stdout);
    sleep(5);
    
  }
  
}

directory_files * list_files_in_directory(char * _dir_path ){

  int _num_files = 1;
  char * _current_file = (char *) malloc( SIZE_FILE_NAME );
  char * _return_val = _current_file;
  
  char _dir_1[256]   = ".";
  char _dir_2[256]   = "..";

  const char * _p1 = _dir_1;
  const char * _p2 = _dir_2;

  struct dirent * _p_file;
  DIR * _directory_entry = opendir(_dir_path);
  
  while( (_p_file = readdir(_directory_entry)) != NULL ){
    if(
        (strcmp(_p_file->d_name, _p1) != 0)
	&&
	(strcmp(_p_file->d_name, _p2) != 0)
       )
    {
      // append to _return_val
      strcpy(_current_file, _p_file->d_name);

      //printf("[%d]\t%s\t[%u]\n",_num_files, _current_file, _current_file);

      _num_files++;
      _return_val  = (char *) realloc(_return_val, (SIZE_FILE_NAME * _num_files));

      int n = _num_files - 1;
      _current_file = _return_val + ( n * SIZE_FILE_NAME);   

    }
  }

  closedir(_directory_entry);


  directory_files * _files = (directory_files *) malloc(sizeof(directory_files));
  _files->_p   = _return_val;
  _files->_count = _num_files - 1;
  
  return  _files;
}

long long int get_file_last_modified_time(char *_file_path) {

  // Get File last modified
  struct stat _result;
  if (!stat( _file_path, &_result)){
    __time_t mod_time_s  = _result.st_mtim.tv_sec;
    return mod_time_s;
  }
  else{
    return -1;
  }
  
}

char * get_file_name(char * _file_name_w_extension){
  
  char * file_name_no_extension = (char *) malloc(SIZE_FILE_NAME);
  char * _char_p_name = _file_name_w_extension;
    
  for(int i  = 0; i < SIZE_FILE_NAME; i++){
    
    char _inp[10];
    if(* _char_p_name != '.'){
      file_name_no_extension[i] = _char_p_name[0];
    }
    else{
      file_name_no_extension[i] = '\0';
      break;
    }
    _char_p_name += sizeof(char);
  }

  return file_name_no_extension;
}

unsigned long long int get_folder_name( char * _path, int _size) {
  int count = 1;
  char * _last = _path + _size - 2;

  unsigned long long int _return = 0;

  do {
    int current_num = *_last - 48;
    _return += current_num * count;

    //printf("%llu \t %c \n", _return, *_last);
    
    _last -= 1;
    count *= 10;
  } while( *_last != 47 );
  
  return _return;
  
}
