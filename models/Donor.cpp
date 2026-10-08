 #include "Donor.h"
#include <QFile>
#include <QTextStream>

static const int COOLDOWN_DAYS = 90;
static const int MIN_AGE = 18;
static const int MAX_AGE = 65;

Donor::Donor()
{
    age = 0;
    eligible = false;
}

Donor::Donor(int userId, QString name, QString phone,
             QString email, QString password,
             QString bloodGroup, int age,
             QString gender, QString city,
             QString lastDonationDate, bool eligible)
    : User(userId, name, phone, email, password)
{
    this->bloodGroup = bloodGroup;
    this->age = age;
    this->gender = gender;
    this->city = city;
    this->lastDonationDate = lastDonationDate;
    this->eligible = eligible;
    history.setDonorId(userId);
}

QString Donor::getBloodGroup() const { return bloodGroup; }
int Donor::getAge() const { return age; }
QString Donor::getGender() const { return gender; }
QString Donor::getCity() const { return city; }
QString Donor::getLastDonationDate() const { return lastDonationDate; }
bool Donor::isEligible() const { return eligible; }
DonationHistory &Donor::getHistory() { return history; }

void Donor::setBloodGroup(QString bloodGroup) { this->bloodGroup = bloodGroup; }
void Donor::setAge(int age) { this->age = age; }
void Donor::setGender(QString gender) { this->gender = gender; }
void Donor::setCity(QString city) { this->city = city; }
void Donor::setLastDonationDate(QString date) { this->lastDonationDate = date; }
void Donor::setEligible(bool eligible) { this->eligible = eligible; }

// 90-day cooldown + age check
void Donor::updateEligibility()
{
    bool ageOk = (age >= MIN_AGE && age <= MAX_AGE);

    bool cooldownOk = true;
    QDate last = QDate::fromString(lastDonationDate, "yyyy-MM-dd");
    if (last.isValid()) {
        cooldownOk = (last.daysTo(QDate::currentDate()) >= COOLDOWN_DAYS);
    }

    eligible = ageOk && cooldownOk;
}

void Donor::addDonation(const Donation &donation)
{
    history.addDonation(donation);
    lastDonationDate = donation.getDonationDate().toString("yyyy-MM-dd");
    updateEligibility();
}

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
        << (lastDonationDate.isEmpty() ? "N/A" : lastDonationDate) << ","
        << (eligible ? "Yes" : "No") << "\n";

    file.close();
    return true;
}

QString Donor::displayProfile() const
{
    return QString("Donor: %1\nBlood Group: %2\nAge: %3\nGender: %4\nCity: %5\n"
                   "Last Donation: %6\nEligible: %7")
        .arg(getName())
        .arg(bloodGroup)
        .arg(age)
        .arg(gender)
        .arg(city)
        .arg(lastDonationDate.isEmpty() ? "N/A" : lastDonationDate)
        .arg(eligible ? "Yes" : "No");
}

QString Donor::getRole() const
{
    return "Donor";
}
