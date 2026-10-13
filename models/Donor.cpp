#include "Donor.h"

Donor::Donor()
{
    age = 0;
    eligible = false;
}

Donor::Donor(int userId, QString name, QString phone,
             QString email, QString password,
             QString bloodGroup, int age,
             QString gender, QString city,
             QString lastDonationDate, bool eligible)
    : User(userId, name, phone, email, password)
{
    this->bloodGroup = bloodGroup;
    this->age = age;
    this->gender = gender;
    this->city = city;
    this->lastDonationDate = lastDonationDate;
    this->eligible = eligible;
}

QString Donor::getBloodGroup() const
{
    return bloodGroup;
}

int Donor::getAge() const
{
    return age;
}

QString Donor::getGender() const
{
    return gender;
}

QString Donor::getCity() const
{
    return city;
}

QString Donor::getLastDonationDate() const
{
    return lastDonationDate;
}

bool Donor::isEligible() const
{
    return eligible;
}

void Donor::setBloodGroup(QString bloodGroup)
{
    this->bloodGroup = bloodGroup;
}

void Donor::setAge(int age)
{
    this->age = age;
}

void Donor::setGender(QString gender)
{
    this->gender = gender;
}

void Donor::setCity(QString city)
{
    this->city = city;
}

void Donor::setLastDonationDate(QString date)
{
    this->lastDonationDate = date;
}

void Donor::setEligible(bool eligible)
{
    this->eligible = eligible;
}

QString Donor::getRole() const
{
    return "Donor";
}