#include "Admin.h"

Admin::Admin()
{
}

Admin::Admin(int userId, QString name, QString phone,
             QString email, QString password)
    : User(userId, name, phone, email, password)
{
}

QString Admin::getRole() const
{
    return "Admin";
}