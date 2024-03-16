#pragma once
#include <qsystemtrayicon.h>
#include "MacroUi.h"
#include "SettingsUi.h"
#include "AboutUi.h"
#include "ui_MainUi.h"
#include "../static.h"

class MainUi : public QMainWindow
{
    Q_OBJECT;
    Ui::MainUiClass ui;
    QSystemTrayIcon* tray = 0;

    QPoint msPos;

public:
    MainUi()
    {
        setWindowFlags(Qt::FramelessWindowHint);
        ui.setupUi(this);
        WidInit();
    }

private:
    void WidInit()
    {
        {
            tray = new QSystemTrayIcon(this);
            tray->setIcon(QIcon(":/icon.png"));
            tray->show();

            Global::ir.mcui = new MacroUi(ui.tabRecord);
            Global::ir.stui = new SettingsUi(ui.tabSettings);
            Global::ir.abui = new AboutUi(ui.tabAbout);
        }
        {
            connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(OnTrayClick(QSystemTrayIcon::ActivationReason)));
            connect(ui.bnClose, SIGNAL(clicked()), this, SLOT(OnTitleClose()));
            connect(ui.bnHide, SIGNAL(clicked()), this, SLOT(OnTitleHide()));
            connect(ui.bnMin, SIGNAL(clicked()), this, SLOT(OnTitleMin()));
        }
    }

    // Move
    void mousePressEvent(QMouseEvent* et) { if (et->buttons() & Qt::LeftButton) msPos = et->pos(); }
    void mouseMoveEvent(QMouseEvent* et) { if (et->buttons() & Qt::LeftButton) move(et->pos() + pos() - msPos); }

private slots:
    void OnTrayClick(QSystemTrayIcon::ActivationReason reason) { if (reason == QSystemTrayIcon::Trigger) setWindowState(Qt::WindowNoState), show(), Window::Top((HWND)winId()); }
    void OnTitleClose() { exit(0); }
    void OnTitleHide() { hide(); }
    void OnTitleMin() { setWindowState(Qt::WindowMinimized); }
};
