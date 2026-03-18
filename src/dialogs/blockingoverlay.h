// BlockingOverlay.h
#ifndef BLOCKINGOVERLAY_H
#define BLOCKINGOVERLAY_H

#include <QDialog>
#include <QLabel>
#include "../core/utils.h"


/**
 * @brief The BlockingOverlay class provides a modal overlay to block user interaction while the application is starting up. 
 * 
 * It displays a message and prevents interacting with the main window until the overlay is dismissed.
 */
class BlockingOverlay : public QDialog
{
    Q_OBJECT

public:
    //  @brief Constructor: Creates a blocking overlay with the given parent and message.
    explicit BlockingOverlay(QWidget *parent = nullptr, const QString &text = "Loading…");

    //  @brief Updates the message displayed on the overlay.
    void setMessage(const QString &text);

    QLabel *m_label;

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
};

#endif // BLOCKINGOVERLAY_H
