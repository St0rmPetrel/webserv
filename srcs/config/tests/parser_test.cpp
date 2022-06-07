#include <iostream>
#include "../Config.hpp"

// todo: change path vars
const std::string valid = "../../configs/basic.conf";
const std::string valid_with_comments = "../../configs/basic_with_comments.conf";
const std::string empty = "../../configs/empty.conf";
const std::string invalid = "../../configs/invalid.conf";
const std::string invalid2 = "../../configs/invalid2.conf";
const std::string invalid3 = "../../configs/invalid3.conf";
const std::string invalid4 = "../../configs/no_rights.conf";



int main(void)
{
    logger::Options opt;
    //	opt.enabled_level = logger::INFO;
    opt.level = logger::DEBUG;
    logger::Logger log;
    log.set_up(opt);

    {
        /// completely valid testcase
        try {
            config::Config config(log);
            config.parse(valid);

            std::ofstream out;
            out.open("../../configs/basic_result.txt", std::ofstream::out | std::ofstream::trunc);
            if (out.is_open()) {
                out << config.Print();
            }
        }
        catch(std::exception& e) {
            log.fatal(e.what());
        }
    }
    {
        /// completely valid testcase + comments in conf file
        try {
            config::Config config(log);
            config.parse(valid_with_comments);

            std::ofstream out;
            out.open("../../configs/basic_with_comments_result.txt", std::ofstream::out | std::ofstream::trunc);
            if (out.is_open()) {
                out << config.Print();
            }
        }
        catch(std::exception& e) {
            log.fatal(e.what());
        }
    }
    {
        /// ?
        try {
            config::Config config(log);
            config.parse(empty);

            std::ofstream out;
            out.open("../../configs/empty_result.txt", std::ofstream::out | std::ofstream::trunc);
            if (out.is_open()) {
                out << config.Print();
            }
        }
        catch(std::exception& e) {
            log.fatal(e.what());
        }
    }
    {
        /// expected exception - {
        try {
            config::Config config(log);
            config.parse(invalid);
            return (1);
        }
        catch(std::exception& e) {
            log.fatal(e.what());
        }
    }
    {
        /// expected exception - double ;
        try {
            config::Config config(log);
            config.parse(invalid2);
            return (1);
        }
        catch(std::exception& e) {
            log.fatal(e.what());
        }
    }
    {
        /// expected exception - ; following {
        try {
            config::Config config(log);
            config.parse(invalid3);
            return (1);
        }
        catch(std::exception& e) {
            log.fatal(e.what());
        }
    }
    {
        /// unable to open conf file
        try {
            config::Config config(log);
            config.parse(invalid4);
            return (1);
        }
        catch(std::exception& e) {
            log.fatal(e.what());
        }
    }

    return (0);
}
