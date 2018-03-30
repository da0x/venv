//
//  main.cpp
//  venv
//
//  Created by Daher Alfawares on 11/27/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#include <iostream>
#include <sstream>
#include "x_options.hpp"
#include "x_shell.hpp"
#include "repository.hpp"

namespace arg {
    std::string help        = "-help";
    std::string init        = "-init";
    std::string deinit      = "-deinit";
    std::string list        = "-ls";
    std::string create      = "-create";
    std::string add         = "-add";
    std::string to          = "-to";
    std::string from        = "-from";
    std::string push        = "-push";
    std::string pull        = "-pull";
    std::string diff        = "-diff";
    std::string select      = "-select";
    std::string cmd         = "-cmd";
    std::string rename      = "-rename";
    std::string remove      = "-remove";
}

int main(int argc, const char * argv[]) try {
    using std::cout;
    using std::cerr;
    using std::endl;
    
    auto repo = v::repository();
    
    switch(argc){
    case 1:
    {
        auto venvs = repo.all_venvs();
        if(venvs.empty()){
            std::cout << "venv is empty" << std::endl;
        } else {
            cout << repo << std::endl;
        }
        break;
    }
    case 2:
        // implicit select..
        auto name = argv[1];
        if(repo.exists(name)){
            repo.current().push();
            repo.current(name).pull();
            return 0;
        }
        break;
    }
    
    auto arguments = x::options(argc,argv);
    arguments.map_to({
        {arg::help,       x::option("this help message")},
        {arg::init,       x::option("initializes the current directory for venv use")},
        {arg::list,       x::option("lists all files in a venv")},
        {arg::deinit,     x::option("removes all venv-related files")},
        {arg::create,     x::option("creates a new venv")},
        {arg::add,        x::option("adds a file to a venv")},
        {arg::push,       x::option("push all changes to the active venv")},
        {arg::to,         x::option("specifies a destination")},
        {arg::from,       x::option("used with -pull switch to provide a source venv")},
        {arg::pull,       x::option("pulls new copies and replaces working copy")},
        {arg::push,       x::option("pushes current changes to a venv")},
        {arg::diff,       x::option("shows diffs of a specific venv")},
        {arg::select,     x::option("switches the active venv. this command will push & pull current changes automatically")},
        {arg::cmd,        x::option("prints the executed command")},
        {arg::rename,     x::option("renames a venv. use with -to option")},
        {arg::remove,     x::option("removes a venv and all of its content")},
    });
    
    
    if(arguments[arg::cmd]){
        std::cout << argv[0] << std::endl;
    }

    if(arguments[arg::help]){
        cout << "venv 1.0 by Daher Alfawares" << std::endl;
        cout << arguments.print() << std::endl;
        return 0;
    }
    
    if(arguments[arg::deinit]){
        cout << "removing all venvs" << std::endl;
        ::system(x::shell::remove(v::root_folder).c_str());
    }
    
    if(arguments[arg::init]){
        cout << "initializing venv" << endl;
        ::system(x::shell::mkdir(v::root_folder).c_str());
    }
    
    if(arguments[arg::create]){
        auto venv_names = arguments[arg::create].values();
        if(venv_names.empty()){
            cerr << "usage: venv " << arg::create << " <name>" << std::endl;
            return -1;
        }
        for(auto name:venv_names){
            auto venv = v::venv(name);
            if(!repo.exists(venv.name)){
                repo.create(venv);
                cout << "creating venv [" << venv.name << "]" << endl;
                ::system(x::shell::mkdir(v::root_folder + "/" + venv.name).c_str());
            } else {
                cerr << "[" << venv.name << "]" << " venv already exists." << std::endl;
            }
        }
    }

    if(arguments[arg::add]){
        if(!arguments[arg::to]){
            cerr << "missing " << arg::to << " <venv> in command line" << std::endl;
            return -1;
        }

        auto file_names = arguments[arg::add].values();
        auto venv_names = arguments[arg::to].values();
        repo.assert_available(venv_names);
        for(auto name:venv_names){
            v::venv& venv = repo[name];
            for(auto file:file_names){
                auto item = v::item(file);
                venv.add(item);
                venv.push(item);
            }
        }
    }

    if(arguments[arg::list]){
        auto venv_names = arguments[arg::add].values();
        if(venv_names.empty()){
            cerr << "usage: venv " << arg::list << " venv_name" << std::endl;
            return -1;
        }
        repo.assert_available(venv_names);
        for(auto name:venv_names){
            auto venv = repo[name];
            std::cout << venv << std::endl;
        }
    }

    if(arguments[arg::pull]){
        auto venv_names = arguments[arg::pull].values();
        if(venv_names.empty()){
            cerr << "usage: venv " << arg::pull << " venv_name" << std::endl;
            return -1;
        }
        repo.assert_available(venv_names);
        for(auto name:venv_names){
            repo[name].pull();
        }
    }

    if(arguments[arg::push]){
        auto venv_names = arguments[arg::push].values();
        if(venv_names.empty()){
            cerr << "usage: venv " << arg::push << " venv_name" << std::endl;
            return -1;
        }
        repo.assert_available(venv_names);
        for(auto name:venv_names){
            repo[name].push();
        }
    }

    if(arguments[arg::diff]){
        auto venv = repo.current();
        for(auto item:venv.items){
            venv.diff(item);
        }
        
        return 0;
    }

    if(arguments[arg::rename]){
        auto names = arguments[arg::rename].values();
        if(names.size()!= 2){
            cerr << "usage: venv " << arg::rename << " <old name> <new name>" << std::endl;
            return -1;
        }
        repo.rename(names[0],names[1]);
        return 0;
    }
        
    if(arguments[arg::remove]){
        auto names = arguments[arg::remove].values();
        if(names.empty()){
            cerr << "usage: venv " << arg::remove << " <name> <name>" << std::endl;
            return -1;
        }
        for(auto name: names){
            repo.remove(name);
        }
        return 0;
    }

    if(arguments[arg::select]){
        auto name = arguments[arg::select].value();
        if(name.empty()){
            cerr << "usage: $ venv " << arg::select << " <venv name>" << std::endl;
            cerr << "usage: $ venv <venv name>" << std::endl;
            return -1;
        }
        repo.assert_available(name);
        repo.current().push();
        repo.current(name).pull();
        return 0;
    }
    
    return 0;
}
catch(...){}
        
