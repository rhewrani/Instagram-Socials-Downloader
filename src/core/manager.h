#ifndef MANAGER_H
#define MANAGER_H

#define _(key) (manager->t(key))

#include "fileagent.h"
#include "instagram.h"

#include <QObject>
#include <QApplication>
#include <QProcess>

#include <QtGui>


class MainWindow;

/**
 * @brief The Manager class acts as the central controller for the application.
 * 
 * It manages settings, profiles, media saving, and coordinates between the 
 * MainWindow and the Instagram API handler.
 */
class Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager(MainWindow* mainWindowRef, QObject *parent = nullptr);
    ~Manager();

    void debug(const QString &message);
    void log(const QString &message, bool MessageBox = false);
    void warning(const QString &message, bool MessageBox = true);
    void critical(const QString &message, bool MessageBox = true);

    QString t(const QString &key);

    QNetworkAccessManager networkManager;

    /**
     * @brief Structure to store application-wide settings.
     */
    struct appSettings {
        bool bEnableLogging = false;
        bool bOpenFileExplorerOnSave = true;
        bool bEnableQuoting = true;
        bool bAutoCopyText = true;
        bool bIsFirstOpen = true;
        int intLanguage = 0;
        QString strDownloadDir;
        QString strSessionid;
        QString strVersion;
        QMap<QString, QString> presets;

        appSettings() = default;

        appSettings(const appSettings &other) = default;
    };

    //  @brief Loads application settings from persistent storage (settings.ini).
    void GetSettings();

    //  @brief Retrieves a reference to the current application settings.
    appSettings& getSettingsStruct() { return settings; }

    //  @brief Saves the given settings to persistent storage.
    bool saveSettings(appSettings settings, bool restart);
    
    //  @brief Loads bookmarked user profiles from storage (profiles.json).
    void GetProfiles();

    //  @brief Retrieves a reference to the list of loaded user profiles.
    QList<Instagram::userData>& getProfiles() { return profiles; }

    //  @brief Saves the current list of profiles to storage.
    bool saveProfiles(QList<Instagram::userData> profiles);

    //  @brief Finds a profile in the loaded list by its username.
    Instagram::userData* getProfilePtrFromName(const QString &username);

    //  @brief Saves a pixmap (image) to a specified local path.
    bool saveMedia(const QPixmap &pixmap, const QString &path);

    //  @brief Downloads and saves a video from a URL to a local path.
    bool saveMediaVideo(const QString &videoUrl, const QString &path);

    //  @brief Asynchronously loads an image from a URL, using cache if available.
    void loadPixmap(const QString &url, const QString &id, int w, int h,
                    std::function<void(const QPixmap&)> callback);

    //  @brief Generates copy-paste text from a preset.
    void generateCopyPasteText(const QString &presetKey, QTextEdit *target, const QMap<QString, QString> &params);
    //  @brief Generates copy-paste text from a template.
    void generateCopyPasteTextString(const QString &templateText, QTextEdit *target, const QMap<QString, QString> &params, bool enableQuoting = true);

    //  @brief Returns a pointer to the Instagram handler.
    Instagram* getInstagram() { return instagram; }

    /* INSTAGRAM LOGIC */
    /* CURRENT SELECTED USER */
    void instagram_setCurrentSelectedUserIndex(int index) {
        instagram_currentSelectedProfileIndex = index;
        if (index >= 0 && index < profiles.size()) {
            currentUser = &profiles[index];
        } else {
            currentUser = nullptr;
        }
    }
    Instagram::userData* instagram_getCurrentSelectedUser() { return currentUser; }
    int instagram_getCurrentSelectedUserIndex() { return instagram_currentSelectedProfileIndex; }
    
    void instagram_GET_userInfo(const QString &username, bool isProfileChecker = false);
    void instagram_GET_userFeed(const QString &username);
    void instagram_GET_PostFromShortcode(const QString &shortcode);
    void instagram_GET_Story(const QString &username, bool isAutoFetch = true);

    
    QHash<QString, QPixmap> m_imageCache;
    QHash<QString, Instagram::contentNode> m_postCache;
    QHash<QString, Instagram::contentNode> m_storyCache;
    
    QList<Instagram::userData> profiles;
    
    int lastApiCall = 0;
    
    signals:

    private:
    MainWindow* mainWindow;
    FileAgent *fileAgent;
    Instagram *instagram;
    
    void Init();
    void InitInstagram();
    
    const QPixmap* getCachedPixmap(const QString &id) const;
    void cachePixmap(const QString &id, const QPixmap &pixmap);
    bool isPixmapCached(const QString &id) const;
    
    
    appSettings settings;
    Instagram::userData *currentUser = nullptr;
    int instagram_currentSelectedProfileIndex = 0;
};

#endif // MANAGER_H
