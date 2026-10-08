#include "DatabaseManager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QStandardPaths>
#include <QDir>
#include <QStringList>

namespace
{
    Donor donorFromQuery(const QSqlQuery& query)
    {
        return Donor(
            query.value(0).toInt(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString(),
            query.value(5).toString(),
            query.value(6).toInt(),
            query.value(7).toString(),
            query.value(8).toString(),
            query.value(9).toString(),
            query.value(10).toBool()
        );
    }

    Recipient recipientFromQuery(const QSqlQuery& query)
    {
        return Recipient(
            query.value(0).toInt(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString(),
            query.value(5).toString(),
            query.value(6).toString()
        );
    }

    Admin adminFromQuery(const QSqlQuery& query)
    {
        return Admin(
            query.value(0).toInt(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString()
        );
    }

    const char* DONOR_SELECT =
        "SELECT u.user_id, u.name, u.phone, u.email, u.password, "
        "d.blood_group, d.age, d.gender, d.city, "
        "d.last_donation_date, d.eligible "
        "FROM users u "
        "JOIN donors d ON u.user_id = d.user_id ";

    const char* RECIPIENT_SELECT =
        "SELECT u.user_id, u.name, u.phone, u.email, u.password, "
        "r.required_blood_group, r.city "
        "FROM users u "
        "JOIN recipients r ON u.user_id = r.user_id ";

    const char* ADMIN_SELECT =
        "SELECT user_id, name, phone, email, password "
        "FROM users WHERE role = 'Admin' ";
}

// ---------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------

DatabaseManager::DatabaseManager()
{
    connectionName = "BloodDonorConnection";
}

// ---------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------

QSqlDatabase DatabaseManager::database() const
{
    return QSqlDatabase::database(connectionName, false);
}

QString DatabaseManager::hashPassword(const QString& password) const
{
    QByteArray hash = QCryptographicHash::hash(
        password.toUtf8(),
        QCryptographicHash::Sha256
    );

    return QString::fromLatin1(hash.toHex());
}

// ---------------------------------------------------------------
// Connection
// ---------------------------------------------------------------

QString DatabaseManager::getDatabasePath() const
{
    QString folder =
        QStandardPaths::writableLocation(
            QStandardPaths::AppDataLocation
        );

    return folder + "/blood_donor.db";
}

bool DatabaseManager::openDatabase()
{
    QSqlDatabase db;

    if (QSqlDatabase::contains(connectionName))
    {
        db = database();
    }
    else
    {
        db = QSqlDatabase::addDatabase(
            "QSQLITE",
            connectionName
        );

        QString folder =
            QStandardPaths::writableLocation(
                QStandardPaths::AppDataLocation
            );

        QDir().mkpath(folder);

        db.setDatabaseName(getDatabasePath());
    }

    if (!db.isOpen() && !db.open())
    {
        lastErrorMessage = db.lastError().text();
        return false;
    }

    QSqlQuery query(db);

    if (!query.exec("PRAGMA foreign_keys = ON"))
    {
        lastErrorMessage = query.lastError().text();
        return false;
    }

    return true;
}

bool DatabaseManager::isOpen() const
{
    return QSqlDatabase::contains(connectionName)
           && database().isOpen();
}

QString DatabaseManager::getLastError() const
{
    return lastErrorMessage;
}

// ---------------------------------------------------------------
// Table creation
// ---------------------------------------------------------------

bool DatabaseManager::createTables()
{
    if (!isOpen())
    {
        lastErrorMessage = "Database is not open.";
        return false;
    }

    QStringList statements;

    // Users
    statements << "CREATE TABLE IF NOT EXISTS users ("
                  "user_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "name TEXT NOT NULL, "
                  "phone TEXT, "
                  "email TEXT NOT NULL UNIQUE, "
                  "password TEXT NOT NULL, "
                  "role TEXT NOT NULL)";

    // Donors
    statements << "CREATE TABLE IF NOT EXISTS donors ("
                  "user_id INTEGER PRIMARY KEY, "
                  "blood_group TEXT NOT NULL, "
                  "age INTEGER, "
                  "gender TEXT, "
                  "city TEXT, "
                  "last_donation_date TEXT, "
                  "eligible INTEGER NOT NULL DEFAULT 1, "
                  "FOREIGN KEY (user_id) REFERENCES users(user_id) "
                  "ON DELETE CASCADE)";

    // Recipients
    statements << "CREATE TABLE IF NOT EXISTS recipients ("
                  "user_id INTEGER PRIMARY KEY, "
                  "required_blood_group TEXT, "
                  "city TEXT, "
                  "FOREIGN KEY (user_id) REFERENCES users(user_id) "
                  "ON DELETE CASCADE)";

    // Blood requests
    statements << "CREATE TABLE IF NOT EXISTS blood_requests ("
                  "request_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "recipient_id INTEGER NOT NULL, "
                  "blood_group TEXT NOT NULL, "
                  "city TEXT, "
                  "units INTEGER NOT NULL DEFAULT 1, "
                  "status TEXT NOT NULL DEFAULT 'Pending', "
                  "created_at TEXT DEFAULT CURRENT_TIMESTAMP, "
                  "FOREIGN KEY (recipient_id) REFERENCES recipients(user_id) "
                  "ON DELETE CASCADE)";

    // Donations table required by the project database design.
    statements << "CREATE TABLE IF NOT EXISTS donations ("
                  "donation_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "donor_id INTEGER NOT NULL, "
                  "donation_date TEXT NOT NULL, "
                  "hospital TEXT, "
                  "blood_group TEXT, "
                  "FOREIGN KEY (donor_id) REFERENCES donors(user_id) "
                  "ON DELETE CASCADE)";

    QSqlQuery query(database());

    for (const QString& statement : statements)
    {
        if (!query.exec(statement))
        {
            lastErrorMessage = query.lastError().text();
            return false;
        }
    }

    return createDefaultAdmin();
}

// ---------------------------------------------------------------
// Default Admin
// ---------------------------------------------------------------

bool DatabaseManager::createDefaultAdmin()
{
    if (countUsersByRole("Admin") > 0)
    {
        return true;
    }

    Admin admin(
        0,
        "System Admin",
        "0000000000",
        "admin@blooddonor.com",
        "admin123"
    );

    return addAdmin(admin) != -1;
}

// ---------------------------------------------------------------
// Create
// ---------------------------------------------------------------

int DatabaseManager::insertUser(const User& user)
{
    QSqlQuery query(database());

    query.prepare(
        "INSERT INTO users "
        "(name, phone, email, password, role) "
        "VALUES (?, ?, ?, ?, ?)"
    );

    query.addBindValue(user.getName());
    query.addBindValue(user.getPhone());
    query.addBindValue(user.getEmail());
    query.addBindValue(hashPassword(user.getPassword()));
    query.addBindValue(user.getRole());

    if (!query.exec())
    {
        lastErrorMessage = query.lastError().text();
        return -1;
    }

    return query.lastInsertId().toInt();
}

int DatabaseManager::addDonor(const Donor& donor)
{
    QSqlDatabase db = database();

    if (!db.transaction())
    {
        lastErrorMessage = db.lastError().text();
        return -1;
    }

    int userId = insertUser(donor);

    if (userId == -1)
    {
        db.rollback();
        return -1;
    }

    QSqlQuery query(db);

    query.prepare(
        "INSERT INTO donors "
        "(user_id, blood_group, age, gender, city, "
        "last_donation_date, eligible) "
        "VALUES (?, ?, ?, ?, ?, ?, ?)"
    );

    query.addBindValue(userId);
    query.addBindValue(donor.getBloodGroup());
    query.addBindValue(donor.getAge());
    query.addBindValue(donor.getGender());
    query.addBindValue(donor.getCity());
    query.addBindValue(donor.getLastDonationDate());
    query.addBindValue(donor.isEligible() ? 1 : 0);

    if (!query.exec())
    {
        lastErrorMessage = query.lastError().text();
        db.rollback();
        return -1;
    }

    if (!db.commit())
    {
        lastErrorMessage = db.lastError().text();
        db.rollback();
        return -1;
    }

    return userId;
}

int DatabaseManager::addRecipient(const Recipient& recipient)
{
    QSqlDatabase db = database();

    if (!db.transaction())
    {
        lastErrorMessage = db.lastError().text();
        return -1;
    }

    int userId = insertUser(recipient);

    if (userId == -1)
    {
        db.rollback();
        return -1;
    }

    QSqlQuery query(db);

    query.prepare(
        "INSERT INTO recipients "
        "(user_id, required_blood_group, city) "
        "VALUES (?, ?, ?)"
    );

    query.addBindValue(userId);
    query.addBindValue(recipient.getRequiredBloodGroup());
    query.addBindValue(recipient.getCity());

    if (!query.exec())
    {
        lastErrorMessage = query.lastError().text();
        db.rollback();
        return -1;
    }

    if (!db.commit())
    {
        lastErrorMessage = db.lastError().text();
        db.rollback();
        return -1;
    }

    return userId;
}

int DatabaseManager::addAdmin(const Admin& admin)
{
    return insertUser(admin);
}

int DatabaseManager::addBloodRequest(
    int recipientId,
    const QString& bloodGroup,
    const QString& city,
    int units)
{
    QSqlQuery query(database());

    query.prepare(
        "INSERT INTO blood_requests "
        "(recipient_id, blood_group, city, units) "
        "VALUES (?, ?, ?, ?)"
    );

    query.addBindValue(recipientId);
    query.addBindValue(bloodGroup);
    query.addBindValue(city);
    query.addBindValue(units);

    if (!query.exec())
    {
        lastErrorMessage = query.lastError().text();
        return -1;
    }

    return query.lastInsertId().toInt();
}

// ---------------------------------------------------------------
// Read
// ---------------------------------------------------------------

bool DatabaseManager::emailExists(const QString& email) const
{
    QSqlQuery query(database());

    query.prepare(
        "SELECT 1 FROM users WHERE email = ?"
    );

    query.addBindValue(email);

    if (!query.exec())
    {
        lastErrorMessage = query.lastError().text();
        return false;
    }

    return query.next();
}

bool DatabaseManager::authenticateUser(
    const QString& email,
    const QString& password,
    QString& role,
    int& userId) const
{
    QSqlQuery query(database());

    query.prepare(
        "SELECT user_id, role "
        "FROM users "
        "WHERE email = ? AND password = ?"
    );

    query.addBindValue(email);
    query.addBindValue(hashPassword(password));

    if (query.exec() && query.next())
    {
        userId = query.value(0).toInt();
        role = query.value(1).toString();

        return true;
    }

    return false;
}

QString DatabaseManager::getUserRole(int userId) const
{
    QSqlQuery query(database());

    query.prepare(
        "SELECT role FROM users WHERE user_id = ?"
    );

    query.addBindValue(userId);

    if (query.exec() && query.next())
    {
        return query.value(0).toString();
    }

    return QString();
}

Donor DatabaseManager::getDonorById(int userId) const
{
    QSqlQuery query(database());

    query.prepare(
        QString(DONOR_SELECT) +
        "WHERE u.user_id = ?"
    );

    query.addBindValue(userId);

    if (query.exec() && query.next())
    {
        return donorFromQuery(query);
    }

    return Donor();
}

Recipient DatabaseManager::getRecipientById(int userId) const
{
    QSqlQuery query(database());

    query.prepare(
        QString(RECIPIENT_SELECT) +
        "WHERE u.user_id = ?"
    );

    query.addBindValue(userId);

    if (query.exec() && query.next())
    {
        return recipientFromQuery(query);
    }

    return Recipient();
}

Admin DatabaseManager::getAdminById(int userId) const
{
    QSqlQuery query(database());

    query.prepare(
        QString(ADMIN_SELECT) +
        "AND user_id = ?"
    );

    query.addBindValue(userId);

    if (query.exec() && query.next())
    {
        return adminFromQuery(query);
    }

    return Admin();
}

QList<Donor> DatabaseManager::getAllDonors() const
{
    QList<Donor> donors;

    QSqlQuery query(database());

    if (query.exec(
            QString(DONOR_SELECT) +
            "ORDER BY u.name"))
    {
        while (query.next())
        {
            donors.append(donorFromQuery(query));
        }
    }
    else
    {
        lastErrorMessage = query.lastError().text();
    }

    return donors;
}

QList<Recipient> DatabaseManager::getAllRecipients() const
{
    QList<Recipient> recipients;

    QSqlQuery query(database());

    if (query.exec(
            QString(RECIPIENT_SELECT) +
            "ORDER BY u.name"))
    {
        while (query.next())
        {
            recipients.append(recipientFromQuery(query));
        }
    }
    else
    {
        lastErrorMessage = query.lastError().text();
    }

    return recipients;
}

QList<Admin> DatabaseManager::getAllAdmins() const
{
    QList<Admin> admins;

    QSqlQuery query(database());

    if (query.exec(
            QString(ADMIN_SELECT) +
            "ORDER BY name"))
    {
        while (query.next())
        {
            admins.append(adminFromQuery(query));
        }
    }
    else
    {
        lastErrorMessage = query.lastError().text();
    }

    return admins;
}

QList<QVariantMap> DatabaseManager::getAllBloodRequests() const
{
    QList<QVariantMap> requests;

    QSqlQuery query(database());

    bool ok = query.exec(
        "SELECT b.request_id, "
        "b.recipient_id, "
        "u.name, "
        "b.blood_group, "
        "b.city, "
        "b.units, "
        "b.status, "
        "b.created_at "
        "FROM blood_requests b "
        "JOIN users u ON b.recipient_id = u.user_id "
        "ORDER BY b.request_id DESC"
    );

    if (!ok)
    {
        lastErrorMessage = query.lastError().text();
        return requests;
    }

    while (query.next())
    {
        QVariantMap request;

        request["requestId"] =
            query.value(0).toInt();

        request["recipientId"] =
            query.value(1).toInt();

        request["recipientName"] =
            query.value(2).toString();

        request["bloodGroup"] =
            query.value(3).toString();

        request["city"] =
            query.value(4).toString();

        request["units"] =
            query.value(5).toInt();

        request["status"] =
            query.value(6).toString();

        request["createdAt"] =
            query.value(7).toString();

        requests.append(request);
    }

    return requests;
}

int DatabaseManager::countUsersByRole(
    const QString& role) const
{
    QSqlQuery query(database());

    query.prepare(
        "SELECT COUNT(*) "
        "FROM users "
        "WHERE role = ?"
    );

    query.addBindValue(role);

    if (query.exec() && query.next())
    {
        return query.value(0).toInt();
    }

    return 0;
}

int DatabaseManager::countBloodRequests(
    const QString& status) const
{
    QSqlQuery query(database());

    if (status.isEmpty())
    {
        query.prepare(
            "SELECT COUNT(*) "
            "FROM blood_requests"
        );
    }
    else
    {
        query.prepare(
            "SELECT COUNT(*) "
            "FROM blood_requests "
            "WHERE status = ?"
        );

        query.addBindValue(status);
    }

    if (query.exec() && query.next())
    {
        return query.value(0).toInt();
    }

    return 0;
}

// ---------------------------------------------------------------
// Update
// ---------------------------------------------------------------

bool DatabaseManager::updateUserInfo(
    const User& user)
{
    QSqlQuery query(database());

    query.prepare(
        "UPDATE users "
        "SET name = ?, phone = ?, email = ? "
        "WHERE user_id = ?"
    );

    query.addBindValue(user.getName());
    query.addBindValue(user.getPhone());
    query.addBindValue(user.getEmail());
    query.addBindValue(user.getUserId());

    if (!query.exec())
    {
        lastErrorMessage = query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool DatabaseManager::updateDonor(
    const Donor& donor)
{
    QSqlDatabase db = database();

    if (!db.transaction())
    {
        lastErrorMessage = db.lastError().text();
        return false;
    }

    if (!updateUserInfo(donor))
    {
        db.rollback();
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "UPDATE donors "
        "SET blood_group = ?, "
        "age = ?, "
        "gender = ?, "
        "city = ?, "
        "last_donation_date = ?, "
        "eligible = ? "
        "WHERE user_id = ?"
    );

    query.addBindValue(donor.getBloodGroup());
    query.addBindValue(donor.getAge());
    query.addBindValue(donor.getGender());
    query.addBindValue(donor.getCity());
    query.addBindValue(donor.getLastDonationDate());
    query.addBindValue(donor.isEligible() ? 1 : 0);
    query.addBindValue(donor.getUserId());

    if (!query.exec())
    {
        lastErrorMessage = query.lastError().text();
        db.rollback();
        return false;
    }

    if (!db.commit())
    {
        lastErrorMessage = db.lastError().text();
        db.rollback();
        return false;
    }

    return true;
}

bool DatabaseManager::updateRecipient(
    const Recipient& recipient)
{
    QSqlDatabase db = database();

    if (!db.transaction())
    {
        lastErrorMessage = db.lastError().text();
        return false;
    }

    if (!updateUserInfo(recipient))
    {
        db.rollback();
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "UPDATE recipients "
        "SET required_blood_group = ?, city = ? "
        "WHERE user_id = ?"
    );

    query.addBindValue(
        recipient.getRequiredBloodGroup()
    );

    query.addBindValue(
        recipient.getCity()
    );

    query.addBindValue(
        recipient.getUserId()
    );

    if (!query.exec())
    {
        lastErrorMessage = query.lastError().text();
        db.rollback();
        return false;
    }

    if (!db.commit())
    {
        lastErrorMessage = db.lastError().text();
        db.rollback();
        return false;
    }

    return true;
}

bool DatabaseManager::updateAdmin(
    const Admin& admin)
{
    return updateUserInfo(admin);
}

bool DatabaseManager::updatePassword(
    int userId,
    const QString& newPassword)
{
    QSqlQuery query(database());

    query.prepare(
        "UPDATE users "
        "SET password = ? "
        "WHERE user_id = ?"
    );

    query.addBindValue(
        hashPassword(newPassword)
    );

    query.addBindValue(userId);

    if (!query.exec())
    {
        lastErrorMessage = query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool DatabaseManager::updateDonorEligibility(
    int userId,
    bool eligible)
{
    QSqlQuery query(database());

    query.prepare(
        "UPDATE donors "
        "SET eligible = ? "
        "WHERE user_id = ?"
    );

    query.addBindValue(
        eligible ? 1 : 0
    );

    query.addBindValue(userId);

    if (!query.exec())
    {
        lastErrorMessage = query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool DatabaseManager::updateBloodRequestStatus(
    int requestId,
    const QString& status)
{
    QSqlQuery query(database());

    query.prepare(
        "UPDATE blood_requests "
        "SET status = ? "
        "WHERE request_id = ?"
    );

    query.addBindValue(status);
    query.addBindValue(requestId);

    if (!query.exec())
    {
        lastErrorMessage = query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

// ---------------------------------------------------------------
// Delete
// ---------------------------------------------------------------

bool DatabaseManager::deleteUser(
    int userId)
{
    QSqlQuery query(database());

    query.prepare(
        "DELETE FROM users "
        "WHERE user_id = ?"
    );

    query.addBindValue(userId);

    if (!query.exec())
    {
        lastErrorMessage = query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool DatabaseManager::deleteBloodRequest(
    int requestId)
{
    QSqlQuery query(database());

    query.prepare(
        "DELETE FROM blood_requests "
        "WHERE request_id = ?"
    );

    query.addBindValue(requestId);

    if (!query.exec())
    {
        lastErrorMessage = query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}