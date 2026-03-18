#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../core/manager.h"
#include "../dialogs/mediaviewerdialog.h"
#include "../dialogs/infodialog.h"
#include "../dialogs/profilewindow.h"
#include "../dialogs/blockingoverlay.h"
#include "settingswindow.h"
#include "clickablelabel.h"

enum ToastType { Info, Warning, Error };

namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class is the primary user interface of the application.
 * 
 * It manages the profile feed display, media preview, download triggers, 
 * and various settings/profile dialogs.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    //  @brief Updates the profile metadata (followers, avatar, etc.) in the UI.
    void updateProfileInfoUI(Instagram::userData *user, bool loadStory = false);

    //  @brief Updates the media feed list in the UI for the given user.
    void updateProfileFeedUI(Instagram::userData *user);
    //  @brief Populates the user selection combobox.
    void setProfileCombobox(const QList<Instagram::userData> &profiles, bool triggerIndexChanged = false, int selectedIndex = 0);

    //  @brief Displays detailed content for a selected media node.
    void displayNodeContent(Instagram::contentNode *node);

    //  @brief Toggles the visibility/state of the story button for a user.
    void toggleStoryButton(const QString &username);

    //  @brief Refreshes the generated template text (for captions).
    void updateGeneratedText();

    //  @brief Resets the preview widgets to their default empty state.
    void resetPreviewWidget();

    //  @brief Performs the initial profile load on app startup.
    void initialLoad();
    void loadEmpty();
    void resetProfileInfoUI();

    //  @brief Displays a non-intrusive toast notification to the user.
    void showToast(const QString &message, ToastType type = Info, int durationMs = 3000, bool isVideo = false);

    BlockingOverlay *overlay;

    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:


    void on_INST_CMBX_USER_currentIndexChanged(int index);

    void on_INST_LV_FEED_clicked(const QModelIndex &index);

    void on_INST_BTN_NP_clicked();

    void on_INST_BTN_RFSH_clicked();

    void on_INST_BTN_PLBK_clicked();

    void on_INST_BTN_DOWN_clicked();

    void on_INST_LV_POST_doubleClicked(const QModelIndex &index);

    void on_INST_BTN_SOUN_clicked();

    void on_INST_BTN_SELC_clicked();

    void on_INST_BTN_DSLC_clicked();

    void on_BTN_MGC_clicked();

    void on_BTN_SAVE_clicked();

    void on_MENU_OPEN_SETT_triggered();

    void on_INST_USER_STRY_clicked();

    void on_MENU_OPEN_INFO_triggered();

    void hideToast();

    void on_MENU_OPEN_PRFL_triggered();

private:
    Ui::MainWindow *ui;
    Manager *manager = nullptr;
    Settingswindow *settingswindow;
    ProfileWindow *profilewindow;
    MediaViewerDialog* mediaViewer;
    Infodialog *infodialog;

    FeedListModel* model;
    ChildMediaModel* cModel;
    QMediaPlayer *m_player;
    QVideoWidget *m_videoWidget;
    QAudioOutput *m_audioOutput;

    QClipboard *clipboard;

    ClickableLabel *INST_MDCT_PFP;
    ClickableLabel *INST_MDIA_PFP;

    Instagram::contentNode *currentSelectedNode = nullptr;
    QMap<QString, QString> currentParams;

    QString pendingShortcode;
    bool preventPlaybackLoop = false;

    QPoint m_dragStartPosition;
    bool m_isDragging = false;
    QWidget *titleBar = nullptr;

    QLabel *m_toastLabel = nullptr;
    QTimer *m_toastTimer = nullptr;

    void Init();
    void InitTitleBar();
    void InitUI();
    void InitLang();

    void setPfpImageFromURL(QLabel *target, QString &url, QString &id, int dimX, int dimY);
    void downloadImagesForFeed(const QList<Instagram::contentNode> &posts, int startRow);
    void downloadChildMediaImages(const QMap<int, Instagram::contentChild> &children);
    void generateCopyPasteText(const QString &presetKey, QTextEdit *target, const QMap<QString, QString> &params);
    void queueSaveMedia(const QMap<int, Instagram::contentChild> &map = {});
    void savePfp(const Instagram::contentChild *child);
    void openPfpViewer();
};

#endif // MAINWINDOW_H
