//
//  venv_item.hpp
//  venv
//
//  Created by Daher Alfawares on 12/14/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#ifndef venv_item_hpp
#define venv_item_hpp

#include "x_uuid.hpp"
#include <string>
#include <sstream>

namespace v {
    struct item {
        std::string filename;
        std::string id;
        item(){}
        item(std::string filename): filename(filename), id(x::uuid::string()) {}
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
        {
            ar & this->filename;
            ar & this->id;
        }
        bool operator < (const item& rvalue) const {
            return this->filename < rvalue.filename;
        }
    };
}

#endif /* venv_item_hpp */
