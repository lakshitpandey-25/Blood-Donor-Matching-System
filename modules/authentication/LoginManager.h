#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include <QString>

class LoginManager
{
public:
    LoginManager();

    bool validateLogin(const QString& email,
                       const QString& password,
                       const QString& selectedRole) const;

    bool validateLogin(const QString& email,
                       const QString& password,
                       const QString& selectedRole,
                       int& userId) const;
};

#endif
