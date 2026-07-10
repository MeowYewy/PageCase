#pragma once

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>

class QNetworkAccessManager;
class QNetworkReply;

class UpdateChecker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool hasUpdate READ hasUpdate NOTIFY hasUpdateChanged)
    Q_PROPERTY(QString latestVersion READ latestVersion NOTIFY latestVersionChanged)
    Q_PROPERTY(QString downloadUrl READ downloadUrl NOTIFY downloadUrlChanged)
    Q_PROPERTY(int downloadProgress READ downloadProgress NOTIFY downloadProgressChanged)
    Q_PROPERTY(QVariantList changelog READ changelog CONSTANT)

public:
    enum Status {
        Idle = 0,
        Checking = 1,
        UpToDate = 2,
        UpdateAvailable = 3,
        CheckFailed = 4,
        Downloading = 5,
    };
    Q_ENUM(Status)

    explicit UpdateChecker(QObject *parent = nullptr);

    int status() const { return m_status; }
    bool hasUpdate() const { return m_hasUpdate; }
    QString latestVersion() const { return m_latestVersion; }
    QString downloadUrl() const { return m_downloadUrl; }
    int downloadProgress() const { return m_downloadProgress; }
    QVariantList changelog() const { return m_changelog; }

    Q_INVOKABLE void checkForUpdates();
    Q_INVOKABLE void downloadUpdate();
    Q_INVOKABLE int changelogEntryCount() const;
    Q_INVOKABLE QVariantMap changelogEntryAt(int index) const;

signals:
    void statusChanged();
    void hasUpdateChanged();
    void latestVersionChanged();
    void downloadUrlChanged();
    void downloadProgressChanged();

private:
    void setStatus(int status);
    void setHasUpdate(bool value);
    void parseUpdateManifest(const QByteArray &data);
    static int compareVersions(const QString &lhs, const QString &rhs);
    static QVariantList loadChangelog();
    QString localVersion() const;

    QNetworkAccessManager *m_network = nullptr;
    int m_status = Idle;
    bool m_hasUpdate = false;
    QString m_latestVersion;
    QString m_downloadUrl;
    QString m_silentInstallArgs;
    int m_downloadProgress = 0;
    QVariantList m_changelog;
    QNetworkReply *m_activeReply = nullptr;
};
