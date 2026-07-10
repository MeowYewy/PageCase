#include "pdfengine.h"
#include "watermarklayout.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QImageReader>
#include <QPdfWriter>
#include <QPainter>
#include <QPageLayout>
#include <QColor>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QStandardPaths>
#include <QTemporaryDir>

#ifdef HAS_QT_PDF
#include <QPdfDocument>
#endif

namespace {

bool isImage(const QString &path)
{
    const QString ext = QFileInfo(path).suffix().toLower();
    return ext == QLatin1String("png") || ext == QLatin1String("jpg")
        || ext == QLatin1String("jpeg") || ext == QLatin1String("bmp")
        || ext == QLatin1String("gif") || ext == QLatin1String("webp")
        || ext == QLatin1String("tif") || ext == QLatin1String("tiff")
        || ext == QLatin1String("ico");
}

bool isText(const QString &path)
{
    const QString ext = QFileInfo(path).suffix().toLower();
    return ext == QLatin1String("txt") || ext == QLatin1String("md")
        || ext == QLatin1String("csv") || ext == QLatin1String("log");
}

QString copyPdfToOutput(const QString &src, const QString &outputPath)
{
    if (src == outputPath)
        return {};

    if (QFile::exists(outputPath) && !QFile::remove(outputPath))
        return QStringLiteral("Cannot overwrite output");

    if (!QFile::copy(src, outputPath))
        return QStringLiteral("Cannot write output");
    return {};
}

QString imageToPdf(const QString &input, const QString &outputPath)
{
    QImageReader reader(input);
    reader.setAutoTransform(true);
    QImage image = reader.read();
    if (image.isNull())
        return QStringLiteral("Cannot load image: %1").arg(input);

    if (QFile::exists(outputPath) && !QFile::remove(outputPath))
        return QStringLiteral("Cannot overwrite output");

    QPdfWriter writer(outputPath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(150);

    QPainter painter(&writer);
    if (!painter.isActive())
        return QStringLiteral("Cannot create PDF for: %1").arg(input);

    const QRect target(0, 0, writer.width(), writer.height());
    painter.drawImage(target, image.scaled(target.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    painter.end();
    return {};
}

QString textToPdf(const QString &input, const QString &outputPath)
{
    QFile f(input);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return QStringLiteral("Cannot read: %1").arg(input);
    const QString text = QString::fromUtf8(f.readAll());

    if (QFile::exists(outputPath) && !QFile::remove(outputPath))
        return QStringLiteral("Cannot overwrite output");

    QPdfWriter writer(outputPath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(150);

    QPainter painter(&writer);
    if (!painter.isActive())
        return QStringLiteral("Cannot create PDF for: %1").arg(input);

    QFont font(QStringLiteral("Consolas"), 10);
    painter.setFont(font);
    int y = 80;
    const int lineHeight = 140;
    for (const QString &line : text.split(QLatin1Char('\n'))) {
        if (y > writer.height() - 80) {
            writer.newPage();
            y = 80;
        }
        painter.drawText(60, y, line);
        y += lineHeight;
    }
    painter.end();
    return {};
}

} // namespace

PdfEngine::PdfEngine(QObject *parent)
    : QObject(parent)
{
}

QString PdfEngine::findQpdf() const
{
    const QStringList candidates = {
        QDir(QCoreApplication::applicationDirPath())
            .filePath(QStringLiteral("tools/qpdf/qpdf.exe")),
        QDir(QCoreApplication::applicationDirPath())
            .filePath(QStringLiteral("../tools/qpdf/qpdf.exe")),
    };
    for (const QString &path : candidates) {
        if (QFileInfo::exists(path))
            return path;
    }
    return QStringLiteral("qpdf");
}

bool PdfEngine::runQpdf(const QStringList &args, QString *error) const
{
    QProcess proc;
    proc.setProgram(findQpdf());
    proc.setArguments(args);
    proc.start();
    if (!proc.waitForFinished(120000)) {
        if (error) *error = QStringLiteral("qpdf timeout");
        return false;
    }
    if (proc.exitCode() != 0) {
        if (error)
            *error = QString::fromUtf8(proc.readAllStandardError());
        return false;
    }
    return true;
}

QString PdfEngine::mergePdfs(const QStringList &inputs, const QString &outputPath)
{
    if (inputs.size() < 2)
        return QStringLiteral("Need at least 2 PDF files");

    if (QFile::exists(outputPath) && !QFile::remove(outputPath))
        return QStringLiteral("Cannot overwrite output");

    QStringList args;
    args << QStringLiteral("--empty") << QStringLiteral("--pages");
    for (const QString &in : inputs)
        args << in;
    args << QStringLiteral("--") << outputPath;

    QString err;
    if (!runQpdf(args, &err))
        return QStringLiteral("Merge failed: %1").arg(err);
    return {};
}

QString PdfEngine::splitPdf(const QString &input, const QString &outputDir, bool byPage)
{
    QDir().mkpath(outputDir);
    const QString pattern = QDir(outputDir).filePath(
        QFileInfo(input).completeBaseName() + QStringLiteral("_%d.pdf"));

    QStringList args;
    args << input << QStringLiteral("--split-pages") << pattern;

    QString err;
    if (!runQpdf(args, &err))
        return QStringLiteral("Split failed: %1").arg(err);
    Q_UNUSED(byPage);
    return {};
}

QString PdfEngine::rotatePdf(const QString &input, const QString &outputPath, int degrees)
{
    QStringList args;
    args << input << outputPath
         << QStringLiteral("--rotate=+%1:1-z").arg(degrees);

    QString err;
    if (!runQpdf(args, &err))
        return QStringLiteral("Rotate failed: %1").arg(err);
    return {};
}

QString PdfEngine::convertToPdf(const QStringList &inputs, const QString &outputPath)
{
    if (inputs.isEmpty())
        return QStringLiteral("No input files");

    QTemporaryDir tempDir;
    if (!tempDir.isValid())
        return QStringLiteral("Cannot create temp directory");

    QStringList pdfParts;

    for (const QString &path : inputs) {
        if (QFileInfo(path).suffix().toLower() == QLatin1String("pdf")) {
            pdfParts.append(path);
            continue;
        }

        const QString partPath = tempDir.filePath(
            QStringLiteral("part_%1.pdf").arg(pdfParts.size()));

        QString err;
        if (isImage(path))
            err = imageToPdf(path, partPath);
        else if (isText(path))
            err = textToPdf(path, partPath);
        else
            return QStringLiteral("Unsupported format: %1").arg(path);

        if (!err.isEmpty())
            return err;

        pdfParts.append(partPath);
    }

    if (pdfParts.isEmpty())
        return QStringLiteral("No convertible files");

    if (pdfParts.size() == 1)
        return copyPdfToOutput(pdfParts.first(), outputPath);

    if (QFile::exists(outputPath) && !QFile::remove(outputPath))
        return QStringLiteral("Cannot overwrite output");

    return mergePdfs(pdfParts, outputPath);
}

int PdfEngine::pageCount(const QString &pdfPath) const
{
    if (!QFile::exists(pdfPath))
        return 0;

    QProcess proc;
    proc.setProgram(findQpdf());
    proc.setArguments({QStringLiteral("--show-npages"), pdfPath});
    proc.start();
    if (!proc.waitForFinished(10000) || proc.exitCode() != 0)
        return 0;

    bool ok = false;
    const int n = QString::fromUtf8(proc.readAllStandardOutput()).trimmed().toInt(&ok);
    return ok ? n : 0;
}

QString PdfEngine::findPdftoppm() const
{
    const QStringList candidates = {
        QDir(QCoreApplication::applicationDirPath())
            .filePath(QStringLiteral("tools/poppler/pdftoppm.exe")),
        QDir(QCoreApplication::applicationDirPath())
            .filePath(QStringLiteral("../tools/poppler/pdftoppm.exe")),
        QStringLiteral("pdftoppm"),
    };
    for (const QString &path : candidates) {
        if (QFileInfo::exists(path))
            return path;
    }
    return QStringLiteral("pdftoppm");
}

QSizeF mediaBoxSizeFromArray(const QJsonArray &box)
{
    if (box.size() < 4)
        return {};
    const double w = box.at(2).toDouble() - box.at(0).toDouble();
    const double h = box.at(3).toDouble() - box.at(1).toDouble();
    if (w > 0.0 && h > 0.0)
        return QSizeF(w, h);
    return {};
}

QSizeF mediaBoxFromPageObject(const QJsonObject &value)
{
    QJsonArray box = value.value(QStringLiteral("/MediaBox")).toArray();
    QSizeF size = mediaBoxSizeFromArray(box);
    if (!size.isEmpty())
        return size;

    const QJsonObject mediaBox = value.value(QStringLiteral("mediabox")).toObject();
    const double left = mediaBox.value(QStringLiteral("lower left x")).toDouble();
    const double bottom = mediaBox.value(QStringLiteral("lower left y")).toDouble();
    const double right = mediaBox.value(QStringLiteral("upper right x")).toDouble();
    const double top = mediaBox.value(QStringLiteral("upper right y")).toDouble();
    const double w = right - left;
    const double h = top - bottom;
    if (w > 0.0 && h > 0.0)
        return QSizeF(w, h);
    return {};
}

QSizeF firstPdfPageSizePoints(const QString &input)
{
#ifdef HAS_QT_PDF
    QPdfDocument doc;
    if (doc.load(input) == QPdfDocument::Error::None) {
        const QSizeF size = doc.pagePointSize(0);
        if (!size.isEmpty())
            return size;
    }
#endif
    QProcess proc;
    proc.setProgram(QCoreApplication::applicationDirPath() + QStringLiteral("/tools/qpdf/qpdf.exe"));
    if (!QFileInfo::exists(proc.program()))
        proc.setProgram(QStringLiteral("qpdf"));
    proc.setArguments({QStringLiteral("--json"), QStringLiteral("--json-key=pages"), input});
    proc.start();
    if (proc.waitForFinished(15000) && proc.exitCode() == 0) {
        const QJsonDocument json = QJsonDocument::fromJson(proc.readAllStandardOutput());
        const QJsonObject root = json.object();

        QJsonArray pages = root.value(QStringLiteral("pages")).toArray();
        if (pages.isEmpty()) {
            const QJsonArray qpdfArr = root.value(QStringLiteral("qpdf")).toArray();
            for (const QJsonValue &entry : qpdfArr) {
                if (!entry.isObject())
                    continue;
                const QJsonArray nested = entry.toObject().value(QStringLiteral("pages")).toArray();
                if (!nested.isEmpty()) {
                    pages = nested;
                    break;
                }
            }
        }

        if (!pages.isEmpty()) {
            const QJsonObject page = pages.first().toObject();
            const double width = page.value(QStringLiteral("width")).toDouble();
            const double height = page.value(QStringLiteral("height")).toDouble();
            if (width > 0.0 && height > 0.0)
                return QSizeF(width, height);

            const QSizeF fromBox = mediaBoxFromPageObject(page);
            if (!fromBox.isEmpty())
                return fromBox;
        }

        const QJsonArray qpdfArr = root.value(QStringLiteral("qpdf")).toArray();
        if (qpdfArr.size() >= 2) {
            const QJsonObject objects = qpdfArr.at(1).toObject();
            for (auto it = objects.begin(); it != objects.end(); ++it) {
                const QJsonObject obj = it.value().toObject();
                const QJsonObject value = obj.value(QStringLiteral("value")).toObject();
                const QString type = value.value(QStringLiteral("/Type")).toString();
                if (type != QStringLiteral("/Page"))
                    continue;
                const QSizeF fromBox = mediaBoxFromPageObject(value);
                if (!fromBox.isEmpty())
                    return fromBox;
            }
        }
    }
    return QSizeF(595.0, 842.0);
}

bool PdfEngine::runProcess(const QString &program, const QStringList &args, QString *error) const
{
    QProcess proc;
    proc.setProgram(program);
    proc.setArguments(args);
    proc.start();
    if (!proc.waitForFinished(120000)) {
        if (error)
            *error = QStringLiteral("Process timeout");
        return false;
    }
    if (proc.exitCode() != 0) {
        if (error)
            *error = QString::fromUtf8(proc.readAllStandardError());
        return false;
    }
    return true;
}

QString PdfEngine::compressPdf(const QString &input, const QString &outputPath, int level)
{
    if (QFile::exists(outputPath) && !QFile::remove(outputPath))
        return QStringLiteral("Cannot overwrite output");

    QStringList args;
    args << input;

    if (level >= 1)
        args << QStringLiteral("--object-streams=generate");
    if (level >= 2)
        args << QStringLiteral("--recompress-flate");

    args << QStringLiteral("--stream-data=compress")
         << QStringLiteral("--")
         << outputPath;

    QString err;
    if (!runQpdf(args, &err))
        return QStringLiteral("Compress failed: %1").arg(err);
    return {};
}

QString PdfEngine::createWatermarkStamp(const QString &text, const QString &outputPath, int count,
                                        qreal pageWPt, qreal pageHPt) const
{
    if (QFile::exists(outputPath) && !QFile::remove(outputPath))
        return QStringLiteral("Cannot overwrite watermark stamp");

    QPdfWriter writer(outputPath);
    const QPageSize pageSize(QSizeF(pageWPt, pageHPt), QPageSize::Point);
    QPageLayout layout(pageSize, QPageLayout::Portrait, QMarginsF(0, 0, 0, 0));
    layout.setMode(QPageLayout::FullPageMode);
    writer.setPageLayout(layout);
    writer.setResolution(200);

    const int pageW = writer.width();
    const int pageH = writer.height();
    if (pageW <= 0 || pageH <= 0)
        return QStringLiteral("Cannot create watermark stamp");

    QImage layer(pageW, pageH, QImage::Format_ARGB32_Premultiplied);
    layer.fill(Qt::transparent);

    {
        QPainter imagePainter(&layer);
        imagePainter.setRenderHint(QPainter::Antialiasing, true);
        imagePainter.setRenderHint(QPainter::TextAntialiasing, true);
        imagePainter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        WatermarkLayout::paint(imagePainter, text.trimmed(), count, pageW, pageH);
    }

    QPainter painter(&writer);
    if (!painter.isActive())
        return QStringLiteral("Cannot create watermark stamp");

    painter.setClipping(false);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.drawImage(QRect(0, 0, pageW, pageH), layer);
    painter.end();
    return {};
}

QString PdfEngine::watermarkPdf(const QString &input, const QString &outputPath, const QString &text, int count)
{
    if (text.trimmed().isEmpty())
        return QStringLiteral("Watermark text is required");

    QTemporaryDir tempDir;
    if (!tempDir.isValid())
        return QStringLiteral("Cannot create temp directory");

    const QString stampPath = tempDir.filePath(QStringLiteral("stamp.pdf"));
    const QSizeF pageSize = firstPdfPageSizePoints(input);
    QString err = createWatermarkStamp(text.trimmed(), stampPath, count,
                                       pageSize.width(), pageSize.height());
    if (!err.isEmpty())
        return err;

    if (QFile::exists(outputPath) && !QFile::remove(outputPath))
        return QStringLiteral("Cannot overwrite output");

    QStringList args;
    args << input
         << QStringLiteral("--overlay")
         << stampPath
         << QStringLiteral("--repeat=1-z")
         << QStringLiteral("--")
         << outputPath;

    if (!runQpdf(args, &err))
        return QStringLiteral("Watermark failed: %1").arg(err);
    return {};
}

QString PdfEngine::exportPdfAsImages(const QString &input, const QString &outputDir, const QString &format)
{
    if (QFileInfo(input).suffix().compare(QLatin1String("pdf"), Qt::CaseInsensitive) != 0)
        return QStringLiteral("Image export requires a PDF file");

    QDir().mkpath(outputDir);
    const QString prefix = QDir(outputDir).filePath(QFileInfo(input).completeBaseName());

    QStringList args;
    if (format == QLatin1String("png"))
        args << QStringLiteral("-png");
    else
        args << QStringLiteral("-jpeg");

    args << input << prefix;

    QString err;
    if (!runProcess(findPdftoppm(), args, &err))
        return QStringLiteral("Export failed: %1").arg(err);
    return {};
}
