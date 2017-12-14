//
//  venv.hpp
//  venv
//
//  Created by Daher Alfawares on 11/30/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#ifndef venv_hpp
#define venv_hpp

#include "x_shell.hpp"
#include "x_table.hpp"
#include "x_uuid.hpp"
#include "venv_item.hpp"
#include "static.hpp"

#include <string>
#include <sstream>
#include <set>
#include <iostream>

namespace v {

    struct venv {
        
        std::string name;
        std::set<item> items;
        std::string id;
        venv(){}
        venv(std::string name):name(name), id(x::uuid::string()) {}
        void add(item i){
            if(items.find(i)==items.end()){
                items.insert(i);
            } else {
                std::cerr << external_path(i) << " already exists." << std::endl;
                throw -1;
            }
        }
        void remove(item i){
            items.erase(i);
        }
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & this->name;
            ar & this->id;
            ar & this->items;
        }
        std::string internal_path(const item& item){
            return v::root_folder + "/" + this->id + "/" + item.id;
        }
        std::string external_path(const item& item){
            return item.filename;
        }
        void pull(const item& item){
            std::string src = this->internal_path(item);
            std::string dst = this->external_path(item);
            std::string command = x::shell::copy(src,dst).c_str();
            ::system(command.c_str());
        }
        void pull(){
            std::cout << "pulling..." << std::endl;
            for(auto item:this->items){
                this->pull(item);
            }
        }
        void push(const item& item){
            std::string src = this->external_path(item);
            std::string dst = this->internal_path(item);
            std::string command = x::shell::copy(src,dst);
            ::system(command.c_str());
        }
        void push(){
            std::cout << "pushing..." << std::endl;
            for(auto item:this->items){
                push(item);
            }
        }
        void diff(const item& item){
            std::string src = this->external_path(item);
            std::string dst = this->internal_path(item);
            std::string command = x::shell::diff(src,dst);
            ::system(command.c_str());
        }
    };
    
    inline
    std::ostream& operator << (std::ostream& stream,const venv& venv){
        x::table out("files in "+venv.name);
        out("id")("file")++;
        for(auto item:venv.items){
            out(item.id)(item.filename)++;
        }
        stream << out;
        return stream;
    }
}


#endif /* venv_hpp */
