#include "LoginManager.h"
#include "../database/DatabaseManager.h"

LoginManager::LoginManager()
{
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

    QString actualRole;
    int authenticatedUserId = -1;

    if (!database.authenticateUser(
            email.trimmed(), password, actualRole, authenticatedUserId))
        return false;

    if (actualRole.compare(selectedRole, Qt::CaseInsensitive) != 0)
        return false;

    userId = authenticatedUserId;
    return true;
}
