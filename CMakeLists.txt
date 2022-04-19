cmake_minimum_required(VERSION 3.20)
project(ft_containers)

set(CMAKE_CXX_STANDARD 98)

set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -std=c++98")

include_directories(.)
include_directories(./srcs/config)
include_directories(./srcs/logger)
include_directories(./srcs/server)

add_executable(webserv
        srcs/config/Config.cpp
        srcs/config/IConfig.hpp
        srcs/config/Config.hpp
        srcs/logger/ILogger.hpp
        srcs/logger/Logger.cpp
        srcs/logger/Logger.hpp
        srcs/logger/Options.cpp
        srcs/logger/Options.hpp
        srcs/server/IServer.hpp
        srcs/server/Options.hpp
        srcs/server/Options.cpp
        srcs/server/Server.cpp
        srcs/server/Server.hpp
        srcs/main.cpp
        )
