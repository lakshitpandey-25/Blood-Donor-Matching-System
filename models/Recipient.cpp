#include "Recipient.h"

Recipient::Recipient()
{
}

Recipient::Recipient(int userId, QString name, QString phone,
                     QString email, QString password,
                     QString requiredBloodGroup, QString city)
    : User(userId, name, phone, email, password)
{
    this->requiredBloodGroup = requiredBloodGroup;
    this->city = city;
}

QString Recipient::getRequiredBloodGroup() const
{
    return requiredBloodGroup;
}

QString Recipient::getCity() const
{
    return city;
}

void Recipient::setRequiredBloodGroup(QString bloodGroup)
{
    this->requiredBloodGroup = bloodGroup;
}

void Recipient::setCity(QString city)
{
    this->city = city;
}

QString Recipient::getRole() const
{
    return "Recipient";
}