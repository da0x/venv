//
//  repository.hpp
//  venv
//
//  Created by Daher Alfawares on 11/30/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#ifndef repository_hpp
#define repository_hpp

#include "static.hpp"
#include "x_table.hpp"
#include "venv.hpp"

#include <map>
#include <fstream>
#include <string>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>

namespace v {
    namespace {
        const std::string root_folder = ".venv";
    }
    
    class repository {
        typedef std::string string;
        typedef std::map<string,v::venv> map;
        map venvs;
        string current_venv;
    public:
        void create(const v::venv& venv){
            venvs[venv.name] = venv;
            current_venv = venv.name;
        }
        repository(){
            std::ifstream ifs(v::root_folder+"/venv");
            if(ifs.is_open()){
                boost::archive::text_iarchive ia(ifs);
                ia & venvs;
                ia & current_venv;
            }
        }
        ~repository(){
            std::ofstream ofs(v::root_folder+"/venv");
            if(ofs.is_open()){
                boost::archive::text_oarchive oa(ofs);
                oa & venvs;
                oa & current_venv;
            }
        }
        void select(const std::string& name){
            this->assert_available(name);
        }
        venv & operator [](const string& name){
            this->assert_available(name);
            return venvs[name];
        }
        bool exists(const string& name) const {
            if(venvs.find(name) == venvs.end()){
                return false;
            }
            return true;
        }
        void assert_available(const string& name) const {
            if(!this->exists(name)){
                std::cerr << "can't find venv [" << name << "]" << std::endl;
                throw -1;
            }
        }
        void assert_available(const std::vector<string>& names) const {
            for(auto name:names){
                this->assert_available(name);
            }
        }
        const map& all_venvs() const {
            return this->venvs;
        }
        venv& current(){
            this->assert_available(this->current_venv);
            return this->venvs[this->current_venv];
        }
        venv& current(string name){
            this->assert_available(name);
            this->current_venv = name;
            return current();
        }
    };
    
    inline
    std::ostream& operator << (std::ostream& stream, repository& repo){
        auto venvs = repo.all_venvs();
        if(venvs.empty()){
            stream << "repository is empty.";
            return stream;
        }
        x::table out("list");
        out("id")("name")("internal file")++;
        for(auto venv:venvs){
            auto v = venv.second;
            for(auto i:v.items){
                auto name = "☐ "+v.name;
                if(v.name == repo.current().name){
                    name = "☒ "+v.name;
                }
                out(name)(i.filename)(v.id)++;
            }
        }
        stream << out;
        return stream;
    }
}

#endif /* repository_hpp */
