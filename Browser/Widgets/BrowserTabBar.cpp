#include "BrowserTabBar.h"

#include <QIcon>
#include <QLabel>
#include <QToolButton>

//TODO: Context menu for tabBar

BrowserTabBar::BrowserTabBar(QWidget *parent) :
    QTabBar(parent)
{
    setExpanding(false);
    setTabsClosable(true);
    setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);
    setMovable(true);
    setElideMode(Qt::ElideRight);

    // Add "New Tab" button
    m_buttonNewTab = new QToolButton(this);
    m_buttonNewTab->setIcon(QIcon::fromTheme("folder-new"));
    m_buttonNewTab->setStyleSheet("QToolButton:hover { border: 1px solid #666666; border-radius: 2px; } ");
    int addTabIdx = addTab(QString());
    setTabButton(addTabIdx, QTabBar::RightSide, m_buttonNewTab);
    setTabToolTip(addTabIdx, tr("New Tab"));
    setTabEnabled(addTabIdx, false);

    setStyleSheet("QTabBar::tab:disabled { background-color: rgba(0, 0, 0, 0); }");

    connect(m_buttonNewTab, &QToolButton::clicked, this, &BrowserTabBar::newTabRequest);
}

QSize BrowserTabBar::tabSizeHint(int index) const
{
    // Special size for "New Tab" pseudo tab
    if (index + 1 == count())
    {
        QSize newTabHint = m_buttonNewTab->sizeHint();
        newTabHint.setWidth(newTabHint.width() * 3 / 2);
        return newTabHint;
    }

    // Get the QTabBar size hint and keep width within an upper bound
    QSize hint = QTabBar::tabSizeHint(index);
    if (count() > 3)
    {
        QFontMetrics fMetric = fontMetrics();
        return hint.boundedTo(QSize(fMetric.width("R") * 20, hint.height()));
    }
    return hint;
}
