#pragma once

#include <QObject>
#include <QStringList>

class PdfEngine : public QObject
{
    Q_OBJECT

public:
    explicit PdfEngine(QObject *parent = nullptr);

    Q_INVOKABLE QString mergePdfs(const QStringList &inputs, const QString &outputPath);
    Q_INVOKABLE QString splitPdf(const QString &input, const QString &outputDir, bool byPage);
    Q_INVOKABLE QString rotatePdf(const QString &input, const QString &outputPath, int degrees);
    Q_INVOKABLE QString convertToPdf(const QStringList &inputs, const QString &outputPath);
    Q_INVOKABLE QString compressPdf(const QString &input, const QString &outputPath, int level);
    Q_INVOKABLE QString watermarkPdf(const QString &input, const QString &outputPath, const QString &text, int count = 1);
    Q_INVOKABLE QString exportPdfAsImages(const QString &input, const QString &outputDir, const QString &format);
    Q_INVOKABLE int pageCount(const QString &inputPath) const;

private:
    QString findQpdf() const;
    QString findPdftoppm() const;
    QString createWatermarkStamp(const QString &text, const QString &outputPath, int count,
                               qreal pageWPt, qreal pageHPt) const;
    bool runQpdf(const QStringList &args, QString *error) const;
    bool runProcess(const QString &program, const QStringList &args, QString *error) const;
};
