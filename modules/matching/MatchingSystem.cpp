#include "MatchingSystem.h"
MatchingSystem::MatchingSystem()
{
}

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

        // 2. Check location when request city is provided
        if (!isLocationMatch(
                donor->getCity(),
                request.getCity()))
        {
            continue;
        }

        // 3. Check current donor eligibility
        if (!isEligibleForDonation(donor))
        {
            continue;
        }

        // 4. Check 90-day donation cooldown
        if (!passed90DayCooldown(donor))
        {
            continue;
        }

        matchingDonors.append(donor);
    }

    return matchingDonors;
}

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

    // Same blood group is always compatible.
    if (donor == required)
    {
        return true;
    }

    // O- can donate to all blood groups.
    if (donor == "O-")
    {
        return true;
    }

    // O+ can donate to positive blood groups.
    if (donor == "O+")
    {
        return required == "A+" ||
               required == "B+" ||
               required == "AB+" ||
               required == "O+";
    }

    // A- can donate to A and AB, both negative and positive.
    if (donor == "A-")
    {
        return required == "A-" ||
               required == "A+" ||
               required == "AB-" ||
               required == "AB+";
    }

    // A+ can donate to A+ and AB+.
    if (donor == "A+")
    {
        return required == "A+" ||
               required == "AB+";
    }

    // B- can donate to B and AB, both negative and positive.
    if (donor == "B-")
    {
        return required == "B-" ||
               required == "B+" ||
               required == "AB-" ||
               required == "AB+";
    }

    // B+ can donate to B+ and AB+.
    if (donor == "B+")
    {
        return required == "B+" ||
               required == "AB+";
    }

    // AB- can donate to AB- and AB+.
    if (donor == "AB-")
    {
        return required == "AB-" ||
               required == "AB+";
    }

    // AB+ can donate only to AB+.
    if (donor == "AB+")
    {
        return required == "AB+";
    }

    return false;
}

bool MatchingSystem::isLocationMatch(
    const QString& donorCity,
    const QString& requestCity) const
{
    QString donorLocation = donorCity.trimmed();
    QString requestLocation = requestCity.trimmed();

    // If no city is specified in the request,
    // location should not restrict matching.
    if (requestLocation.isEmpty())
    {
        return true;
    }

    return donorLocation.compare(
               requestLocation,
               Qt::CaseInsensitive) == 0;
}

bool MatchingSystem::isEligibleForDonation(
    const Donor* donor) const
{
    if (donor == nullptr)
    {
        return false;
    }

    return donor->isEligible();
}

bool MatchingSystem::passed90DayCooldown(
    const Donor* donor) const
{
    if (donor == nullptr)
    {
        return false;
    }

    QString lastDonation = donor->getLastDonationDate().trimmed();

    // No previous donation means the donor has no cooldown restriction.
    if (lastDonation.isEmpty())
    {
        return true;
    }

    QDate lastDonationDate =
        QDate::fromString(lastDonation, "yyyy-MM-dd");

    // Invalid date is not treated as a recent donation.
    if (!lastDonationDate.isValid())
    {
        return false;
    }

    QDate today = QDate::currentDate();

    int daysSinceDonation =
        lastDonationDate.daysTo(today);

    // Future dates are not accepted as having completed cooldown.
    if (daysSinceDonation < 0)
    {
        return false;
    }

    return daysSinceDonation >= 90;
}