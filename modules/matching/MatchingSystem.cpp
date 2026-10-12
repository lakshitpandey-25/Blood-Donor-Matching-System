
#include "MatchingSystem.h"

#include <QDate>

// Constructor
MatchingSystem::MatchingSystem()
{
}

// Find compatible donors
QList<Donor*> MatchingSystem::findMatchingDonors(
    const BloodRequest& request,
    const QList<Donor*>& donors)
{
    QList<Donor*> matchingDonors;

    for (Donor* donor : donors)
    {
        if (donor == nullptr)
        {
            continue;
        }

        // 1. Check blood group compatibility
        if (!isCompatibleBloodGroup(
                donor->getBloodGroup(),
                request.getBloodGroup()))
        {
            continue;
        }

        // 2. Check city
        if (!isLocationMatch(
                donor->getCity(),
                request.getCity()))
        {
            continue;
        }

        // 3. Check donor eligibility
        if (!isEligibleForDonation(donor))
        {
            continue;
        }

        // 4. Check 90-day cooldown
        if (!passed90DayCooldown(donor))
        {
            continue;
        }

        matchingDonors.append(donor);
    }

    return matchingDonors;
}

// Check blood group compatibility
bool MatchingSystem::isCompatibleBloodGroup(
    const QString& donorBloodGroup,
    const QString& requiredBloodGroup) const
{
    QString donor = donorBloodGroup.trimmed().toUpper();
    QString required = requiredBloodGroup.trimmed().toUpper();

    if (donor.isEmpty() || required.isEmpty())
    {
        return false;
    }

    // O negative can donate red blood cells to all groups.
    if (donor == "O-")
    {
        return true;
    }

    // O positive
    if (donor == "O+")
    {
        return required == "O+" ||
               required == "A+" ||
               required == "B+" ||
               required == "AB+";
    }

    // A negative
    if (donor == "A-")
    {
        return required == "A-" ||
               required == "A+" ||
               required == "AB-" ||
               required == "AB+";
    }

    // A positive
    if (donor == "A+")
    {
        return required == "A+" ||
               required == "AB+";
    }

    // B negative
    if (donor == "B-")
    {
        return required == "B-" ||
               required == "B+" ||
               required == "AB-" ||
               required == "AB+";
    }

    // B positive
    if (donor == "B+")
    {
        return required == "B+" ||
               required == "AB+";
    }

    // AB negative
    if (donor == "AB-")
    {
        return required == "AB-" ||
               required == "AB+";
    }

    // AB positive
    if (donor == "AB+")
    {
        return required == "AB+";
    }

    return false;
}

// Check city matching
bool MatchingSystem::isLocationMatch(
    const QString& donorCity,
    const QString& requestCity) const
{
    QString donorLocation = donorCity.trimmed();
    QString requestLocation = requestCity.trimmed();

    // No city restriction if request city is empty.
    if (requestLocation.isEmpty())
    {
        return true;
    }

    return donorLocation.compare(
        requestLocation,
        Qt::CaseInsensitive) == 0;
}

// Check donor's eligibility flag
bool MatchingSystem::isEligibleForDonation(
    const Donor* donor) const
{
    if (donor == nullptr)
    {
        return false;
    }

    return donor->isEligible();
}

// Check the 90-day cooldown
bool MatchingSystem::passed90DayCooldown(
    const Donor* donor) const
{
    if (donor == nullptr)
    {
        return false;
    }

    QString lastDonation =
        donor->getLastDonationDate().trimmed();

    // Empty means no previous donation is recorded.
    if (lastDonation.isEmpty())
    {
        return true;
    }

    QDate lastDonationDate =
        QDate::fromString(lastDonation, "yyyy-MM-dd");

    // Reject invalid dates.
    if (!lastDonationDate.isValid() ||
        lastDonationDate.toString("yyyy-MM-dd") != lastDonation)
    {
        return false;
    }

    QDate today = QDate::currentDate();

    int daysSinceDonation =
        lastDonationDate.daysTo(today);

    // Reject future dates.
    if (daysSinceDonation < 0)
    {
        return false;
    }

    // At least 90 days must have passed.
    return daysSinceDonation >= 90;
}
