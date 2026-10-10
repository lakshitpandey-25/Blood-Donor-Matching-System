#ifndef DONATIONHISTORY_H
#define DONATIONHISTORY_H

#include <QVector>
#include <QString>
#include <QDate>
#include "Donation.h"

class DonationHistory {
private:
    int donorId;
    QVector<Donation> donations;

public:
    DonationHistory();
    explicit DonationHistory(int donorId);

    int getDonorId() const;
    void setDonorId(int donorId);

    void addDonation(const Donation &donation);
    QVector<Donation> getDonations() const;
    int getTotalDonations() const;
    QDate getLastDonationDate() const;

    bool exportToCsv(const QString &filePath) const;
};

#endif // DONATIONHISTORY_H
