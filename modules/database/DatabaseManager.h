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

class DatabaseManager
{
private:
    QString connectionName;
    mutable QString lastErrorMessage;

    // ---------- Internal Helpers ----------
    QSqlDatabase database() const;
    QString hashPassword(const QString& password) const;

    int insertUser(const User& user);
    bool updateUserInfo(const User& user);
    bool createDefaultAdmin();

public:
    // ---------- Constructor ----------
    DatabaseManager();

    // ---------- Connection ----------
    bool openDatabase();
    bool createTables();
    bool isOpen() const;

    QString getDatabasePath() const;
    QString getLastError() const;

    // ---------- Create ----------
    // Returns the new user ID, or -1 on failure.
    int addDonor(const Donor& donor);
    int addRecipient(const Recipient& recipient);
    int addAdmin(const Admin& admin);

    // Returns the new blood request ID, or -1 on failure.
    int addBloodRequest(
        int recipientId,
        const QString& bloodGroup,
        const QString& city,
        int units
    );

    // ---------- Read ----------
    bool emailExists(const QString& email) const;

    // On success:
    // - role is filled with the user's role
    // - userId is filled with the user's ID
    // - returns true
    bool authenticateUser(
        const QString& email,
        const QString& password,
        QString& role,
        int& userId
    ) const;

    QString getUserRole(int userId) const;

    // Returns a default-constructed object if not found.
    Donor getDonorById(int userId) const;
    Recipient getRecipientById(int userId) const;
    Admin getAdminById(int userId) const;

    QList<Donor> getAllDonors() const;
    QList<Recipient> getAllRecipients() const;
    QList<Admin> getAllAdmins() const;

    // Blood request map keys:
    // requestId
    // recipientId
    // recipientName
    // bloodGroup
    // city
    // units
    // status
    // createdAt
    QList<QVariantMap> getAllBloodRequests() const;

    int countUsersByRole(const QString& role) const;

    // If status is empty, returns total number of requests.
    // Otherwise, returns requests having the given status.
    int countBloodRequests(
        const QString& status = QString()
    ) const;

    // ---------- Update ----------
    bool updateDonor(const Donor& donor);
    bool updateRecipient(const Recipient& recipient);
    bool updateAdmin(const Admin& admin);

    bool updatePassword(
        int userId,
        const QString& newPassword
    );

    bool updateDonorEligibility(
        int userId,
        bool eligible
    );

    bool updateBloodRequestStatus(
        int requestId,
        const QString& status
    );

    // ---------- Delete ----------
    bool deleteUser(int userId);
    bool deleteBloodRequest(int requestId);
};

#endif // DATABASEMANAGER_H