// #include "json.hpp"

#include "App.h"


// #define RegisterLua(lua, class, p1, p2, p3, p4, p5) auto c_##class = lua.new_usertype<class>(#class); c_##class[#p1] = &##class::##p1;
// #define RegisterLua(lua, class, p1, p2) auto c_##class = lua.new_usertype<class>(#class); c_##class[#p1] = &##class::##p1; c_##class[#p2] = &##class::##p2; 

// class address {
// private:
//     std::string street;
//     int housenumber;
//     int postcode;

// public:
//     NLOHMANN_DEFINE_TYPE_INTRUSIVE(address, street, housenumber, postcode)
// };

int main(int argc, char** argv) {
    Plutus::App app;
    app.Run();

    // RegisterLua(lua, myclass, x, y, z, w)
}

