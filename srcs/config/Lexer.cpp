#include "Lexer.hpp"

Lexer::Lexer() {}

Lexer::~Lexer() {}

std::vector<std::string>	Lexer::tokenizer(const std::string &line)
{
    std::vector<std::string>	tokens;
    std::string to_line;
    std::string new_world = "";
    unsigned long i = 0;
    while (i < line.length())
    {
        char c = line[i];
        switch (c) {
            case ' ':
            case '\n':
            case '\t':
            case '\0':
            {
                if (new_world != "")
                {
                    tokens.push_back(new_world);
                    new_world = "";
                }
                break;
            }
            case '{':
            case '}':
            case ';': {
            if (new_world != "")
            {
                tokens.push_back(new_world);
                new_world = "";
            }
            tokens.push_back(line.substr(i, 1));
            break ; }
            default:
            new_world += line[i];
        }
        i++;
    }
    return (tokens);
}

std::vector<std::string>	    Lexer::readFile(const char *filename) {
    int							size = BUFFER_SIZE;
    char					    buf[size + 1];
    std::string					line;
    int							fd;
    std::vector<std::string>	tokens;
    int                         i;
    std::string::size_type      start_comment = 0;
    std::string::size_type      end_comment = 0;

    if ((fd = open(filename, O_RDONLY)) <= 0)
        throw config::Config::FileNotFoundException();
    std::memset(buf, '\0', size + 1);
    for (i = BUFFER_SIZE; i > 0; i = read(fd, buf, BUFFER_SIZE )) {
        buf[i] = '\0';
        line += buf;
    }
    if (i == -1)
        throw config::Config::ReadingConfigFileException();
    while (start_comment != std::string::npos)
    {
        start_comment = line.find_first_of('#');
        end_comment = line.find_first_of('\n', start_comment);
        if (start_comment != std::string::npos)
            line = line.erase(start_comment, end_comment - start_comment);
    }
    tokens = tokenizer(line);
    return tokens;
}