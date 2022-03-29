#ifndef LEXER_HPP
#define LEXER_HPP

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include "Config.hpp"

#define BUFFER_SIZE 1024

class Lexer {
    public:
    Lexer();

    Lexer(Lexer const &obj);
    Lexer	&operator=(Lexer const &obj);

    virtual ~Lexer();
    std::vector<std::string>        tokenizer(std::string &line);
    std::vector<std::string>	    readFile(const char *filename);
};

#endif