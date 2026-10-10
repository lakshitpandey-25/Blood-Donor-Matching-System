#ifndef DONATION_H
#define DONATION_H

#include <QString>
#include <QDate>

class Donation {
private:
    int donationId;
    int donorId;
    QDate donationDate;
    QString hospital;
    QString bloodGroup;

public:
    Donation();
    Donation(int donationId, int donorId, const QDate &donationDate,
             const QString &hospital, const QString &bloodGroup);

    int getDonationId() const;
    int getDonorId() const;
    QDate getDonationDate() const;
    QString getHospital() const;
    QString getBloodGroup() const;

    void setDonationId(int id);
    void setDonorId(int id);
    void setDonationDate(const QDate &date);
    void setHospital(const QString &hospital);
    void setBloodGroup(const QString &bloodGroup);
};

#endif
