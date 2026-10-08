 #ifndef DONOR_H
#define DONOR_H

#include <QString>
#include <QDate>
#include "User.h"
#include "DonationHistory.h"

class Donor : public User
{
private:
    QString bloodGroup;
    int age;
    QString gender;
    QString city;
    QString lastDonationDate;   // format: yyyy-MM-dd
    bool eligible;
    DonationHistory history;    // Composition

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
    DonationHistory &getHistory();

    void setBloodGroup(QString bloodGroup);
    void setAge(int age);
    void setGender(QString gender);
    void setCity(QString city);
    void setLastDonationDate(QString date);
    void setEligible(bool eligible);

    void updateEligibility();                 // 90-day cooldown
    void addDonation(const Donation &donation);
    bool exportToCsv(const QString &filePath) const;

    virtual QString displayProfile() const;
    QString getRole() const override;
};

#endif
