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
#include <vector>
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
        void create(const v::venv& venv){
            if(this->exists(venv.name)){
                std::cerr << "already exists" << std::endl;
                throw -1;
            }
            venvs[venv.name] = venv;
            current_venv = venv.name;
        }
        void rename(string old_name,string new_name){
            this->assert_available(old_name);
            if(this->exists(new_name)){
                std::cerr << "already exists" << std::endl;
                throw -1;
            }
            auto venv = this->venvs[old_name];
            venv.name = new_name;
            this->venvs.erase(old_name);
            this->venvs[new_name] = venv;
            if(current_venv == old_name){
                current_venv = new_name;
            }
        }
        void remove(string name){
            if(this->exists(name)){
                std::cerr << "removing [" << name << "]" << std::endl;
                auto venv = venvs[name];
                for(auto item:venv.items){
                    venv.remove(item);
                }
                this->venvs.erase(name);
                if(current_venv == name){
                    current_venv = "";
                }
                return;
            } else {
                std::cerr << "can't find [" << name << "]" << std::endl;
            }
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
        out("")("label")("path")("internal path")++;
        for(auto venv:venvs){
            auto v = venv.second;
            auto selected = "☐";
            if(v.name == repo.current().name){
                selected = "☒";
            }
            for(auto i:v.items){
                out(selected)(v.name)(v.external_path(i))(v.internal_path(i))++;
            }
        }
        stream << out;
        
        std::vector<std::string> empty_venvs;
        for(auto venv:venvs){
            if(venv.second.items.empty()){
                empty_venvs.push_back(venv.second.name);
            }
        }
        if(!empty_venvs.empty()){
            stream << std::endl << "empty venvs:" << std::endl;
            for(auto name:empty_venvs){
                stream << "\t[" << name << "]" << std::endl;
            }
        }
        return stream;
    }
}

#endif /* repository_hpp */
