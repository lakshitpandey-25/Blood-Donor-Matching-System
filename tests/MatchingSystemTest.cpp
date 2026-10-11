#include <QtTest>
#include "../modules/matching/MatchingSystem.h"

class MatchingSystemTest : public QObject
{
    Q_OBJECT

private slots:
    void testAllBloodGroupCompatibility();
    void testIncompatibleBloodGroup();
    void testCityMatching();
    void testEmptyRequestCity();
    void testEligibility();
    void testDonationCooldown();
    void testInvalidDonationDate();
    void testFutureDonationDate();
    void testNoMatchingDonors();
    void testNullDonor();
};

void MatchingSystemTest::testAllBloodGroupCompatibility()
{
    MatchingSystem system;

    // Donor blood group -> compatible recipient blood groups
    const QList<QString> groups = {
        "O-", "O+", "A-", "A+", "B-", "B+", "AB-", "AB+"
    };

    const QList<QList<QString>> compatible = {
        {"O-", "O+", "A-", "A+", "B-", "B+", "AB-", "AB+"},
        {"O+", "A+", "B+", "AB+"},
        {"A-", "A+", "AB-", "AB+"},
        {"A+", "AB+"},
        {"B-", "B+", "AB-", "AB+"},
        {"B+", "AB+"},
        {"AB-", "AB+"},
        {"AB+"}
    };

    for (int i = 0; i < groups.size(); ++i)
    {
        Donor donor(1, "Test", "123", "test@test.com", "pass",
                    groups[i], 25, "Male", "Delhi", "", true);

        QList<Donor*> donors = { &donor };

        for (const QString& recipientGroup : groups)
        {
            BloodRequest request(
                1, 2, recipientGroup, 1, "Hospital",
                "Delhi", QDate::currentDate(), "Pending"
            );

            const bool expected =
                compatible[i].contains(recipientGroup);

            QCOMPARE(
                !system.findMatchingDonors(request, donors).isEmpty(),
                expected
            );
        }
    }
}

void MatchingSystemTest::testIncompatibleBloodGroup()
{
    MatchingSystem system;

    Donor donor(1, "Test", "123", "test@test.com", "pass",
                "AB+", 25, "Male", "Delhi", "", true);

    BloodRequest request(1, 2, "O-", 1, "Hospital",
                         "Delhi", QDate::currentDate(), "Pending");

    QCOMPARE(
        system.findMatchingDonors(request, {&donor}).size(),
        0
    );
}

void MatchingSystemTest::testCityMatching()
{
    MatchingSystem system;

    Donor donor(1, "Test", "123", "test@test.com", "pass",
                "O-", 25, "Male", " Delhi ", "", true);

    BloodRequest request(1, 2, "O+", 1, "Hospital",
                         "delhi", QDate::currentDate(), "Pending");

    QCOMPARE(
        system.findMatchingDonors(request, {&donor}).size(),
        1
    );

    request.setCity("Mumbai");

    QCOMPARE(
        system.findMatchingDonors(request, {&donor}).size(),
        0
    );
}

void MatchingSystemTest::testEmptyRequestCity()
{
    MatchingSystem system;

    Donor donor(1, "Test", "123", "test@test.com", "pass",
                "O-", 25, "Male", "Delhi", "", true);

    BloodRequest request(1, 2, "O+", 1, "Hospital",
                         "", QDate::currentDate(), "Pending");

    QCOMPARE(
        system.findMatchingDonors(request, {&donor}).size(),
        1
    );
}

void MatchingSystemTest::testEligibility()
{
    MatchingSystem system;

    Donor donor(1, "Test", "123", "test@test.com", "pass",
                "O-", 25, "Male", "Delhi", "", false);

    BloodRequest request(1, 2, "O+", 1, "Hospital",
                         "Delhi", QDate::currentDate(), "Pending");

    QCOMPARE(
        system.findMatchingDonors(request, {&donor}).size(),
        0
    );
}

void MatchingSystemTest::testDonationCooldown()
{
    MatchingSystem system;

    Donor recent(1, "Recent", "123", "recent@test.com", "pass",
                 "O-", 25, "Male", "Delhi",
                 QDate::currentDate().addDays(-30).toString("yyyy-MM-dd"),
                 true);

    Donor exactly90(2, "Ninety", "456", "ninety@test.com", "pass",
                    "O-", 25, "Male", "Delhi",
                    QDate::currentDate().addDays(-90).toString("yyyy-MM-dd"),
                    true);

    BloodRequest request(1, 3, "O+", 1, "Hospital",
                         "Delhi", QDate::currentDate(), "Pending");

    QCOMPARE(system.findMatchingDonors(request, {&recent}).size(), 0);
    QCOMPARE(system.findMatchingDonors(request, {&exactly90}).size(), 1);
}

void MatchingSystemTest::testInvalidDonationDate()
{
    MatchingSystem system;

    Donor donor(1, "Test", "123", "test@test.com", "pass",
                "O-", 25, "Male", "Delhi", "invalid-date", true);

    BloodRequest request(1, 2, "O+", 1, "Hospital",
                         "Delhi", QDate::currentDate(), "Pending");

    QCOMPARE(
        system.findMatchingDonors(request, {&donor}).size(),
        0
    );
}

void MatchingSystemTest::testFutureDonationDate()
{
    MatchingSystem system;

    Donor donor(1, "Test", "123", "test@test.com", "pass",
                "O-", 25, "Male", "Delhi",
                QDate::currentDate().addDays(1).toString("yyyy-MM-dd"),
                true);

    BloodRequest request(1, 2, "O+", 1, "Hospital",
                         "Delhi", QDate::currentDate(), "Pending");

    QCOMPARE(
        system.findMatchingDonors(request, {&donor}).size(),
        0
    );
}

void MatchingSystemTest::testNoMatchingDonors()
{
    MatchingSystem system;

    Donor donor(1, "Test", "123", "test@test.com", "pass",
                "AB+", 25, "Male", "Delhi", "", true);

    BloodRequest request(1, 2, "O-", 1, "Hospital",
                         "Delhi", QDate::currentDate(), "Pending");

    QVERIFY(
        system.findMatchingDonors(request, {&donor}).isEmpty()
    );
}

void MatchingSystemTest::testNullDonor()
{
    MatchingSystem system;

    BloodRequest request(1, 2, "O+", 1, "Hospital",
                         "Delhi", QDate::currentDate(), "Pending");

    QList<Donor*> donors = { nullptr };

    QVERIFY(system.findMatchingDonors(request, donors).isEmpty());
}

QTEST_APPLESS_MAIN(MatchingSystemTest)

#include "MatchingSystemTest.moc"