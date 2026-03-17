#include "../gui/mainwindow.h"
#include "gui/mainwindow.h"
#include "instagram.h"
#include <qdebug.h>
#include <qjsonobject.h>

Manager::Manager(MainWindow *mainWindowRef, QObject *parent)
    : QObject{parent}
    , mainWindow{mainWindowRef}
{
    fileAgent = new FileAgent();

    Init();

}

Manager::~Manager()
{

}

void Manager::debug(const QString &message)
{
    qDebug() << message;
    Logger::instance()->debug(message);
}

void Manager::log(const QString &message, bool MessageBox)
{
    if (settings.bEnableLogging) {
        Logger::instance()->log(message, MessageBox);
    }
}

void Manager::warning(const QString &message, bool MessageBox)
{
    Logger::instance()->warning(message, MessageBox);
}

void Manager::critical(const QString &message, bool MessageBox)
{
    Logger::instance()->critical(message, MessageBox);
}

QString Manager::t(const QString &key)
{
    
    return translate(key, settings.intLanguage);
}

bool Manager::saveProfiles(QList<Instagram::userData> profilesEdit)
{
    for (int i = 0; i < profilesEdit.size(); ++i) {
        for (const auto &oldProfile : profiles) {
            if (profilesEdit[i].username == oldProfile.username) {
                profilesEdit[i] = oldProfile;
                break;
            }
        }
    }

    QString selectedUsername = currentUser ? currentUser->username : QString();
    profiles = profilesEdit;
    currentUser = nullptr;

    for (int i = 0; i < profiles.size(); ++i) {
        if (profiles[i].username == selectedUsername) {
            currentUser = &profiles[i];
            instagram_currentSelectedProfileIndex = i;
            break;
        }
    }

    QFile file("profiles.json");

    QJsonDocument doc = fileAgent->File_GetDataDocument(file);
    QJsonObject root = doc.object();

    QJsonArray profilesArray;
    for (const auto &userData : profilesEdit) {
        QJsonObject profileObj;
        profileObj["id"] = userData.id;
        profileObj["username"] = userData.username;
        profileObj["fullname"] = userData.fullname;
        profilesArray.append(profileObj);
    }
    root["profiles"] = profilesArray;
    doc.setObject(root);
    fileAgent->File_Open(file, 1);
    file.write(doc.toJson(QJsonDocument::JsonFormat::Indented));
    file.close();
    return true;
}

Instagram::userData* Manager::getProfilePtrFromName(const QString &username)
{
    for (auto &profile : profiles) {
        if (profile.username == username) {
            return &profile;
        }
    }
    return nullptr;
}

bool Manager::saveSettings(appSettings settingsStruct, bool restart)
{
    if (!restart) {
        // Only non-restart required settings here
        settings.presets = settingsStruct.presets;
        settings.bEnableLogging = settingsStruct.bEnableLogging;
        settings.bOpenFileExplorerOnSave = settingsStruct.bOpenFileExplorerOnSave;
        settings.bEnableQuoting = settingsStruct.bEnableQuoting;
        settings.bAutoCopyText = settingsStruct.bAutoCopyText;
        settings.strDownloadDir = settingsStruct.strDownloadDir;
    }

    QFile file("settings.json");

    QJsonDocument doc = fileAgent->File_GetDataDocument(file);
    QJsonObject root = doc.object();

    QJsonObject appObj = root.value("app").toObject();
    QJsonObject userObj = root.value("user").toObject();
    QJsonObject featuresObj = root.value("features").toObject();
    QJsonObject presetsObj = root.value("text_presets").toObject();
    QJsonObject dataObj = root.value("data").toObject();


    root["app"] = appObj;

    userObj["language"] = settingsStruct.intLanguage;
    root["user"] = userObj;

    featuresObj["enableLogging"] = settingsStruct.bEnableLogging;
    featuresObj["openFileExporerOnSave"] = settingsStruct.bOpenFileExplorerOnSave;
    featuresObj["enableQuoting"] = settingsStruct.bEnableQuoting;
    featuresObj["autoCopyText"] = settingsStruct.bAutoCopyText;
    featuresObj["isFirstOpen"] = settingsStruct.bIsFirstOpen;
    root["features"] = featuresObj;


    QJsonObject setPresets;
    for (auto it = settingsStruct.presets.begin(); it != settingsStruct.presets.end(); ++it) {
        setPresets[it.key()] = it.value();
    }
    root["text_presets"] = setPresets;

    dataObj["downloadDir"] = settingsStruct.strDownloadDir;
    if (settings.strSessionid != settingsStruct.strSessionid) {
         dataObj["sessionid"] = settingsStruct.strSessionid;
    }
    root["data"] = dataObj;


    doc.setObject(root);
    fileAgent->File_Open(file, 1);
    file.write(doc.toJson(QJsonDocument::JsonFormat::Indented));
    file.close();

    log("Saved settings");

    if (restart) {
        log("Quitting application");
        log("Initiating application restart");

        QApplication::quit();
        return false;
    }
    return true;
}


