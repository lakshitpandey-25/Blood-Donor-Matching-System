#include "LoginManager.h"

LoginManager::LoginManager()
{
}

bool LoginManager::validateLogin(const QString& email,
                                 const QString& password) const
{
    if (email.isEmpty() || password.isEmpty())
    {
        return false;
    }

    return true;
}