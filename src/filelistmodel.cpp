#include "filelistmodel.h"

#include <QDir>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QUrl>

namespace {

QString normalizeLocalPath(const QString &path)
{
    if (path.isEmpty())
        return {};
    QString s = path;
    if (s.startsWith(QStringLiteral("file:"), Qt::CaseInsensitive))
        s = QUrl(s).toLocalFile();
    s = QDir::cleanPath(QDir::fromNativeSeparators(s));
    return QFileInfo(s).absoluteFilePath();
}

} // namespace

FileListModel::FileListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int FileListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_paths.size();
}

QVariant FileListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_paths.size())
        return {};

    const QFileInfo info(m_paths.at(index.row()));
    switch (role) {
    case PathRole: return info.absoluteFilePath();
    case NameRole: return info.fileName();
    case TypeRole: return QMimeDatabase().mimeTypeForFile(info).name();
    default: return {};
    }
}

QHash<int, QByteArray> FileListModel::roleNames() const
{
    return {
        {PathRole, "path"},
        {NameRole, "name"},
        {TypeRole, "mimeType"},
    };
}

void FileListModel::addFiles(const QStringList &paths)
{
    QStringList added;
    for (const QString &p : paths) {
        const QString resolved = normalizeLocalPath(p);
        if (resolved.isEmpty())
            continue;
        if (!m_paths.contains(resolved))
            added.append(resolved);
    }
    if (added.isEmpty())
        return;

    const int first = m_paths.size();
    beginInsertRows({}, first, first + added.size() - 1);
    m_paths.append(added);
    endInsertRows();
    emit countChanged();
    emit filesChanged();
}

void FileListModel::removeAt(int index)
{
    if (index < 0 || index >= m_paths.size())
        return;
    beginRemoveRows({}, index, index);
    m_paths.removeAt(index);
    endRemoveRows();
    emit countChanged();
    emit filesChanged();
}

void FileListModel::clear()
{
    if (m_paths.isEmpty())
        return;
    beginResetModel();
    m_paths.clear();
    endResetModel();
    emit countChanged();
    emit filesChanged();
}

void FileListModel::move(int from, int to)
{
    if (from < 0 || from >= m_paths.size() || to < 0 || to >= m_paths.size() || from == to)
        return;
    const int dest = to > from ? to + 1 : to;
    beginMoveRows({}, from, from, {}, dest);
    m_paths.move(from, to);
    endMoveRows();
    emit filesChanged();
}