bool Manager::saveMedia(const QPixmap &pixmap, const QString &path)
{
    if (pixmap.isNull()) return false;

    QString appDir = QCoreApplication::applicationDirPath();
    QString fullPath = appDir + "/" + path;
    if (!settings.strDownloadDir.isEmpty()) {

        fullPath = path;
    }

    QFileInfo fileInfo(fullPath);
    QDir().mkpath(fileInfo.absolutePath());

    return pixmap.save(fullPath, "JPG", 100);

}

bool Manager::saveMediaVideo(const QString &videoUrl, const QString &path)
{
    if (videoUrl.isEmpty() || path.isEmpty()) return false;

    QString appDir = QCoreApplication::applicationDirPath();
    QString fullPath = appDir + "/" + path;
    if (!settings.strDownloadDir.isEmpty()) {

        fullPath = path;
    }

    QFileInfo fileInfo(fullPath);
    QDir().mkpath(fileInfo.absolutePath());

    QNetworkRequest request((QUrl(videoUrl)));
    QNetworkReply *reply = networkManager.get(request);

    QObject::connect(reply, &QNetworkReply::finished, [reply, fullPath]() {
        if (reply->error() == QNetworkReply::NoError) {
            QFile file(fullPath);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(reply->readAll());
                file.close();
            }
        } else {
            qDebug() << "Video download error:" << reply->errorString();
        }
        reply->deleteLater();
    });
    return true;
}

void Manager::loadPixmap(const QString &url, const QString &id, int w, int h,
                         std::function<void(const QPixmap&)> callback)
{
    if (m_imageCache.contains(id)) {
        callback(m_imageCache[id]);
        return;
    }

    QNetworkRequest req(url);
    QNetworkReply *reply = networkManager.get(req);

    connect(reply, &QNetworkReply::finished, this,
            [this, reply, id, w, h, callback = std::move(callback)]() mutable {
                reply->deleteLater();
                if (reply->error() != QNetworkReply::NoError) return;

                QPixmap pixmap;
                if (pixmap.loadFromData(reply->readAll())) {
                    QPixmap scaled = pixmap.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                    m_imageCache[id] = scaled;
                    callback(scaled);
                }
            });
}

void Manager::generateCopyPasteText(const QString &presetKey, QTextEdit *target, const QMap<QString, QString> &params)
{
    if (!target) return;

    QString templateText = settings.presets.value(presetKey);
    if (templateText.isEmpty()) {
        target->setPlainText("");
        return;
    }

    QString result = templateText;
    for (auto it = params.begin(); it != params.end(); ++it) {
        QString placeholder = "{" + it.key() + "}";
        result.replace(placeholder, it.value());
    }

    if (params.contains("caption") && settings.bEnableQuoting) {
        QString caption = params.value("caption");
        if (!caption.isEmpty()) {
            QFontMetrics fm(target->font());
            int maxWidth = target->viewport()->width() - target->document()->documentMargin() * 2;

            QString quotedCaption = reflowTextWithPrefix(caption, "> ", fm, maxWidth);

            result.replace(params.value("caption"), quotedCaption);
        }
    }

    target->setPlainText(result);
}

void Manager::generateCopyPasteTextString(const QString &templateText, QTextEdit *target, const QMap<QString, QString> &params, bool enableQuoting)
{
    if (!target || templateText.isEmpty()) return;

    QString result = templateText;

    for (auto it = params.begin(); it != params.end(); ++it) {
        QString placeholder = "{" + it.key() + "}";
        result.replace(placeholder, it.value());
    }

    if (params.contains("caption") && enableQuoting) {
        QString caption = params.value("caption");
        if (!caption.isEmpty()) {
            QFontMetrics fm(target->font());
            int maxWidth = 369 - target->document()->documentMargin() * 2; // 369 hardcoded since it's much smaller (not optimal) before being shown, resulting the result texted being not correctly formatted
            QString quotedCaption = reflowTextWithPrefix(caption, "> ", fm, maxWidth);
            result.replace(params.value("caption"), quotedCaption);
        }
    }

    target->setPlainText(result);
}



void Manager::instagram_GET_userInfo(const QString &username, bool isProfileChecker) // by name
{
    Instagram::userData *user;
    if (isProfileChecker) {
        user = new Instagram::userData();
        user->username = username;
    } else {
        user = getProfilePtrFromName(username);
    }
    instagram->GET_userInfo(user, isProfileChecker);
}

void Manager::instagram_GET_userFeed(const QString &username)
{
    instagram->GET_userFeed(getProfilePtrFromName(username));
}

void Manager::instagram_GET_PostFromShortcode(const QString &shortcode)
{
    if (m_postCache.contains(shortcode)) {

        mainWindow->displayNodeContent(&m_postCache[shortcode]);
        return;
    }
    instagram->GET_post(shortcode, m_postCache);
}

void Manager::instagram_GET_Story(const QString &username, bool isAutoFetch)
{
    if (m_storyCache.contains(username) && !isAutoFetch) {

        mainWindow->displayNodeContent(&m_storyCache[username]);
        return;
    }
    instagram->GET_story(username, m_storyCache, isAutoFetch);
}


