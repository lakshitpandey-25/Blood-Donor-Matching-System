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
void Donor::setBloodGroup(const QString &bloodGroup)
{
    this->bloodGroup = bloodGroup;
}

void Donor::setAge(int age)
{
    this->age = age;
    updateEligibility();
}

void Donor::setGender(const QString &gender)
{
    this->gender = gender;
}

void Donor::setCity(const QString &city)
{
    this->city = city;
}

void Donor::setLastDonationDate(const QDate &date)
{
    this->lastDonationDate = date;
    updateEligibility();
}

// ---------- Eligibility (90-day cooldown) ----------
void Donor::updateEligibility()
{
    bool ageOk = (age >= MIN_AGE && age <= MAX_AGE);

    bool cooldownOk = true;
    if (lastDonationDate.isValid()) {
        cooldownOk = (lastDonationDate.daysTo(QDate::currentDate()) >= COOLDOWN_DAYS);
    }

    eligible = ageOk && cooldownOk;
}

// ---------- Donation add ----------
void Donor::addDonation(const Donation &donation)
{
    history.addDonation(donation);
    lastDonationDate = donation.getDonationDate();
    updateEligibility();
}

// ---------- CSV export ----------
bool Donor::exportToCsv(const QString &filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    out << "Name,BloodGroup,Age,Gender,City,LastDonationDate,Eligible\n";
    out << getName() << ","
        << bloodGroup << ","
        << age << ","
        << gender << ","
        << city << ","
        << (lastDonationDate.isValid() ? lastDonationDate.toString("yyyy-MM-dd") : "N/A") << ","
        << (eligible ? "Yes" : "No") << "\n";

    file.close();
    return true;
}

// ---------- Polymorphism ----------
QString Donor::displayProfile() const
{
    return QString("Donor: %1\nBlood Group: %2\nAge: %3\nGender: %4\nCity: %5\n"
                   "Last Donation: %6\nEligible: %7")
        .arg(getName())
        .arg(bloodGroup)
        .arg(age)
        .arg(gender)
        .arg(city)
        .arg(lastDonationDate.isValid() ? lastDonationDate.toString("dd MMM yyyy") : "N/A")
        .arg(eligible ? "Yes" : "No");
}

QString Donor::getRole() const
{
    return "Donor";
}
