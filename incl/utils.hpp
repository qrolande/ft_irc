#ifndef UTILS_HPP
# define UTILS_HPP

void split(std::vector<std::string>& dest, const std::string& str);
void split(std::vector<std::string>& dest, int max_args);
int contains_new_line( char buffer[] );
void adam_sender(int fd, std::string message);
bool    is_nickname_valid( const std::string& nick );

#endif