#include "appsettings.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QHash>

namespace {

QHash<QString, QHash<QString, QString>> buildStrings()
{
    QHash<QString, QHash<QString, QString>> all;

    all["zh_CN"] = {
        {"appName", "PDF Studio"},
        {"tabSplit", "拆分"},
        {"tabMerge", "合并"},
        {"tabRotate", "旋转"},
        {"tabConvert", "转换"},
        {"dropHint", "拖放文件到此处，或点击浏览"},
        {"browse", "浏览文件"},
        {"run", "开始处理"},
        {"clear", "清空"},
        {"preview", "文件预览"},
        {"noFiles", "尚未添加文件"},
        {"filesAdded", "个文件已添加"},
        {"language", "语言"},
        {"theme", "界面"},
        {"light", "浅色"},
        {"dark", "深色"},
        {"about", "关于"},
        {"aboutTitle", "关于 PDF Studio"},
        {"aboutTagline", "简洁优雅的PDF工具"},
        {"close", "关闭"},
        {"fileList", "文件"},
        {"addedFiles", "已添加"},
        {"version", "版本"},
        {"aboutAuthor", "MeowYewy"},
        {"aboutCopyright", "版权所有"},
        {"checkUpdate", "检查更新"},
        {"newVersion", "新版本"},
        {"changelog", "更新日志"},
        {"changelogTitle", "更新日志"},
        {"changelogEmpty", "暂无更新记录"},
        {"splitDesc", "将多页 PDF 拆分为单页"},
        {"mergeDesc", "将多个 PDF 合并为一个文件"},
        {"rotateDesc", "旋转PDF页面至对应方向"},
        {"convertDesc", "将其他格式文件转换为PDF"},
        {"tabCompress", "压缩"},
        {"tabWatermark", "水印"},
        {"compressDesc", "减小 PDF 文件体积"},
        {"watermarkDesc", "为 PDF 每一页添加文字水印"},
        {"watermarkPlaceholder", "水印文字"},
        {"formatPdf", "PDF"},
        {"formatPng", "PNG"},
        {"formatJpeg", "JPEG"},
        {"compressLow", "标准"},
        {"compressMid", "较高"},
        {"compressHigh", "最高"},
        {"success", "已完成"},
        {"failed", "处理失败"},
        {"pages", "页"},
    };

    all["zh_TW"] = {
        {"appName", "PDF Studio"},
        {"tabSplit", "拆分"},
        {"tabMerge", "合併"},
        {"tabRotate", "旋轉"},
        {"tabConvert", "轉換"},
        {"dropHint", "拖放檔案到此處，或點擊瀏覽"},
        {"browse", "瀏覽檔案"},
        {"run", "開始處理"},
        {"clear", "清空"},
        {"preview", "檔案預覽"},
        {"noFiles", "尚未新增檔案"},
        {"filesAdded", "個檔案已新增"},
        {"language", "語言"},
        {"theme", "介面"},
        {"light", "淺色"},
        {"dark", "深色"},
        {"about", "關於"},
        {"aboutTitle", "關於 PDF Studio"},
        {"aboutTagline", "簡潔優雅的PDF工具"},
        {"close", "關閉"},
        {"fileList", "檔案"},
        {"addedFiles", "已新增"},
        {"version", "版本"},
        {"aboutAuthor", "MeowYewy"},
        {"aboutCopyright", "版權所有"},
        {"checkUpdate", "檢查更新"},
        {"newVersion", "新版本"},
        {"changelog", "更新日誌"},
        {"changelogTitle", "更新日誌"},
        {"changelogEmpty", "暫無更新記錄"},
        {"splitDesc", "將多頁 PDF 拆分為單頁"},
        {"mergeDesc", "將多個 PDF 合併為一個檔案"},
        {"rotateDesc", "旋轉 PDF 頁面至對應方向"},
        {"convertDesc", "將其他格式檔案轉換為 PDF"},
        {"tabCompress", "壓縮"},
        {"tabWatermark", "浮水印"},
        {"compressDesc", "縮小 PDF 檔案體積"},
        {"watermarkDesc", "為 PDF 每一頁新增文字浮水印"},
        {"watermarkPlaceholder", "浮水印文字"},
        {"formatPdf", "PDF"},
        {"formatPng", "PNG"},
        {"formatJpeg", "JPEG"},
        {"compressLow", "標準"},
        {"compressMid", "較高"},
        {"compressHigh", "最高"},
        {"success", "已完成"},
        {"failed", "處理失敗"},
        {"pages", "頁"},
    };

    all["en"] = {
        {"appName", "PDF Studio"},
        {"tabSplit", "Split"},
        {"tabMerge", "Merge"},
        {"tabRotate", "Rotate"},
        {"tabConvert", "Convert"},
        {"dropHint", "Drop files here, or click to browse"},
        {"browse", "Browse"},
        {"run", "Process"},
        {"clear", "Clear"},
        {"preview", "Preview"},
        {"noFiles", "No files added yet"},
        {"filesAdded", "file(s) added"},
        {"language", "Language"},
        {"theme", "Appearance"},
        {"light", "Light"},
        {"dark", "Dark"},
        {"about", "About"},
        {"aboutTitle", "About PDF Studio"},
        {"aboutTagline", "Simple, elegant PDF tools"},
        {"close", "Close"},
        {"fileList", "Files"},
        {"addedFiles", "Added"},
        {"version", "Version"},
        {"aboutAuthor", "MeowYewy"},
        {"aboutCopyright", "All rights reserved"},
        {"checkUpdate", "Check for Updates"},
        {"newVersion", "New"},
        {"changelog", "Changelog"},
        {"changelogTitle", "Changelog"},
        {"changelogEmpty", "No release notes yet"},
        {"splitDesc", "Split a multi-page PDF into single-page files"},
        {"mergeDesc", "Combine multiple PDFs into one"},
        {"rotateDesc", "Rotate PDF pages to the desired orientation"},
        {"convertDesc", "Convert other file formats to PDF"},
        {"tabCompress", "Compress"},
        {"tabWatermark", "Watermark"},
        {"compressDesc", "Reduce PDF file size"},
        {"watermarkDesc", "Add a text watermark to every PDF page"},
        {"watermarkPlaceholder", "Watermark text"},
        {"formatPdf", "PDF"},
        {"formatPng", "PNG"},
        {"formatJpeg", "JPEG"},
        {"compressLow", "Standard"},
        {"compressMid", "High"},
        {"compressHigh", "Maximum"},
        {"success", "Completed"},
        {"failed", "Failed"},
        {"pages", "pages"},
    };

    all["fr"] = {
        {"appName", "PDF Studio"},
        {"tabSplit", "Diviser"},
        {"tabMerge", "Fusionner"},
        {"tabRotate", "Rotation"},
        {"tabConvert", "Convertir"},
        {"dropHint", "Déposez des fichiers ici ou cliquez pour parcourir"},
        {"browse", "Parcourir"},
        {"run", "Traiter"},
        {"clear", "Effacer"},
        {"preview", "Aperçu"},
        {"noFiles", "Aucun fichier ajouté"},
        {"filesAdded", "fichier(s) ajouté(s)"},
        {"language", "Langue"},
        {"theme", "Apparence"},
        {"light", "Clair"},
        {"dark", "Sombre"},
        {"about", "À propos"},
        {"aboutTitle", "À propos de PDF Studio"},
        {"aboutTagline", "Des outils PDF simples et élégants"},
        {"close", "Fermer"},
        {"fileList", "Fichiers"},
        {"addedFiles", "Ajoutés"},
        {"version", "Version"},
        {"aboutAuthor", "MeowYewy"},
        {"aboutCopyright", "Tous droits réservés"},
        {"checkUpdate", "Vérifier les mises à jour"},
        {"newVersion", "Nouveau"},
        {"changelog", "Journal"},
        {"changelogTitle", "Journal des mises à jour"},
        {"changelogEmpty", "Aucune note de version"},
        {"splitDesc", "Diviser un PDF multipage en pages simples"},
        {"mergeDesc", "Fusionner plusieurs PDF en un seul"},
        {"rotateDesc", "Faire pivoter les pages PDF dans l'orientation souhaitée"},
        {"convertDesc", "Convertir d'autres formats de fichiers en PDF ou en images"},
        {"tabCompress", "Compression"},
        {"tabWatermark", "Filigrane"},
        {"compressDesc", "Réduire la taille du fichier PDF"},
        {"watermarkDesc", "Ajouter un filigrane texte sur chaque page PDF"},
        {"watermarkPlaceholder", "Texte du filigrane"},
        {"formatPdf", "PDF"},
        {"formatPng", "PNG"},
        {"formatJpeg", "JPEG"},
        {"compressLow", "Standard"},
        {"compressMid", "Élevé"},
        {"compressHigh", "Maximum"},
        {"success", "Terminé"},
        {"failed", "Échec du traitement"},
        {"pages", "pages"},
    };

    return all;
}

} // namespace

