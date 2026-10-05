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

    DatabaseManager database;

    if (!database.openDatabase())
    {
        return false;
    }

    database.createTables();

    QString role;
    int userId = 0;

    return database.authenticateUser(
        email,
        password,
        role,
        userId);
}

QString LoginManager::getUserRole(const QString& email,
                                  const QString& password) const
{
    DatabaseManager database;

    if (!database.openDatabase())
    {
        return "";
    }

    database.createTables();

    QString role;
    int userId = 0;

    if (database.authenticateUser(
            email,
            password,
            role,
            userId))
    {
        return role;
    }

    return "";
}

int LoginManager::getUserId(const QString& email,
                            const QString& password) const
{
    DatabaseManager database;

    if (!database.openDatabase())
    {
        return 0;
    }

    database.createTables();

    QString role;
    int userId = 0;

    if (database.authenticateUser(
            email,
            password,
            role,
            userId))
    {
        return userId;
    }

    return 0;
}