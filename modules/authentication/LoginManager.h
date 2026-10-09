#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include <QString>

#include "../database/DatabaseManager.h"

class LoginManager
{
public:
    LoginManager();

    bool validateLogin(const QString& email,
                       const QString& password) const;

    QString getUserRole(const QString& email,
                        const QString& password) const;

    int getUserId(const QString& email,
                  const QString& password) const;
};

#endif