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
        string selected_venv;
    public:
        void create(const v::venv& venv){
            venvs[venv.name] = venv;
            selected_venv = venv.name;
        }
        repository(){
            std::ifstream ifs(v::root_folder+"/venv");
            if(ifs.is_open()){
                boost::archive::text_iarchive ia(ifs);
                ia & venvs;
                ia & selected_venv;
            }
        }
        ~repository(){
            std::ofstream ofs(v::root_folder+"/venv");
            if(ofs.is_open()){
                boost::archive::text_oarchive oa(ofs);
                oa & venvs;
                oa & selected_venv;
            }
        }
        void select(const std::string& name){
            this->assert_available(name);
            this->selected_venv = name;
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
                std::cerr << "can't find venv \"" << name << "\"" << std::endl;
                throw -1;
            }
        }
        const string& active() const {
            if(this->selected_venv.empty()){
                std::cerr << "select a valid venv." << std::endl;
                throw -1;
            }
            return selected_venv;
        }
        const map& all_venvs() const {
            return this->venvs;
        }
    };
    
    inline
    std::ostream& operator <<(std::ostream& stream, repository& v){
        auto venvs = v.all_venvs();
        if(venvs.empty()){
            stream << "repository is empty.";
            return stream;
        }
        x::table out("venv list:");
        out("id")("files")("name")("selected")++;
        for(auto venv:venvs){
            auto s = venv.second;
            std::stringstream size;
            size << s.items.size();
            
            out(s.id)(size.str())(s.name)(s.name==v.active()?"*":"")++;
        }
        stream << out;
        return stream;
    }
}

#endif /* repository_hpp */
