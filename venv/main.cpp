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
    std::string select      = "-select";
    std::string add         = "-add";
    std::string to          = "-to";
    std::string push        = "-push";
    std::string pull        = "-pull";
}

int main(int argc, const char * argv[]) try {
    using std::cout;
    using std::cerr;
    using std::endl;
    
    auto arguments = x::options(argc,argv);
    arguments.map_to({
        {arg::help,       x::option("this help message.")},
        {arg::init,       x::option("initializes the current directory for venv use.")},
        {arg::list,       x::option("lists all files in a venv.")},
        {arg::deinit,     x::option("removes all items and venvs.")},
        {arg::select,     x::option("select the active venv. auto push/pull")},
        {arg::create,     x::option("creates a new venv.")},
        {arg::add,        x::option("adds a file to a venv \"-add file -to venv\".")},
        {arg::to,         x::option("used with -add switch to provide destination venv.")},
        {arg::pull,       x::option("pull new copies from the active venv.")},
        {arg::push,       x::option("push all changes to the active venv.")},
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
            cerr << "[" << venv.name << "]" << " venv exists." << std::endl;
        }
    }

    if(arguments[arg::add]){
        if(!arguments[arg::to]){
            cerr << "missing -to <venv> in command line" << std::endl;
            return -1;
        }
        auto file_names = arguments[arg::add].values();
        auto venv_name = arguments[arg::to].value();

        v::venv& venv = repo[venv_name];
        for(auto file:file_names){
            auto item = v::item(file);
            venv.add(item);
            venv.push(item);
        }
    }
    
    if(arguments[arg::select]){
        if(arguments[arg::select].values().size()!=1){
            cerr << "usage: -select <venv>" << std::endl;
            return -1;
        }
        
        auto venv_name = arguments[arg::select].value();
        repo.assert_available(venv_name);
        
        if(!repo.active().empty()){
            auto venv = repo[repo.active()];
            venv.push();
        }
        repo.select(venv_name);
        auto venv = repo[repo.active()];
        venv.pull();
    }
    
    if(arguments[arg::list]){
        auto venv = repo[repo.active()];
        if(arguments[arg::list].value().size() > 0){
            auto venv_name = arguments[arg::list].value();
            venv = repo[venv_name];
        }
        std::cout << venv << std::endl;
    }
    
    if(arguments[arg::pull]){
        auto venv = repo[repo.active()];
        venv.pull();
    }
    
    if(arguments[arg::push]){
        auto venv = repo[repo.active()];
        venv.push();
    }
    
    if(argc == 1)
    {
        //std::cout << argv[0] << std::endl;
        std::cout << repo << std::endl;
        if(!repo.active().empty()){
            auto venv = repo[repo.active()];
            for(auto item:venv.items){
                venv.diff(item);
            }
        }
        return 0;
    }
    
    return 0;
}
catch(...) {}
