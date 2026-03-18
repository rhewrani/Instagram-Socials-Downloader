#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QObject>
#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QAbstractItemModel>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QPainterPath>
#include <QIcon>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>
#include <QLayout>
#include <QDir>
#include <QSaveFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QPushButton>
#include <QtNetwork>
#include <QtMath>
#include <QLabel>
#include <QTextEdit>
#include <QDesktopServices>

#include "lang.h"

enum class InstagramLinkError {
    NoError,
    InvalidUrl,
    NotInstagram,
    Story,
    ProfileLink,
    InvalidPath
};

enum Position { Left, Right };

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Critical
};

//  @brief Returns a date format string for the given index.
QString getDateFormat(int format, bool backupFormat = false);

//  @brief Translates a UI key using the current application language.
QString translate(const QString &key, int lang);

//  @brief Formats a large integer with thousands separators.
QString formatNumber(int number);

//  @brief Formats a Unix timestamp into a readable date string with ordinal day (e.g. 1st, 2nd).
QString formatTimestampWithOrdinal(qint64 timestamp);

//  @brief Adjusts a label's font size to fit the given text.
void fitTextToLabel(QLabel *target, const QString &text);

//  @brief Parses an Instagram URL to extract the media shortcode.
QString extractInstagramShortcode(const QString &url, InstagramLinkError &error);

//  @brief Extracts the username from an Instagram story URL.
QString extractUsernameFromStoriesUrl(const QString &url);

//  @brief Extracts a full name from a caption string.
QString extractFullnameFromACPT(const QString &acpt);

//  @brief Helper to position a pixmap/icon label relative to a text label.
void setPixmapToText(QLabel *textLabel, QLabel *pixmapLabel, Position position, int offset = 20, bool adjustY = false);

//  @brief Adjusts a button's font size to fit its label text.
void fitTextToButton(QPushButton *button, const QString &text);

//  @brief Reflows a text block with a prefix on every line (e.g., for Discord quotes).
QString reflowTextWithPrefix(const QString &text, const QString &prefix, const QFontMetrics &fm, int maxWidth);

//  @brief Sets a label's text while optionally prepending or appending emojis.
void setLabelTextWithEmoji(QLabel *label, const QString &text,
                           const QString &emojiBefore = QString(),
                           const QString &emojiAfter = QString());


//  @brief A singleton class for logging messages to a file and/or message boxes.
class Logger : public QObject
{
    Q_OBJECT
public:
    static Logger* instance();

    //  @brief Configures the default parent widget for message boxes and date format.
    void setParentWidget(QWidget *parent, int dateFormat);

    //  @brief Logs an informational message.
    void log(const QString &message, bool MB = false, QWidget *parent = nullptr);

    //  @brief Logs a warning message.
    void warning(const QString &message, bool MB = true, QWidget *parent = nullptr);

    //  @brief Logs a critical error message.
    void critical(const QString &message, bool MB = true, QWidget *parent = nullptr);

    //  @brief Logs a debug message (always to file, never to UI).
    void debug(const QString &message);
private:
    QFile logFile;
    QTextStream logStream;
    QWidget *messageParent = nullptr;
    Logger();

    void logMessage(LogLevel level, const QString &message, bool MessageBox, QWidget *parent = nullptr);
    QString levelToString(LogLevel level);

    int intDateFormat = 0;
};


#endif // UTILS_H
