#include "DonationHistory.h"
#include <QFile>
#include <QTextStream>

DonationHistory::DonationHistory() : donorId(0) {}

DonationHistory::DonationHistory(int donorId) : donorId(donorId) {}

int DonationHistory::getDonorId() const { return donorId; }
void DonationHistory::setDonorId(int donorId) { this->donorId = donorId; }

void DonationHistory::addDonation(const Donation &donation) {
    donations.append(donation);
}

QVector<Donation> DonationHistory::getDonations() const { return donations; }

int DonationHistory::getTotalDonations() const { return donations.size(); }

QDate DonationHistory::getLastDonationDate() const {
    QDate latest;
    for (const Donation &d : donations) {
        if (!latest.isValid() || d.getDonationDate() > latest)
            latest = d.getDonationDate();
    }
    return latest;
}

bool DonationHistory::exportToCsv(const QString &filePath) const {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    out << "donationId,donorId,donationDate,hospital,bloodGroup\n";
    for (const Donation &d : donations) {
        out << d.getDonationId() << ","
            << d.getDonorId() << ","
            << d.getDonationDate().toString(Qt::ISODate) << ","
            << d.getHospital() << ","
            << d.getBloodGroup() << "\n";
    }
    file.close();
    return true;
}
