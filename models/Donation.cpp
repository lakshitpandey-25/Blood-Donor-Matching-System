#include "Donation.h"

// Default constructor
Donation::Donation()
    : donationId(0), donorId(0), donationDate(QDate()), hospital(""), bloodGroup("")
{
}

// Parameterized constructor
Donation::Donation(int donationId, int donorId, const QDate &donationDate,
                   const QString &hospital, const QString &bloodGroup)
    : donationId(donationId), donorId(donorId), donationDate(donationDate),
      hospital(hospital), bloodGroup(bloodGroup)
{
}

// Getters
int Donation::getDonationId() const { return donationId; }
int Donation::getDonorId() const { return donorId; }
QDate Donation::getDonationDate() const { return donationDate; }
QString Donation::getHospital() const { return hospital; }
QString Donation::getBloodGroup() const { return bloodGroup; }

// Setters
void Donation::setDonationId(int donationId) { this->donationId = donationId; }
void Donation::setDonorId(int donorId) { this->donorId = donorId; }
void Donation::setDonationDate(const QDate &donationDate) { this->donationDate = donationDate; }
void Donation::setHospital(const QString &hospital) { this->hospital = hospital; }
void Donation::setBloodGroup(const QString &bloodGroup) { this->bloodGroup = bloodGroup; }
