#ifndef MATCHINGSYSTEM_H
#define MATCHINGSYSTEM_H

#include <QList>
#include <QString>
#include <QDate>

#include "../../models/Donor.h"
#include "../../models/BloodRequest.h"

class MatchingSystem
{
public:
    MatchingSystem();

    QList<Donor*> findMatchingDonors(
        const BloodRequest& request,
        const QList<Donor*>& donors);

private:
    bool isCompatibleBloodGroup(
        const QString& donorBloodGroup,
        const QString& requiredBloodGroup) const;

    bool isLocationMatch(
        const QString& donorCity,
        const QString& requestCity) const;

    bool isEligibleForDonation(
        const Donor* donor) const;

    bool passed90DayCooldown(
        const Donor* donor) const;
};

#endif