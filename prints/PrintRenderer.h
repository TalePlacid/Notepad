#ifndef _PRINTRENDERER_H //guard
#define _PRINTRENDERER_H

class CWnd;
class CDC;
class PreviewPaginator;
class PrinterResource;

class PrintRenderer {
public:
    PrintRenderer(CWnd* parent, PreviewPaginator* paginator, PrinterResource *printerResource);
    virtual ~PrintRenderer();

    void Render(CDC* dc);
private:
    CWnd* parent;
    PreviewPaginator* paginator;
    PrinterResource* printerResource;
};

#endif // !_PRINTRENDERER_H

