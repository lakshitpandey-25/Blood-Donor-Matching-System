
#include "LoginManager.h"
#include "../database/DatabaseManager.h"

LoginManager::LoginManager()
{
}

bool LoginManager::validateLogin(const QString& email,
                                 const QString& password,
                                 QString& role,
                                 int& userId) const
{
    if (email.trimmed().isEmpty() || password.isEmpty())
        return false;

    DatabaseManager dbManager;

    if (!dbManager.openDatabase())
        return false;

    if (!dbManager.isOpen())
        return false;

    return dbManager.authenticateUser(
        email.trimmed(), password, role, userId);
}