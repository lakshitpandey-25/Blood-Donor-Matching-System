#include <QtTest>
#include <QStandardPaths>
#include <QDateTime>

#include "modules/database/DatabaseManager.h"
#include "models/Recipient.h"

class DatabaseManagerTest : public QObject
{
    Q_OBJECT

private slots:
    void testDuplicatePendingRequest()
    {
        QStandardPaths::setTestModeEnabled(true);

        DatabaseManager db;

        QVERIFY2(db.openDatabase(),
                 qPrintable(db.getLastError()));

        QVERIFY2(db.createTables(),
                 qPrintable(db.getLastError()));

        QString email =
            QString("duplicate-test-%1@example.invalid")
                .arg(QDateTime::currentMSecsSinceEpoch());

        Recipient recipient(
            0,
            "Test Recipient",
            "9999999999",
            email,
            "test123",
            "A+",
            "Delhi"
        );

        int recipientId = db.addRecipient(recipient);

        QVERIFY2(recipientId > 0,
                 qPrintable(db.getLastError()));

        int firstRequest =
            db.addBloodRequest(recipientId, "A+", "Delhi", 2);

        QVERIFY2(firstRequest > 0,
                 qPrintable(db.getLastError()));

        int duplicateRequest =
            db.addBloodRequest(recipientId, "A+", " delhi ", 2);

        QCOMPARE(duplicateRequest, -1);

        QVERIFY(db.getLastError().contains(
            "identical pending request",
            Qt::CaseInsensitive));

        int differentUnitsRequest =
            db.addBloodRequest(recipientId, "A+", "Delhi", 3);

        QVERIFY2(differentUnitsRequest > 0,
                 qPrintable(db.getLastError()));

        // Verify retrieval for this recipient only.
        QList<QVariantMap> allRequests = db.getAllBloodRequests();
        QList<QVariantMap> requests;

        for (const QVariantMap& request : allRequests)
        {
            if (request["recipientId"].toInt() == recipientId)
            {
                requests.append(request);
            }
        }

        QCOMPARE(requests.size(), 2);

        // Latest request should appear first.
        QCOMPARE(requests[0]["recipientName"].toString(),
                 QString("Test Recipient"));
        QCOMPARE(requests[0]["bloodGroup"].toString(), QString("A+"));
        QCOMPARE(requests[0]["city"].toString(), QString("Delhi"));
        QCOMPARE(requests[0]["units"].toInt(), 3);
        QCOMPARE(requests[0]["status"].toString(), QString("Pending"));

        // Original request should still be present.
        QCOMPARE(requests[1]["recipientId"].toInt(), recipientId);
        QCOMPARE(requests[1]["units"].toInt(), 2);
        QCOMPARE(requests[1]["status"].toString(), QString("Pending"));
    }
};

QTEST_GUILESS_MAIN(DatabaseManagerTest)

#include "DatabaseManagerTest.moc"
