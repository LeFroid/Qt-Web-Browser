#include "Settings.h"

#include "BrowserApplication.h"
#include <QDir>
#include <QFileInfo>
#include <QWebSettings>

Settings::Settings() :
    m_firstRun(false),
    m_settings(),
    m_storagePath()
{
    // Check if defaults need to be set
    if (!m_settings.contains("StoragePath"))
        setDefaults();

    m_storagePath = m_settings.value("StoragePath").toString();
}

QString Settings::getPathValue(const QString &key)
{
    return m_storagePath + m_settings.value(key).toString();
}

QVariant Settings::getValue(const QString &key)
{
    return m_settings.value(key);
}

void Settings::setValue(const QString &key, const QVariant &value)
{
    m_settings.setValue(key, value);
}

bool Settings::firstRun() const
{
    return m_firstRun;
}

void Settings::setDefaults()
{
    m_firstRun = true;

    QString storageFolder = QFileInfo(m_settings.fileName()).absoluteDir().absolutePath() + QDir::separator();
    m_settings.setValue("StoragePath", storageFolder);
    m_settings.setValue("BookmarkPath", "bookmarks.db");
    m_settings.setValue("CookiePath", "cookies.db");
    m_settings.setValue("HistoryPath", "history.db");
    m_settings.setValue("FaviconPath", "favicons.db");
    m_settings.setValue("AdBlockFile", "hosts_adblock.txt");
    m_settings.setValue("UserAgentsFile",  "user_agents.json");
    m_settings.setValue("SearchEnginesFile", "search_engines.json");
    m_settings.setValue("SessionFile", "last_session.json");
    m_settings.setValue("UserScriptsDir", "UserScripts");
    m_settings.setValue("UserScriptsConfig", "user_scripts.json");
    m_settings.setValue("AdBlockPlusConfig", "adblock_plus.json");
    m_settings.setValue("AdBlockPlusDataDir", "AdBlockPlus");

    m_settings.setValue("HomePage", "https://www.ixquick.com/");
    m_settings.setValue("StartupMode", QVariant::fromValue(StartupMode::LoadHomePage));
    m_settings.setValue("NewTabsLoadHomePage", true);
    m_settings.setValue("DownloadDir", QDir::homePath() + QDir::separator() + "Downloads");
    m_settings.setValue("AskWhereToSaveDownloads", false);
    m_settings.setValue("EnableJavascript", true);
    m_settings.setValue("EnableJavascriptPopups", false);
    m_settings.setValue("AutoLoadImages", true);
    m_settings.setValue("EnablePlugins", false);
    m_settings.setValue("EnableCookies", true);
    m_settings.setValue("EnableXSSAudit", true);
    m_settings.setValue("EnableBookmarkBar", false);
    m_settings.setValue("CustomUserAgent", false);
    m_settings.setValue("UserScriptsEnabled", true);
    m_settings.setValue("AdBlockPlusEnabled", true);

    QWebSettings *webSettings = QWebSettings::globalSettings();
    m_settings.setValue("StandardFont", webSettings->fontFamily(QWebSettings::StandardFont));
    m_settings.setValue("SerifFont", webSettings->fontFamily(QWebSettings::SerifFont));
    m_settings.setValue("SansSerifFont", webSettings->fontFamily(QWebSettings::SansSerifFont));
    m_settings.setValue("CursiveFont", webSettings->fontFamily(QWebSettings::CursiveFont));
    m_settings.setValue("FantasyFont", webSettings->fontFamily(QWebSettings::FantasyFont));
    m_settings.setValue("FixedFont", webSettings->fontFamily(QWebSettings::FixedFont));

    m_settings.setValue("StandardFontSize", webSettings->fontSize(QWebSettings::DefaultFontSize));
    m_settings.setValue("FixedFontSize", webSettings->fontSize(QWebSettings::DefaultFixedFontSize));

    // Todo: Store settings such as whether or not to store history
}
