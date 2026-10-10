#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QList>
#include <QVariantMap>
#include <QSqlDatabase>

#include "../../models/User.h"
#include "../../models/Donor.h"
#include "../../models/Recipient.h"
#include "../../models/Admin.h"
#include "../../models/Donation.h"   // NEW

class DatabaseManager
{
private:
    QString connectionName;
    mutable QString lastErrorMessage;

    QSqlDatabase database() const;
    QString hashPassword(const QString& password) const;

    int insertUser(const User& user);
    bool updateUserInfo(const User& user);
    bool createDefaultAdmin();

public:
    DatabaseManager();

    // ---------- Connection ----------
    bool openDatabase();
    bool createTables();
    bool isOpen() const;
    QString getDatabasePath() const;
    QString getLastError() const;

    // ---------- Create ----------
    // Each add function returns the new userId, or -1 on failure
    int addDonor(const Donor& donor);
    int addRecipient(const Recipient& recipient);
    int addAdmin(const Admin& admin);

    // Returns the new request id, or -1 on failure
    int addBloodRequest(int recipientId, const QString& bloodGroup,
                        const QString& city, int units);

    // Validates age, date and 90-day cooldown, then saves the donation.
    // Returns the new donationId, or -1 on failure (reason in error).
    int addDonation(Donation& donation, QString& error);   // NEW

    // ---------- Read ----------
    bool emailExists(const QString& email) const;

    // On success fills role and userId and returns true
    bool authenticateUser(const QString& email, const QString& password,
                          QString& role, int& userId) const;

    QString getUserRole(int userId) const;

    // These return a default-constructed object (userId 0) if not found
    Donor getDonorById(int userId) const;
    Recipient getRecipientById(int userId) const;
    Admin getAdminById(int userId) const;

    QList<Donor> getAllDonors() const;
    QList<Recipient> getAllRecipients() const;
    QList<Admin> getAllAdmins() const;

    // Keys: requestId, recipientId, recipientName, bloodGroup,
    //       city, units, status, createdAt
    QList<QVariantMap> getAllBloodRequests() const;

    QList<Donation> getDonationsByDonor(int donorId) const;   // NEW

    int countUsersByRole(const QString& role) const;
    int countBloodRequests(const QString& status = QString()) const;

    // ---------- Update ----------
    bool updateDonor(const Donor& donor);
    bool updateRecipient(const Recipient& recipient);
    bool updateAdmin(const Admin& admin);
    bool updatePassword(int userId, const QString& newPassword);
    bool updateDonorEligibility(int userId, bool eligible);
    bool updateBloodRequestStatus(int requestId, const QString& status);

    // ---------- Delete ----------
    bool deleteUser(int userId);
    bool deleteBloodRequest(int requestId);
};

#endif