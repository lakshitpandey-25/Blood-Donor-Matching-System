#ifndef USER_H
#define USER_H

#include <QString>

class User
{
protected:
    int userId;
    QString name;
    QString phone;
    QString email;
    QString password;

public:
    User();
    User(int userId, QString name, QString phone,
         QString email, QString password);

    virtual ~User();

    int getUserId() const;
    QString getName() const;
    QString getPhone() const;
    QString getEmail() const;
    QString getPassword() const;

    void setName(QString name);
    void setPhone(QString phone);
    void setEmail(QString email);
    void setPassword(QString password);

    virtual QString getRole() const = 0;
};

#endif