void Manager::Init()
{
    GetSettings();
    GetProfiles();
    InitInstagram();
    mainWindow->setProfileCombobox(profiles, false);
}

void Manager::InitInstagram()
{
    if (!profiles.isEmpty()) {
        currentUser = &profiles[0];
    }
    instagram = new Instagram(fileAgent, networkManager, settings.intLanguage, settings.strSessionid, currentUser);
    connect(instagram, &Instagram::signal_updateMainPageProfileInfo, mainWindow, &MainWindow::updateProfileInfoUI);
    connect(instagram, &Instagram::signal_updateMainPageProfileFeed, mainWindow, &MainWindow::updateProfileFeedUI);
    connect(instagram, &Instagram::signal_loadEmpty, mainWindow, &MainWindow::loadEmpty);
    connect(instagram, &Instagram::signal_fetchFailed, mainWindow, &MainWindow::resetPreviewWidget);
    connect(instagram, &Instagram::signal_postFetched, this, [this](const QString &shortcode) {
        if (m_postCache.contains(shortcode)) {
            mainWindow->displayNodeContent(&m_postCache[shortcode]);
        }
    });
    connect(instagram, &Instagram::signal_storyFetched, this, [this](const QString &username, bool isAutoFetch) {
        qDebug() << "signal_storyFetched" << username << isAutoFetch;
        if (m_storyCache.contains(username)) {

            if (!isAutoFetch) {
                mainWindow->displayNodeContent(&m_storyCache[username]);
            }
            qDebug() << "toggleStoryButton" << username;
            mainWindow->toggleStoryButton(username);

        }
    });
}

const QPixmap* Manager::getCachedPixmap(const QString &id) const
{
    auto it = m_imageCache.find(id);
    return (it != m_imageCache.end()) ? &it.value() : nullptr;
}

void Manager::cachePixmap(const QString &id, const QPixmap &pixmap)
{
    if (!id.isEmpty() && !pixmap.isNull())
        m_imageCache[id] = pixmap;
}

bool Manager::isPixmapCached(const QString &id) const
{
    return m_imageCache.contains(id);
}

void Manager::GetProfiles()
{
    QFile file("profiles.json");
    QJsonDocument doc = fileAgent->File_GetDataDocument(file);
    QJsonObject root = doc.object();
    
    QList<Instagram::userData> loadedProfiles;
    
    if (root.value("profiles").isArray()) {
        QJsonArray profilesArray = root.value("profiles").toArray();
        for (const auto &val : profilesArray) {
            QJsonObject profileObj = val.toObject();
            Instagram::userData userData;
            userData.id = profileObj.value("id").toString();
            userData.username = profileObj.value("username").toString();
            userData.fullname = profileObj.value("fullname").toString();
            loadedProfiles.append(userData);
            log("Loaded profile: " + userData.id + " - " + userData.username + " - " + userData.fullname);
        }
    } else {
        // Fallback for old object format
        QJsonObject profilesObj = root.value("profiles").toObject();
        for (auto it = profilesObj.begin(); it != profilesObj.end(); ++it) {
            QJsonObject profileObj = it.value().toObject();
            Instagram::userData userData;
            userData.id = it.key();
            userData.username = profileObj.value("username").toString();
            userData.fullname = profileObj.value("fullname").toString();
            loadedProfiles.append(userData);
            log("Loaded profile: " + userData.id + " - " + userData.username + " - " + userData.fullname);
        }
    }
    profiles = loadedProfiles;
}


void Manager::GetSettings()
{
    QFile file("settings.json");

    QJsonDocument doc = fileAgent->File_GetDataDocument(file);

    QJsonObject settingsRoot = doc.object();

    QJsonObject appObj = settingsRoot.value("app").toObject();
    QJsonObject userObj = settingsRoot.value("user").toObject();
    QJsonObject featuresObj = settingsRoot.value("features").toObject();
    QJsonObject presetsObj = settingsRoot.value("text_presets").toObject();
    QJsonObject dataObj = settingsRoot.value("data").toObject();

    settings.bOpenFileExplorerOnSave = featuresObj.value("openFileExporerOnSave").toBool();
    settings.bEnableQuoting = featuresObj.value("enableQuoting").toBool();
    settings.bAutoCopyText = featuresObj.value("autoCopyText").toBool();
    settings.bIsFirstOpen = featuresObj.value("isFirstOpen").toBool();

    settings.intLanguage = userObj.value("language").toInt();

    settings.strDownloadDir = dataObj.value("downloadDir").toString();
    QString sessionid = dataObj.value("sessionid").toString();
    if (sessionid.isEmpty() || sessionid == "YOUR_SESSION_ID") {
        settings.strSessionid = "";
    } else {
        settings.strSessionid = sessionid;
    }

    settings.strVersion = appObj.value("version").toString();

    QMap<QString, QString> textPresets;
    for (auto it = presetsObj.begin(); it != presetsObj.end(); ++it) {
        textPresets[it.key()] = it.value().toString();
    }
    settings.presets = textPresets;

    if (featuresObj.value("enableLogging").toBool()) {
        settings.bEnableLogging = featuresObj.value("enableLogging").toBool();
        log("LOGGING: Enabled");
    }
}
