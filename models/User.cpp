#include "User.h"

User::User()
{
    userId = 0;
}

User::User(int userId, QString name, QString phone,
           QString email, QString password)
{
    this->userId = userId;
    this->name = name;
    this->phone = phone;
    this->email = email;
    this->password = password;
}

User::~User()
{
}

int User::getUserId() const
{
    return userId;
}

QString User::getName() const
{
    return name;
}

QString User::getPhone() const
{
    return phone;
}

QString User::getEmail() const
{
    return email;
}

QString User::getPassword() const
{
    return password;
}

void User::setName(QString name)
{
    this->name = name;
}

void User::setPhone(QString phone)
{
    this->phone = phone;
}

void User::setEmail(QString email)
{
    this->email = email;
}

void User::setPassword(QString password)
{
    this->password = password;
}