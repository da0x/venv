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
    std::string cmd         = "-cmd";
}

int main(int argc, const char * argv[]) try {
    using std::cout;
    using std::cerr;
    using std::endl;
    
    auto arguments = x::options(argc,argv);
    arguments.map_to({
        {arg::help,       x::option("this help message")},
        {arg::init,       x::option("initializes the current directory for venv use")},
        {arg::list,       x::option("lists all files in a venv")},
        {arg::deinit,     x::option("removes all venv-related files")},
        {arg::create,     x::option("creates a new venv")},
        {arg::add,        x::option("adds a file to a venv")},
        {arg::push,       x::option("push all changes to the active venv")},
        {arg::from,       x::option("used with -pull switch to provide a source venv")},
        {arg::pull,       x::option("pulls new copies and replaces working copy")},
        {arg::push,       x::option("pushes current changes to a venv")},
        {arg::diff,       x::option("shows diffs of a specific venv")},
        {arg::cmd,        x::option("prints the given command")},
    });
    
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
    
    auto repo = v::repository();
    
    if(arguments[arg::create]){
        auto venv = v::venv(arguments[arg::create].value());
        
        if(!repo.exists(venv.name)){
            repo.create(venv);
            cout << "creating venv [" << venv.name << "]" << endl;
            ::system(x::shell::mkdir(v::root_folder + "/" + venv.id).c_str());
        } else {
            cerr << "[" << venv.name << "]" << " venv already exists." << std::endl;
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
        auto venv_names = arguments[arg::diff].values();
        if(venv_names.empty()){
            cerr << "usage: venv " << arg::diff << " venv_name" << std::endl;
            return -1;
        }
        repo.assert_available(venv_names);
        for(auto name:venv_names){
            auto venv = repo[name];
            for(auto item:venv.items){
                venv.diff(item);
            }
        }
        return 0;
    }
        
        if(arguments[arg::cmd]){
            std::cout << argv[0] << std::endl;
        }
    
    return 0;
}
catch(...) {}
