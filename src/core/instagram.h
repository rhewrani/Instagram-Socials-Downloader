#ifndef INSTAGRAM_H
#define INSTAGRAM_H

#include <QObject>
#include "fileagent.h"

/**
 * @brief The Instagram class handles interaction with the Instagram API.
 * 
 * This class provides methods to fetch user information, profile feeds, individual posts, and stories.
 * It manages network requests and parses JSON responses from Instagram's endpoints.
 */
class Instagram : public QObject
{
    Q_OBJECT
public:

    // @brief Represents a single child item within an Instagram post (e.g., in a sidecar).
    struct contentChild {
        QString type = "Image"; // Default for child
        QString mediaUrl;
        QString videoUrl;
        QString id;
        QString accessibilityCaption;
        QString story_timestamp;
        QString story_expires;
        int dimensionHeight;
        int dimensionWidth;
        int childIndex;
    };

    // @brief Represents a single node in an Instagram feed (a post, story, etc.).
    struct contentNode {
        QString shortcode;
        QString type = "Image"; // Default for feed node
        QString imageUrl;
        QString videoUrl;
        QString location;
        QString caption;
        QString accessibilityCaption;
        QString timestamp;
        QString id;
        QString foreignOwnerUsername; // foreign is used if post is fetched using link
        QString foreignOwnerFullname;
        QString foreignOwnerPfpUrl;
        QString foreignOwnerId;
        bool foreignOwnerIsVerified = false;
        bool isNew = false;
        int originalDimensionHeight = 0;
        int originalDimensionWidth = 0;
        int videoViewCount = 0;
        int likeCount = 0;
        int commentCount = 0;
        QMap<int, contentChild> children; // Post children, only if type is sidecar
    };

    // @brief Holds data for an Instagram user profile.
    struct userData {
        QString username;
        QString fullname;
        QString biography;
        QString profilePicUrl;
        QString endCursor; // Cursor for pagination
        QString id;
        int followersCount = 0;
        int postsCount = 0;
        int currentFeedIndex = 0;
        bool allowGetProfileInfo = true;
        bool allowGetProfileFeed = true;
        bool allowUpdateProfileInfoUI = true;
        bool allowUpdateProfileFeedUI = true;
        bool shouldFeedUIRefresh = true;
        bool hasNextPage = false;
        bool isVerified = false;
        QMap<int, contentNode> feed;
        QList<contentNode> appendFeed;

        void clear();
        void dump();
    };

    userData* getUserPtr(int userIndex);

    //@brief Retrieves detailed user information from Instagram.
    void GET_userInfo(userData *user, bool isProfileChecker = false);

    //@brief Fetches the recent feed posts for a user.
    void GET_userFeed(userData *user);

    //@brief Fetches a single post using its shortcode.
    void GET_post(const QString &shortcode, QHash<QString, contentNode> &hash);

    //@brief Fetches the latest stories for a user.
    void GET_story(const QString &username, QHash<QString, contentNode> &hash, bool isAutoFetch);

    QString t(const QString &key);

    explicit Instagram(FileAgent *fileAgentRef, QNetworkAccessManager &networkManagerRef, int lang, QString sessionid, userData *initialLoadUser, QObject *parent = nullptr);
signals:

    void signal_updateMainPageProfileInfo(Instagram::userData *user, bool loadStory);
    void signal_updateMainPageProfileFeed(Instagram::userData *user);
    void signal_postFetched(const QString &shortcode);
    void signal_storyFetched(const QString &username, bool isAutoFetch);
    void signal_fetchFailed();
    void signal_loadEmpty();
    void signal_profileCheckerReceivedInfo(Instagram::userData *user);

private:
    FileAgent *fileAgent;
    QNetworkAccessManager &networkManager; // Main networkManager used for the program
    // temporary networkManagers created used for feed and post fetch on each call
    // the reason for this is some kind of weird behavior with QNetworkAccessManager. It's only able to make one authenticated api call to instagrams api before failing
    // this is currently fixed by just creating a temporary QNetworkAccessManager for each call (which is automatically deleted afterwards) and works just as good
    // the issue is probably related with the connection protocol, both http1 and http2 being used, etc.

    void Init(userData *initialLoadUser);
    QJsonObject getObjectFromEntries(const QString& name, const QString& data);
    QJsonObject findReelsInObject(const QJsonObject &obj);
    QJsonObject findReelsInArray(const QJsonArray &arr);
    void generateSessionData(int isInit, userData *initialLoadUser);
    void setupHeaders(QNetworkRequest &request, int headerSet);
    void extractFeedData(QJsonArray &arr, userData* user);
    contentNode extractPostData(QJsonObject &obj);
    contentNode extractStoryData(const QJsonObject &obj);
    QJsonObject extractReelsMedia(const QString &htmlContent);
    bool checkResponse(QNetworkReply *reply, const QString &origin, int currentAttempt = 1);


    QString anonCookie;
    QString currentLsdToken;
    QString currentCsrfToken;
    QString appId = "936619743392459";
    // For each of these there is a max attempts cap at 1. They are placed where the respective function usually fails
    int postFetchAttempts = 0;
    int feedFetchAttempts = 0;
    int storyFetchAttempts = 0;
    int settingsLanguage = 0;
    QString settingsSessionid;

};

class FeedListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        IsVideoRole = Qt::UserRole + 1,
        IsNewRole
    };

    explicit FeedListModel(QObject *parent = nullptr);
    ~FeedListModel() override = default;

    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void clear();
    void setFeed(const QMap<int, Instagram::contentNode> &feed);
    void appendPosts(const QList<Instagram::contentNode> &newPosts);
    void setPixmapForRow(int row, const QPixmap &pixmap);
    bool hasPixmapForRow(int row) const;

private:
    QList<Instagram::contentNode> m_feed;
    QVector<QPixmap> m_pixmaps;
};


class ChildMediaModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        IsVideoRole = Qt::UserRole + 1
    };

    explicit ChildMediaModel(QObject *parent = nullptr);
    ~ChildMediaModel() override = default;

    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setChildren(const QMap<int, Instagram::contentChild> &children);
    void setPixmapForRow(int row, const QPixmap &pixmap);
    bool hasPixmapForRow(int row) const;

private:
    QList<Instagram::contentChild> m_children;
    QVector<QPixmap> m_pixmaps;
};


class FeedItemDelegate : public QStyledItemDelegate {
public:


    explicit FeedItemDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

class VideoOverlayDelegate : public QStyledItemDelegate
{
public:
    explicit VideoOverlayDelegate(QObject *parent = nullptr)
        : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // INSTAGRAM_H
