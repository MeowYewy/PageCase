#pragma once

#include <QImage>
#include <QString>
#include <QStringList>

class PdfPageRenderer
{
public:
    static QString findPdftoppm();
    static QStringList renderPdfPages(const QString &pdfPath, const QString &outputDir,
                                      const QString &prefix, int dpi = 120,
                                      int firstPage = 1, int lastPage = -1);
    static QImage renderPageWithQt(const QString &pdfPath, int pageIndex, int dpi);
    static bool hasQtPdf();
};
