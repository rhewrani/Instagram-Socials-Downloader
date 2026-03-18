#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include "../core/manager.h"

class Manager;

namespace Ui {
class Settingswindow;
}

/**
 * @brief The Settingswindow class provides a UI to configure application preferences.
 * 
 * Includes settings for language, download directory, session ID, and custom 
 * caption presets.
 */
class Settingswindow : public QDialog
{
    Q_OBJECT

public:
    explicit Settingswindow(Manager *managerRef, QWidget *parent = nullptr);
    ~Settingswindow();

    void closeEvent(QCloseEvent *event) override;

    //  @brief Shows the settings window and populates it with current values.
    void sw_show();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    //  @brief Emitted when UI-affecting settings (like language) are changed.
    void signal_updateTextMainWindow();

private slots:
    void on_CKBX_OFE_checkStateChanged(const Qt::CheckState &arg1);

    void on_CKBX_DQ_checkStateChanged(const Qt::CheckState &arg1);

    void on_CKBX_EAC_checkStateChanged(const Qt::CheckState &arg1);

    void on_CMBX_LANG_activated(int index);

    void on_BTN_TDF_OPEN_clicked();

    void on_LN_TGEN_INP_textChanged();

    void on_LN_TGEN_INP2_textChanged();

    void on_BTN_RESET_clicked();

    void on_BTN_SAVE_clicked();

    void on_LN_SSID_textChanged(const QString &arg1);

private:
    Ui::Settingswindow *ui;
    Manager *manager;

    Manager::appSettings &settings;
    Manager::appSettings editSettings;

    QPoint m_dragStartPosition;
    bool m_isDragging = false;
    QWidget *titleBar = nullptr;

    void Init();
    void InitTitleBar();
    void InitLang();

    void sw_setData();
    void checkVariables();

    bool settingsChanged = false;
    bool restartRequired = false;
    bool saveButtonUsed = false;

    QMap<QString, QString> params = {
        {"user", "J"},
        {"username", "jennierubyjane"},
        {"caption", "The Original Icon: SUPERSTAR @adidasoriginals #adidassuperstar"},
        {"link", "https://www.instagram.com/jennierubyjane/p/DVatnY_EYuI/"},
        {"likes", "3.1M"},
        {"comments", "50K"},
        {"views", "0"},
        {"timestamp", "2026-03-03"},
        {"location", "Los Angeles, USA"},
        {"biography", "@lesyeuxdenini"},
        {"followers", "89.7M"},
        {"posts_count", "1,355"}
    };

    QMap<QString, QString> paramsStory = {
        {"user", "J"},
        {"username", "jennierubyjane"},
        {"link", "https://www.instagram.com/stories/jennierubyjane/"},
        {"biography", "@lesyeuxdenini"},
        {"followers", "89.7M"},
        {"posts_count", "1,355"}
    };
};

#endif // SETTINGSWINDOW_H
