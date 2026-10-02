#include "AuthenticationManager.h"

AuthenticationManager::AuthenticationManager()
{
}

bool AuthenticationManager::validateRegistration(const User& user) const
{
    if (user.getName().isEmpty())
    {
        return false;
    }

    if (user.getEmail().isEmpty())
    {
        return false;
    }

    if (user.getPassword().isEmpty())
    {
        return false;
    }

    return true;
}