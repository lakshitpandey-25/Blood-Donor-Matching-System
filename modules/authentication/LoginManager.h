
#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include <QString>

class LoginManager
{
public:
    LoginManager();

    bool validateLogin(const QString& email,
                       const QString& password,
                       QString& role,
                       int& userId) const;
};

#endif