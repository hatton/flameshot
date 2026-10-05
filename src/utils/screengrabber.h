// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2017-2019 Alejandro Sirgo Rica & Contributors

#pragma once

#include "utils/desktopinfo.h"

#include <QEvent>
#include <QList>
#include <QObject>
#include <QPixmap>
#include <QScreen>

class QEventLoop;
class QWidget;
class MonitorPreview;

class ScreenGrabber : public QObject
{
    Q_OBJECT
public:
    explicit ScreenGrabber(QObject* parent = nullptr);
    enum class PortalStatus
    {
        Success,
        Unavailable,
        Failed
    };
    QPixmap grabEntireDesktop(bool& ok, int preSelectedMonitor = -1);
    QPixmap grabFullDesktop(bool& ok);
    QRect screenGeometry(QScreen* screen);
    QPixmap grabScreen(QScreen* screenNumber, bool& ok);
    PortalStatus freeDesktopPortal(QPixmap& res, QString& errorDetail);
    QRect desktopGeometry();
    QRect logicalDesktopGeometry();
    int getSelectedMonitor() const { return m_selectedMonitor; }
    QScreen* getSelectedScreen() const;
    QPixmap selectMonitorAndCrop(const QPixmap& fullScreenshot, bool& ok);
#if defined(Q_OS_WIN)
    // Rectangles in Windows virtual-desktop pixels, unscaled by DPI
    static QRect nativeScreenRect(QScreen* screen);
    static QRect nativeDesktopRect();
#endif

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    void adjustDevicePixelRatio(QPixmap& pixmap);
    QWidget* createMonitorPreviews(const QPixmap& fullScreenshot);
    void cancelMonitorSelection();
    void moveHighlightedMonitorPreview(int offset);
    int previewIndexForMonitor(int monitorIndex) const;
    void selectHighlightedMonitorPreview();
    void selectMonitor(int monitorIndex);
    void setHighlightedMonitorPreview(int previewIndex);
    QPixmap cropToMonitor(const QPixmap& fullScreenshot, int monitorIndex);
    QPixmap windowsScreenshot(int wid);
#if defined(Q_OS_WIN)
    static void drawCursor(QPixmap& pixmap, const QPoint& nativeOrigin);
#endif
    QPixmap x11LegacyScreenshot();
    QPixmap unixScreenshot(bool& ok);

    DesktopInfo m_info;
    QPixmap Screenshot;
    int m_selectedMonitor;
    int m_highlightedMonitorPreview;
    QList<MonitorPreview*> m_monitorPreviews;
    QEventLoop* m_monitorSelectionLoop;
    bool m_userCancelled;
    static bool m_monitorSelectionActive;
};
