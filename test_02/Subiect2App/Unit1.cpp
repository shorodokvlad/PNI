//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <jpeg.hpp>
#include "Unit1.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1* Form1;

// globale
TPoint pt[100];
int cnt = 0;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) {}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject* Sender)
{
    Image1->Proportional = true;
    Image2->Proportional = true;

    /// de completat

    if (OpenDialog1->Execute()) {
        if (!FileExists(OpenDialog1->FileName))
            return;
        AnsiString temp = ExtractFileExt(OpenDialog1->FileName);
        AnsiString Ext = temp.LowerCase();
        if (Ext.AnsiPos("jpg") > 0) {
            TJPEGImage* myjpeg = new TJPEGImage();
            myjpeg->LoadFromFile(OpenDialog1->FileName);
            myjpeg->DIBNeeded();
            Image1->Picture->Bitmap->Assign(myjpeg);
            delete myjpeg;
        } else if (Ext.AnsiPos("bmp") > 0)
            Image1->Picture->Bitmap->LoadFromFile(OpenDialog1->FileName);
    }

    //** de completat histograma *//
    long Hr[256], Hg[256], Hb[256];
    memset(Hr, 0, sizeof(Hr));
    memset(Hg, 0, sizeof(Hg));
    memset(Hb, 0, sizeof(Hb));

    Graphics::TBitmap* src = new Graphics::TBitmap;
    src->Assign(Image1->Picture->Bitmap);
    src->PixelFormat = Image1->Picture->Bitmap->PixelFormat;
    src->PixelFormat = Image1->Picture->Bitmap->PixelFormat;

    Graphics::TBitmap* dst = new Graphics::TBitmap;
    dst->Width = src->Width;
    dst->Height = src->Height;
    dst->PixelFormat = src->PixelFormat;

    for (int y = 0; y < src->Height; y++) {
        for (int x = 0; x < src->Width; x++) {
            RGBTRIPLE* pixels = (RGBTRIPLE*)src->ScanLine[y];

            Hr[(int)(pixels[x].rgbtRed)]++;
            Hg[(int)(pixels[x].rgbtGreen)]++;
            Hb[(int)(pixels[x].rgbtBlue)]++;
        }
    }

    Chart1->Series[0]->Clear();
    Chart1->Series[1]->Clear();
    Chart1->Series[2]->Clear();

    for (int i = 0; i < 256; i++) {
        Chart1->Series[0]->AddY(Hr[i], "", clRed);
        Chart1->Series[1]->AddY(Hg[i], "", clGreen);
        Chart1->Series[2]->AddY(Hb[i], "", clBlue);
    }
    Chart1->Series[0]->Active = true;
    Chart1->Series[1]->Active = true;
    Chart1->Series[2]->Active = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject* Sender)
{
    Graphics::TBitmap* source = new Graphics::TBitmap;
    Graphics::TBitmap* dstS = new Graphics::TBitmap;
    source->Assign(Image1->Picture->Bitmap);
    source->PixelFormat = Image1->Picture->Bitmap->PixelFormat;

    Graphics::TBitmap* dest = new Graphics::TBitmap;
    dest->Width = source->Width;
    dest->Height = source->Height;
    dest->PixelFormat = source->PixelFormat;
    // de completat

    // FILTRARE
    if (CheckBox1->Checked) {
        double w[3][3] = { { 1.0 / 10, 1.0 / 10, 1.0 / 10 },
            { 1.0 / 10, 1.0 / 10, 1.0 / 10 },
            { 1.0 / 10, 1.0 / 10, 1.0 / 10 } };

        for (int y = 0; y < dest->Height; y++) {
            RGBTRIPLE* pixels = (RGBTRIPLE*)dest->ScanLine[y];
            for (int x = 0; x < dest->Width; x++) {
                double totalR = 0, totalG = 0, totalB = 0;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        int srcX = x + i - 1;
                        int srcY = y + j - 1;
                        if (srcX >= 0 && srcX < source->Width && srcY >= 0 &&
                            srcY < source->Height) {
                            RGBTRIPLE* pixelsS =
                                (RGBTRIPLE*)source->ScanLine[srcY];
                            totalR += w[i][j] * pixelsS[srcX].rgbtRed;
                            totalG += w[i][j] * pixelsS[srcX].rgbtGreen;
                            totalB += w[i][j] * pixelsS[srcX].rgbtBlue;
                        }
                    }
                }

                pixels[x].rgbtRed = std::max(0, std::min(255, (int)totalR));
                pixels[x].rgbtGreen = std::max(0, std::min(255, (int)totalG));
                pixels[x].rgbtBlue = std::max(0, std::min(255, (int)totalB));
            }
        }

        Image2->Picture->Bitmap->Assign(dest);
    }

    // DETECTIA MUCHIILOR
    if (CheckBox2->Checked) {
        double w[3][3] = { { 0, 0, 0 }, { 1, -1, 0 }, { 0, 0, 0 } };
        double w1[3][3] = { { 0, 1, 0 }, { 0, -1, 0 }, { 0, 0, 0 } };

        for (int y = 0; y < dest->Height; y++) {
            RGBTRIPLE* pixels = (RGBTRIPLE*)dest->ScanLine[y];
            for (int x = 0; x < dest->Width; x++) {
                double totalR = 0, totalG = 0, totalB = 0;
                double totalR1 = 0, totalG1 = 0, totalB1 = 0;

                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        int srcX = x + i - 1;
                        int srcY = y + j - 1;
                        if (srcX >= 0 && srcX < source->Width && srcY >= 0 &&
                            srcY < source->Height) {
                            RGBTRIPLE* pixelsS =
                                (RGBTRIPLE*)source->ScanLine[srcY];
                            totalR += w[i][j] * pixelsS[srcX].rgbtRed;
                            totalG += w[i][j] * pixelsS[srcX].rgbtGreen;
                            totalB += w[i][j] * pixelsS[srcX].rgbtBlue;

                            totalR1 += w1[i][j] * pixelsS[srcX].rgbtRed;
                            totalG1 += w1[i][j] * pixelsS[srcX].rgbtGreen;
                            totalB1 += w1[i][j] * pixelsS[srcX].rgbtBlue;
                        }
                    }
                }

                pixels[x].rgbtRed = std::max(
                    0, std::min(255,
                           (int)sqrt(totalR * totalR + totalR1 * totalR1)));
                pixels[x].rgbtGreen = std::max(
                    0, std::min(255,
                           (int)sqrt(totalG * totalG + totalG1 * totalG1)));
                pixels[x].rgbtBlue = std::max(
                    0, std::min(255,
                           (int)sqrt(totalB * totalB + totalB1 * totalB1)));
            }
        }

        Image2->Picture->Bitmap->Assign(dest);
    }


        // SELECTIA
        if (CheckBox3->Checked)
    {
        int sx1 = pt[cnt - 2].x, sy1 = pt[cnt - 2].y;
        int sx2 = pt[cnt - 1].x, sy2 = pt[cnt - 1].y;
        if (sx1 > sx2)
            std::swap(sx1, sx2);
        if (sy1 > sy2)
            std::swap(sy1, sy2);

        Graphics::TBitmap* src = Image1->Picture->Bitmap;
        int w = std::max(0, std::min(sx2, src->Width) - std::max(0, sx1));
        int h = std::max(0, std::min(sy2, src->Height) - std::max(0, sy1));
        if (w == 0 || h == 0)
            return;

        Graphics::TBitmap* bmp = new Graphics::TBitmap();
        bmp->PixelFormat = src->PixelFormat;
        bmp->Width = w;
        bmp->Height = h;

        bmp->Canvas->CopyRect(
            TRect(0, 0, w, h), src->Canvas, TRect(sx1, sy1, sx1 + w, sy1 + h));

        Image2->Picture->Bitmap = bmp;
    }


}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image1MouseDown(
    TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    // PROFILUL UNEI LINII
    Graphics::TBitmap* source = new Graphics::TBitmap;
    Graphics::TBitmap* dstS = new Graphics::TBitmap;
    source->Assign(Image1->Picture->Bitmap);
    source->PixelFormat = Image1->Picture->Bitmap->PixelFormat;

    RGBTRIPLE* linie =
        (RGBTRIPLE*)
            source->ScanLine[Y * (float)source->Height / Image1->Height];

    Chart1->Series[0]->Clear();
    Chart1->Series[1]->Clear();
    Chart1->Series[2]->Clear();

    for (int i = 0; i < source->Width; i++) {
        Chart1->Series[0]->AddY(linie[i].rgbtRed, "", clRed);
        Chart1->Series[1]->AddY(linie[i].rgbtGreen, "", clGreen);
        Chart1->Series[2]->AddY(linie[i].rgbtBlue, "", clBlue);
    }
    Chart1->Series[0]->Active = true;
    Chart1->Series[1]->Active = true;
	Chart1->Series[2]->Active = true;

    // SELECTIE
    pt[cnt].X = X;
	pt[cnt].Y = Y;
	cnt++;
}
//---------------------------------------------------------------------------

