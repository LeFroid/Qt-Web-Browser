#include "AdBlockSubscription.h"
#include "AdBlockFilterParser.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace adblock
{

Subscription::Subscription() :
    m_enabled(true),
    m_filePath(),
    m_name(),
    m_sourceUrl(),
    m_lastUpdate(),
    m_nextUpdate(),
    m_filters()
{
}

Subscription::Subscription(const QString &dataFile) :
    m_enabled(true),
    m_filePath(dataFile),
    m_name(),
    m_sourceUrl(),
    m_lastUpdate(),
    m_nextUpdate(),
    m_filters()
{
}

Subscription::Subscription(Subscription &&other) noexcept :
    m_enabled(other.m_enabled),
    m_filePath(other.m_filePath),
    m_name(other.m_name),
    m_sourceUrl(other.m_sourceUrl),
    m_lastUpdate(other.m_lastUpdate),
    m_nextUpdate(other.m_nextUpdate),
    m_filters(std::move(other.m_filters))
{
}

Subscription &Subscription::operator =(Subscription &&other) noexcept
{
    if (this != &other)
    {
        m_enabled = other.m_enabled;
        m_filePath = other.m_filePath;
        m_name = other.m_name;
        m_sourceUrl = other.m_sourceUrl;
        m_lastUpdate = other.m_lastUpdate;
        m_nextUpdate = other.m_nextUpdate;
        m_filters = std::move(other.m_filters);
    }

    return *this;
}

Subscription::~Subscription()
{
}

bool Subscription::isEnabled() const
{
    return m_enabled;
}

void Subscription::setEnabled(bool value)
{
    m_enabled = value;
}

const QString &Subscription::getName() const
{
    return m_name;
}

const QUrl &Subscription::getSourceUrl() const
{
    return m_sourceUrl;
}

const QDateTime &Subscription::getLastUpdate() const
{
    return m_lastUpdate;
}

const QDateTime &Subscription::getNextUpdate() const
{
    return m_nextUpdate;
}

void Subscription::load(AdBlockManager *adBlockManager)
{
    if (!m_enabled || m_filePath.isEmpty())
        return;

    // Load subscription file
    QFile subFile(m_filePath);
    if (!subFile.exists() || !subFile.open(QIODevice::ReadOnly))
        return;

    m_filters.clear();

    FilterParser parser(adBlockManager);

    QString line;
    QTextStream stream(&subFile);
    while (stream.readLineInto(&line))
    {
        // Check for metadata
        if (line.startsWith(QChar('!')))
        {
            // Subscription name
            if (m_name.isEmpty())
            {
                int titleIdx = line.indexOf(QStringLiteral("Title:"));
                if (titleIdx > 0)
                    m_name = line.mid(titleIdx + 7);
            }

            // Check for next update
            int expireIdx = line.indexOf(QStringLiteral("! Expires:")), numDaysIdx = line.indexOf(QStringLiteral(" day"));
            if (expireIdx >= 0 && numDaysIdx > 0)
            {
                // Update string is in format "! Expires: x days" Try extracting x and converting to integer
                QString numDayStr = line.mid(10, numDaysIdx - 10).trimmed();
                bool ok;
                int numDays = numDayStr.toInt(&ok, 10);
                if (!ok || numDays == 0)
                    continue;

                // Add the number of days to the last update and set as next update
                QDateTime updateDate = getLastUpdate();
                m_nextUpdate = updateDate.addDays(numDays);
            }

            continue;
        }
        else if (line.isEmpty() || line.compare(QStringLiteral("#")) == 0 || line.startsWith(QStringLiteral("# ")) || line.startsWith(QStringLiteral("[Adblock")))
            continue;

        m_filters.push_back(parser.makeFilter(line));
    }

    // Set name to filename if it was not specified in data region of file
    if (m_name.isEmpty())
    {
        int sepIdx = m_filePath.lastIndexOf(QDir::separator());
        m_name = m_filePath.mid(sepIdx + 1);
    }
}

void Subscription::setLastUpdate(const QDateTime &date)
{
    m_lastUpdate = date;
}

void Subscription::setNextUpdate(const QDateTime &date)
{
    m_nextUpdate = date;
}

void Subscription::setSourceUrl(const QUrl &source)
{
    m_sourceUrl = source;
}

int Subscription::getNumFilters() const
{
    if (!m_enabled)
        return 0;

    return static_cast<int>(m_filters.size());
}

Filter *Subscription::getFilter(int index)
{
    if (!m_enabled)
        return nullptr;

    if (index < 0 || index >= static_cast<int>(m_filters.size()))
        return nullptr;
    return m_filters[index].get();
}

const QString &Subscription::getFilePath() const
{
    return m_filePath;
}

void Subscription::setFilePath(const QString &filePath)
{
    m_filePath = filePath;
}

}
