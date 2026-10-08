#include "Donor.h"
#include <QFile>
#include <QTextStream>

static const int COOLDOWN_DAYS = 90;
static const int MIN_AGE = 18;
static const int MAX_AGE = 65;

// ---------- Constructors ----------
Donor::Donor()
    : User(),
      bloodGroup(""),
      age(0),
      gender(""),
      city(""),
      lastDonationDate(),
      eligible(false)
{
}

Donor::Donor(int userId, const QString &name, const QString &phone,
             const QString &email, const QString &password,
             const QString &bloodGroup, int age, const QString &gender,
             const QString &city, const QDate &lastDonationDate)
    : User(userId, name, phone, email, password),
      bloodGroup(bloodGroup),
      age(age),
      gender(gender),
      city(city),
      lastDonationDate(lastDonationDate),
      eligible(false)
{
    updateEligibility();
}

// ---------- Getters ----------
QString Donor::getBloodGroup() const { return bloodGroup; }
int Donor::getAge() const { return age; }
QString Donor::getGender() const { return gender; }
QString Donor::getCity() const { return city; }
QDate Donor::getLastDonationDate() const { return lastDonationDate; }
bool Donor::isEligible() const { return eligible; }
DonationHistory &Donor::getHistory() { return history; }

// ---------- Setters ----------
void Donor::setBl
