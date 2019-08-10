#include "BookmarkNode.h"
#include "URLRecord.h"
#include "URLSuggestion.h"

URLSuggestion::URLSuggestion(const BookmarkNode *bookmark, const HistoryEntry &historyEntry, MatchType matchType) :
    Favicon(bookmark->getIcon()),
    Title(bookmark->getName()),
    URL(bookmark->getURL().toString()),
    LastVisit(historyEntry.LastVisit),
    URLTypedCount(historyEntry.URLTypedCount),
    VisitCount(historyEntry.NumVisits),
    StartIndex(0),
    IsHostMatch(false),
    IsBookmark(true),
    Type(matchType)
{
}

URLSuggestion::URLSuggestion(const URLRecord &record, const QIcon &icon, MatchType matchType) :
    Favicon(icon),
    Title(record.getTitle()),
    URL(record.getUrl().toString()),
    LastVisit(record.getLastVisit()),
    URLTypedCount(record.getUrlTypedCount()),
    VisitCount(record.getNumVisits()),
    StartIndex(0),
    IsHostMatch(false),
    IsBookmark(false),
    Type(matchType)
{
}
