#include "LoginManager.h"
#include "../database/DatabaseManager.h"

LoginManager::LoginManager()
{
}

bool LoginManager::validateLogin(const QString& email,
                                 const QString& password,
                                 const QString& selectedRole) const
{
    if (email.trimmed().isEmpty() || password.isEmpty())
        return false;

    DatabaseManager database;

    if (!database.openDatabase())
        return false;

    QString actualRole;
    int userId = -1;

    if (!database.authenticateUser(
            email.trimmed(), password, actualRole, userId))
        return false;

    return actualRole.compare(selectedRole, Qt::CaseInsensitive) == 0;
}
