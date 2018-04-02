//
//  x_shell.hpp
//  x
//
//  Created by Daher Alfawares on 12/13/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#ifndef x_shell_hpp
#define x_shell_hpp

#include <string>
#include <iostream>
#include <sstream>

namespace x {
    namespace shell {
        
        using std::string;
        
        inline string remove(string file){
            std::stringstream command;
            command
            << "rm "
            << "-f "
            << "-r "
            << "-v "
            << "\"" << file << "\" ";
            return command.str();
        }
        inline string mkdir(string dir){
            std::stringstream command;
            command
            << "mkdir "
            << "\"" << dir << "\" ";
            return command.str();
        }
        inline string copy(string src, string dst){
            std::stringstream command;
            command
            << "cp "    // copy
            << "-f "    // force
            << "-r "    // recursive
            << "-v "    // verbose
            << "\"" << src << "\" "
            << "\"" << dst << "\" ";
            
          //  std::cout << command.str() << std::endl;
            return command.str();
        }
        
        inline string diff(string src, string dst){
            std::stringstream command;
            command
            << "diff " // diff
            << "-q "   // quiet
            << "\"" << src << "\" "
            << "\"" << dst << "\" ";
          //  std::cout << command.str() << std::endl;
            return command.str();
        }
        inline string move(string src, string dst){
            std::stringstream command;
            command
            << "mv "
            << "-f "
            << "-v "
            << "\"" << src << "\" "
            << "\"" << dst << "\" ";
            return command.str();
           // std::cout << command.str() << std::endl;
        }
    }
}

#endif /* x_shell_hpp */
