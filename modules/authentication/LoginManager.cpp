#include "LoginManager.h"
#include "../database/DatabaseManager.h"

LoginManager::LoginManager()
{
}

bool LoginManager::validateLogin(const QString& email,
                                 const QString& password) const
{
    QString role;
    int userId = 0;

    return authenticate(email, password, role, userId);
}

bool LoginManager::authenticate(const QString& email,
                                const QString& password,
                                QString& role,
                                int& userId) const
{
    if (email.trimmed().isEmpty() || password.isEmpty())
        return false;

    DatabaseManager database;

    if (!database.openDatabase() || !database.isOpen())
        return false;

    return database.authenticateUser(
        email.trimmed(), password, role, userId);
}
