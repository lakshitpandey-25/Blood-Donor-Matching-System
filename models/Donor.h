#ifndef DONOR_H
#define DONOR_H

#include <QString>
#include <QDate>
#include "User.h"
#include "DonationHistory.h"

class Donor : public User {
private:
    QString bloodGroup;
    int age;
    QString gender;
    QString city;
    QDate lastDonationDate;
    bool eligible;
    DonationHistory history;   // Composition

public:
    Donor();
    Donor(int userId, const QString &name, const QString &phone,
          const QString &email, const QString &password,
          const QString &bloodGroup, int age, const QString &gender,
          const QString &city, const QDate &lastDonationDate);

    QString getBloodGroup() const;
    int getAge() const;
    QString getGender() const;
    QString getCity() const;
    QDate getLastDonationDate() const;
    bool isEligible() const;
    DonationHistory &getHistory();

    void setBloodGroup(const QString &bloodGroup);
    void setAge(int age);
    void setGender(const QString &gender);
    void setCity(const QString &city);
    void setLastDonationDate(const QDate &date);

    void updateEligibility();                 // 90-day cooldown
    void addDonation(const Donation &donation);
    bool exportToCsv(const QString &filePath) const;

    // Polymorphism (overrides of User)
    QString displayProfile() const override;
    QString getRole() const override;
};

#endif // DONOR_H
