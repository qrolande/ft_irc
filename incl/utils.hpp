#ifndef UTILS_HPP
# define UTILS_HPP

void Error(std::string msg);
void Error(std::string msg, Server sever);
void split(std::vector<std::string>& dest, const std::string& str);
void split(std::vector<std::string>& dest, int max_args);
std::vector<std::string> split(std::string &str, char sep);
int contains_new_line( char buffer[] );
void adam_sender(int fd, std::string message);
bool    is_nickname_valid( const std::string& nick );
bool    is_valid_keys(const std::string& keys, bool mode);



#endif