AppSettings::AppSettings(QObject *parent)
    : QObject(parent)
    , m_store(QStringLiteral("TechG"), QStringLiteral("ProjectP"))
    , m_language(m_store.value(QStringLiteral("language"), QStringLiteral("zh_CN")).toString())
    , m_theme(m_store.value(QStringLiteral("theme"), QStringLiteral("light")).toString())
{
    loadTranslations(m_language);
    loadWatermarkHistory();
}

void AppSettings::setLanguage(const QString &lang)
{
    if (m_language == lang)
        return;
    m_language = lang;
    m_store.setValue(QStringLiteral("language"), lang);
    ++m_languageRevision;
    loadTranslations(lang);
    emit languageChanged();
}

void AppSettings::setTheme(const QString &theme)
{
    if (m_theme == theme)
        return;
    m_theme = theme;
    m_store.setValue(QStringLiteral("theme"), theme);
    ++m_themeRevision;
    emit themeChanged();
}

QString AppSettings::trKey(const QString &key) const
{
    static const auto strings = buildStrings();
    const auto langMap = strings.value(m_language, strings.value(QStringLiteral("zh_CN")));
    return langMap.value(key, key);
}

QString AppSettings::lastOutputDir() const
{
    return m_store.value(QStringLiteral("lastOutputDir")).toString();
}

