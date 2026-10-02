#ifndef RECIPIENT_H
#define RECIPIENT_H

#include "User.h"

class Recipient : public User
{
private:
    QString requiredBloodGroup;
    QString city;

public:
    Recipient();

    Recipient(int userId, QString name, QString phone,
              QString email, QString password,
              QString requiredBloodGroup, QString city);

    QString getRequiredBloodGroup() const;
    QString getCity() const;

    void setRequiredBloodGroup(QString bloodGroup);
    void setCity(QString city);

    QString getRole() const override;
};

#endif