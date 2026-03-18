#ifndef FILEAGENT_H
#define FILEAGENT_H

#include "utils.h"

/**
 * @brief Helper class for reading and writing JSON files.
 */
class FileAgent : public QObject
{
    Q_OBJECT
public:
    explicit FileAgent(QObject *parent = nullptr);

    /** @brief Opens a file in the specified mode (Standard modes from Utils). */
    void File_Open(QFile &f, int mode = 0);

    /** @brief Reads a file's content as a JSON object. */
    QJsonObject File_GetDataObject(QFile &File);

    /** @brief Reads a file's content as a JSON document. */
    QJsonDocument File_GetDataDocument(QFile &File);

signals:


};

#endif // FILEAGENT_H