void AppSettings::setLastOutputDir(const QString &dir)
{
    const QString normalized = dir.trimmed();
    if (lastOutputDir() == normalized)
        return;
    m_store.setValue(QStringLiteral("lastOutputDir"), normalized);
    emit lastOutputDirChanged();
}

void AppSettings::rememberOutputPath(const QString &fileOrDir)
{
    const QString trimmed = fileOrDir.trimmed();
    if (trimmed.isEmpty())
        return;

    QFileInfo info(trimmed);
    const QString dir = info.isDir() ? info.absoluteFilePath() : info.absolutePath();
    if (!dir.isEmpty())
        setLastOutputDir(dir);
}

void AppSettings::loadWatermarkHistory()
{
    m_watermarkHistory = m_store.value(QStringLiteral("watermarkHistory")).toStringList();
    while (m_watermarkHistory.size() > 3)
        m_watermarkHistory.removeLast();
}

void AppSettings::saveWatermarkHistory()
{
    m_store.setValue(QStringLiteral("watermarkHistory"), m_watermarkHistory);
}

void AppSettings::addWatermarkHistory(const QString &text)
{
    const QString trimmed = text.trimmed();
    if (trimmed.isEmpty())
        return;

    m_watermarkHistory.removeAll(trimmed);
    m_watermarkHistory.prepend(trimmed);
    while (m_watermarkHistory.size() > 3)
        m_watermarkHistory.removeLast();

    saveWatermarkHistory();
    ++m_watermarkHistoryRevision;
    emit watermarkHistoryChanged();
}

void AppSettings::removeWatermarkHistoryAt(int index)
{
    if (index < 0 || index >= m_watermarkHistory.size())
        return;

    m_watermarkHistory.removeAt(index);
    saveWatermarkHistory();
    ++m_watermarkHistoryRevision;
    emit watermarkHistoryChanged();
}

void AppSettings::loadTranslations(const QString &)
{
    // Inline dictionary via trKey(); external .qm files can be added later.
}
