#ifndef DONOR_H
#define DONOR_H

#include "User.h"

class Donor : public User
{
private:
    QString bloodGroup;
    int age;
    QString gender;
    QString city;
    QString lastDonationDate;
    bool eligible;

public:
    Donor();

    Donor(int userId, QString name, QString phone,
          QString email, QString password,
          QString bloodGroup, int age,
          QString gender, QString city,
          QString lastDonationDate, bool eligible);

    QString getBloodGroup() const;
    int getAge() const;
    QString getGender() const;
    QString getCity() const;
    QString getLastDonationDate() const;
    bool isEligible() const;

    void setBloodGroup(QString bloodGroup);
    void setAge(int age);
    void setGender(QString gender);
    void setCity(QString city);
    void setLastDonationDate(QString date);
    void setEligible(bool eligible);

    QString getRole() const override;
};

#endif