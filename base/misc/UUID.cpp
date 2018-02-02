//
// Created by zhaohaibin on 1/31/18.
//

#include "UUID.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <sstream>


string UUID::Generate()
{
    boost::uuids::random_generator rgen;
    boost::uuids::uuid u = rgen();
    std::stringstream strstream;
    strstream << u;
    return  strstream.str();
}