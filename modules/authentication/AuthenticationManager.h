#ifndef AUTHENTICATIONMANAGER_H
#define AUTHENTICATIONMANAGER_H

#include "../../models/User.h"

class AuthenticationManager
{
public:
    AuthenticationManager();

    bool validateRegistration(const User& user) const;
};

#endif