//
//  x_uuid.hpp
//  x
//
//  Created by Daher Alfawares on 12/14/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#ifndef x_uuid_hpp
#define x_uuid_hpp

#include <string>
#include <sstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace x {
    namespace uuid {
        inline std::string string(){
            std::stringstream str;
            boost::uuids::uuid u = boost::uuids::random_generator()();
            str << u;
            return str.str();
        }
    }
}

#endif /* x_uuid_hpp */
