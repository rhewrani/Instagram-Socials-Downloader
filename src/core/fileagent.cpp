#include "fileagent.h"

FileAgent::FileAgent(QObject *parent)
    : QObject{parent}
{}

// Opens a file in the specified mode (ReadOnly by default).
void FileAgent::File_Open(QFile &f, int mode)
{
    auto device = QIODevice::ReadOnly;
    if (mode == 1) {
        device = QIODevice::WriteOnly;
    }

    if (!f.open(device)) {
        Logger::instance()->warning("ERROR: Failed to open file:" + f.errorString());
        return;
    }
    return;
}

// Reads a file's content and parses it as a QJsonObject.
QJsonObject FileAgent::File_GetDataObject(QFile &File)
{
    if (!File.exists()) {
        Logger::instance()->critical("ERROR: File not found: " + File.fileName());
        return {};
    }

    File_Open(File);

    QByteArray bytes = File.readAll();
    File.close();

    if (bytes.isEmpty()) {
        Logger::instance()->warning("ERROR: File is empty");
        return {};
    }

    QJsonParseError JsonError;
    QJsonDocument doc = QJsonDocument::fromJson(bytes, &JsonError);
    if (JsonError.error != QJsonParseError::NoError) {
        Logger::instance()->warning("ERROR: JSON Parse Error:" + JsonError.errorString());
        return {};
    }

    if (!doc.isObject()) {
        Logger::instance()->warning("ERROR: JSON is not an object");
        return {};
    }

    return doc.object();
}

// Reads a file's content and parses it as a QJsonDocument.
QJsonDocument FileAgent::File_GetDataDocument(QFile &File)
{
    if (!File.exists()) {
        Logger::instance()->critical("ERROR: File not found: " + File.fileName());
        return {};
    }

    File_Open(File);

    QByteArray bytes = File.readAll();
    File.close();

    if (bytes.isEmpty()) {
        Logger::instance()->warning("ERROR: File is empty");
        return {};
    }

    QJsonParseError JsonError;
    QJsonDocument doc = QJsonDocument::fromJson(bytes, &JsonError);
    if (JsonError.error != QJsonParseError::NoError) {
        Logger::instance()->warning("ERROR: JSON Parse Error:" + JsonError.errorString());
        return {};
    }


    return doc;
}
