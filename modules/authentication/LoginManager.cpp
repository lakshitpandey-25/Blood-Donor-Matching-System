#include "LoginManager.h"
#include "../database/DatabaseManager.h"

LoginManager::LoginManager()
{
}

bool LoginManager::validateLogin(const QString& email,
                                 const QString& password) const
{
    return !getUserRole(email, password).isEmpty();
}

bool LoginManager::validateLogin(const QString& email,
                                 const QString& password,
                                 const QString& selectedRole) const
{
    int userId = -1;
    return validateLogin(email, password, selectedRole, userId);
}

bool LoginManager::validateLogin(const QString& email,
                                 const QString& password,
                                 const QString& selectedRole,
                                 int& userId) const
{
    userId = -1;

    if (email.trimmed().isEmpty() || password.isEmpty())
        return false;

    DatabaseManager database;

    if (!database.openDatabase())
        return false;

    if (!database.createTables())
        return false;

    QString actualRole;
    int authenticatedUserId = -1;

    if (!database.authenticateUser(
            email.trimmed(), password,
            actualRole, authenticatedUserId))
        return false;

    if (actualRole.compare(selectedRole, Qt::CaseInsensitive) != 0)
        return false;

    userId = authenticatedUserId;
    return true;
}

QString LoginManager::getUserRole(const QString& email,
                                  const QString& password) const
{
    if (email.trimmed().isEmpty() || password.isEmpty())
        return "";

    DatabaseManager database;

    if (!database.openDatabase())
        return "";

    if (!database.createTables())
        return "";

    QString role;
    int userId = 0;

    if (database.authenticateUser(
            email.trimmed(), password, role, userId))
        return role;

    return "";
}

int LoginManager::getUserId(const QString& email,
                             const QString& password) const
{
    if (email.trimmed().isEmpty() || password.isEmpty())
        return 0;

    DatabaseManager database;

    if (!database.openDatabase())
        return 0;

    if (!database.createTables())
        return 0;

    QString role;
    int userId = 0;

    if (database.authenticateUser(
            email.trimmed(), password, role, userId))
        return userId;

    return 0;
}