#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

class Admin : public User
{
public:
    Admin();

    Admin(int userId, QString name, QString phone,
          QString email, QString password);

    QString getRole() const override;
};

#endif