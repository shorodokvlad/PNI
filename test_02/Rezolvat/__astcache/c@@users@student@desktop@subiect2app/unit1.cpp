//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <jpeg.hpp>
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1* Form1;
#include <jpeg.hpp>

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) {}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject* Sender)
{
    Image1->Proportional = true;
    Image2->Proportional = true;

    /// de completat

    //** de completat histograma *//
    long Hr[256], Hg[256], Hb[256];
    memset(Hr, 0, sizeof(Hr));
    memset(Hg, 0, sizeof(Hg));
    memset(Hb, 0, sizeof(Hb));

    if (OpenDialog1->Execute()) {
        OpenDialog1->Filter = "Imagini|*.bmp;*.jpg";
        AnsiString temp = ExtractFileExt(OpenDialog1->FileName);
        AnsiString Ext = temp.LowerCase();

        if (Ext.AnsiPos("jpg") > 0) // este un fisier jpg
        { //-- Decomprima jpeg-ul in imagine bitmap
            TJPEGImage* myjpeg = new TJPEGImage();
            myjpeg->LoadFromFile(OpenDialog1->FileName);
            // creaza o replica de tip bitmap
            myjpeg->DIBNeeded();
            Image1->Picture->Bitmap->Assign(myjpeg);
            delete myjpeg;
        } else if (Ext.AnsiPos("bmp") > 0) {
            Image1->Picture->Bitmap->LoadFromFile(OpenDialog1->FileName);
        }
    }
    Graphics::TBitmap* src = new Graphics::TBitmap;
    src->Assign(Image1->Picture->Bitmap);
    src->PixelFormat = Image1->Picture->Bitmap->PixelFormat;
    for (int y = 0; y < src->Height; y++) {
        RGBTRIPLE* pixels = (RGBTRIPLE*)src->ScanLine[y];
        for (int x = 0; x < src->Width; x++) {
            Hr[pixels[x].rgbtRed]++;
            Hg[pixels[x].rgbtGreen]++;
            Hb[pixels[x].rgbtBlue]++;
        }
    }

    Chart1->LeftAxis->Logarithmic = true;
    Chart1->Series[0]->Clear();
    Chart1->Series[1]->Clear();
    Chart1->Series[2]->Clear();
    for (int i = 0; i < 256; i++) {
        Chart1->Series[0]->AddY(Hr[i], "", clRed);
        Chart1->Series[1]->AddY(Hg[i], "", clBlue);
        Chart1->Series[2]->AddY(Hb[i], "", clGreen);
    }
    Chart1->Series[0]->Active = true;
    Chart1->Series[1]->Active = true;
    Chart1->Series[2]->Active = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject* Sender)
{
    Graphics::TBitmap* src = new Graphics::TBitmap;
    Graphics::TBitmap* dstS = new Graphics::TBitmap;
    src->Assign(Image1->Picture->Bitmap);
    src->PixelFormat = Image1->Picture->Bitmap->PixelFormat;
    Graphics::TBitmap* dst = new Graphics::TBitmap;
    dst->Width = src->Width;
    dst->Height = src->Height;
    dst->PixelFormat = src->PixelFormat;

    // de completat

    if (CheckBox1->Checked) {
        double kernel[3][3] = { { 1.0 / 10, 1.0 / 10, 1.0 / 10 },
            { 1.0 / 10, 2.0 / 10, 1.0 / 10 },
            { 1.0 / 10, 1.0 / 10, 1.0 / 10 } };
        for (int y = 0; y < dst->Height; y++) {
            RGBTRIPLE* pixels = (RGBTRIPLE*)dst->ScanLine[y];
            for (int x = 0; x < dst->Width; x++) {
                double totalR = 0, totalG = 0, totalB = 0;

                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        int srcX = x + i - 1;
                        int srcY = y + j - 1;
                        if (srcX >= 0 && srcX < src->Width && srcY >= 0 &&
                            srcY < src->Height) {
                            RGBTRIPLE* pixelsS =
                                (RGBTRIPLE*)src->ScanLine[srcY];
                            totalR += kernel[i][j] * pixelsS[srcX].rgbtRed;
                            totalG += kernel[i][j] * pixelsS[srcX].rgbtGreen;
                            totalB += kernel[i][j] * pixelsS[srcX].rgbtBlue;
                        }
                    }
                }

                pixels[x].rgbtRed = std::max(0, std::min(255, (int)totalR));
                pixels[x].rgbtGreen = std::max(0, std::min(255, (int)totalG));
                pixels[x].rgbtBlue = std::max(0, std::min(255, (int)totalB));
            }
        }
		Image2->Picture->Bitmap->Assign(dst);
    } else if (CheckBox2->Checked) {
		double kernel[3][3] = { { 0, 0, 0 }, { 1, -1, 0 }, { 0,0,0} };
        double kernel1[3][3] = { { 1.0 / 10, 1.0 / 10, 1.0 / 10 },
            { 1.0 / 10, 2.0 / 10, 1.0 / 10 },
            { 1.0 / 10, 1.0 / 10, 1.0 / 10 } };
        for (int y = 0; y < dst->Height; y++) {
            RGBTRIPLE* pixels = (RGBTRIPLE*)dst->ScanLine[y];
            for (int x = 0; x < dst->Width; x++) {
                double totalR = 0, totalG = 0, totalB = 0;
                double totalR1 = 0, totalG1 = 0, totalB1 = 0;

                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        int srcX = x + i - 1;
                        int srcY = y + j - 1;
                        if (srcX >= 0 && srcX < src->Width && srcY >= 0 &&
                            srcY < src->Height) {
                            RGBTRIPLE* pixelsS =
                                (RGBTRIPLE*)src->ScanLine[srcY];
                            totalR += kernel[i][j] * pixelsS[srcX].rgbtRed;
                            totalG += kernel[i][j] * pixelsS[srcX].rgbtGreen;
                            totalB += kernel[i][j] * pixelsS[srcX].rgbtBlue;
                            totalR1 += kernel1[i][j] * pixelsS[srcX].rgbtRed;
                            totalG1 += kernel1[i][j] * pixelsS[srcX].rgbtGreen;
                            totalB1 += kernel1[i][j] * pixelsS[srcX].rgbtBlue;
                        }
                    }
                }

                pixels[x].rgbtRed = std::max(0, std::min(255, (int)totalR));
                pixels[x].rgbtGreen = std::max(0, std::min(255, (int)totalG));
                pixels[x].rgbtBlue = std::max(0, std::min(255, (int)totalB));
            }
        }
        Image2->Picture->Bitmap->Assign(dst);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image1MouseDown(
    TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    long Hr[256], Hg[256], Hb[256];
    memset(Hr, 0, sizeof(Hr));
    memset(Hg, 0, sizeof(Hg));
    memset(Hb, 0, sizeof(Hb));
    RGBTRIPLE* linie = (RGBTRIPLE*)Image1->Picture->Bitmap->ScanLine[Y];
    for (int x = 0; x < Image1->Picture->Bitmap->Width; x++) {
        Hr[linie[x].rgbtRed]++;
        Hg[linie[x].rgbtGreen]++;
        Hb[linie[x].rgbtBlue]++;
    }
    Chart1->Series[0]->Clear();
    Chart1->Series[1]->Clear();
    Chart1->Series[2]->Clear();
    for (int i = 0; i < 256; i++) {
        Chart1->Series[0]->AddY(Hr[i], "", clRed);
        Chart1->Series[1]->AddY(Hg[i], "", clBlue);
        Chart1->Series[2]->AddY(Hb[i], "", clGreen);
    }
    Chart1->Series[0]->Active = true;
    Chart1->Series[1]->Active = true;
    Chart1->Series[2]->Active = true;
}

//---------------------------------------------------------------------------

