//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <jpeg.hpp>
#include "Unit1.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1* Form1;


//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) {}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject* Sender)
{
    Image1->Proportional = true;
    Image2->Proportional = true;

    /// de completat

    if (OpenDialog1->Execute()) {
        if (!FileExists(OpenDialog1->FileName)) {
            return;
        }
        AnsiString Ext = ExtractFileExt(OpenDialog1->FileName);
        AnsiString temp = Ext.LowerCase();
        if (Ext.AnsiPos("jpg") > 0) {
            TJEPGImage* myjpeg = new TJEPGImage();
            myjpeg->LoadFromFile(OpenDialog1->FileName);
            myjpeg->DIBNeeded();
            Image1->Picture->Bitmap->Assign(myjpeg);
            delete myjpeg;
        } else if (Ext.AnsiPos("bmp") > 0) {
            Image1->Picture->Bitmap->LoadFromFile(OpenDialog1->FileName);
        }
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

            Hr[pixels[x].rgbtRed]++;
            Hg[pixels[x].rgbtRed]++;
            Hb[pixels[x].rgbtRed]++;
        }
    }

    Chart1->Series[0]->Clear();

    for (int i = 0; i < 256; i++) {
        Chart1->Series[0]->AddY(Hr[i], "", clRed);
    }
    Chart1->Series[0]->Active = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject* Sender)
{
    Graphics::TBitmap* source = new Graphics::TBitmap;
    source->Assign(Image1->Picture->Bitmap);
    source->PixelFormat = Image1->Picture->Bitmap->PixelFormat;

    if (CheckBox3->Checked) {
        double avg = 0;
        for (int y = 0; y < source->Height; y++) {
            RGBTRIPLE* pixels = (RGBTRIPLE*)source->ScanLine[y];
            for (int x = 0; x < source->Width; x++) {
                avg = (totalR * 0.33 + totalG * 0.59 + totalB * 0.11);

                pixels[x].rgbtRed = avg;
                pixels[x].rgbtGreen = avg;
                pixels[x].rgbtBlue = avg;
            }
        }
        Image2->Picture->Bitmap->Assign(dest);
    }

    Graphics::TBitmap* dest = new Graphics::TBitmap;
    dest->Width = source->Width;
    dest->Height = source->Height;
    dest->PixelFormat = source->PixelFormat;
    // de completat

    if (CheckBox1->Checked) {
         double w[3][3] = { { 1.0 / 10, 1.0 / 10, 1.0 / 10 },
            { 1.0 / 10, 1.0 / 10, 1.0 / 10 },
            { 1.0 / 10, 1.0 / 10, 1.0 / 10 } };

        for (int y = 0; y < dest->Height; y++) {
            RGBTRIPLE* pixels = (RGBTRIPLE*)source->ScanLine[y];
            for (int x = 0; x < dest->Width; x++) {
                double totalR = 0, totalG = 0, totalB = 0;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        int srcX = x + i - 1;
                        int srcY = y + j - 1;

                        if (srcX >= 0 && srcX < source->Width && srcY >= 0 && srcY < source->Height) {
                            RGBTRIPLE* pixelsS = (RGBTRIPLE*)source->ScanLine[srcY];
                            totalR = w[i][j] * pixelsS[srcY].rgbtRed;
                            totalG = w[i][j] * pixelsS[srcY].rgbtGreen;
                            totalB = w[i][j] * pixelsS[srcY].rgbtBlue;
                        }
                    }
                }
                pixels[x].rgbtRed = std::max(0, std:min(255, (int)totalR));
                pixels[x].rgbtGreen = std::max(0, std:min(255, (int)totalR));
                pixels[x].rgbtBlue = std::max(0, std:min(255, (int)totalR));

            }
        }
        Image2->Picture->Bitmap->Assign(dest);
    }

    if (CheckBox2->Checked) {
         double w[3][3] = { { 0, 0, 0 }, { 1, -1, 0 }, { 0, 0, 0 } };
         double w1[3][3] = { { 0, 1, 0 }, { 0, -1, 0 }, { 0, 0, 0 } };

        for (int y = 0; y < dest->Height; y++) {
            RGBTRIPLE* pixels = (RGBTRIPLE*)source->ScanLine[y];
            for (int x = 0; x < dest->Width; x++) {
                double totalR = 0, totalG = 0, totalB = 0;
                double totalR1 = 0, totalG1 = 0, totalB1 = 0;

                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        int srcX = x + i - 1;
                        int srcY = y + j - 1;

                        if (srcX >= 0 && srcX < source->Width && srcY >= 0 && srcY < source->Height) {
                            RGBTRIPLE* pixelsS = (RGBTRIPLE*)source->ScanLine[srcY];
                            totalR = w[i][j] * pixelsS[srcY].rgbtRed;
                            totalG = w[i][j] * pixelsS[srcY].rgbtGreen;
                            totalB = w[i][j] * pixelsS[srcY].rgbtBlue;

                            totalR1 = w1[i][j] * pixelsS[srcY].rgbtRed;
                            totalG1 = w1[i][j] * pixelsS[srcY].rgbtGreen;
                            totalB1 = w1[i][j] * pixelsS[srcY].rgbtBlue;
                        }
                    }
                }
                pixels[x].rgbtRed = std::max(0, std:min(255, (int)sqrt(totalR * totalR + totalR1 * totalR1)));
                pixels[x].rgbtGreen = std::max(0, std:min(255, (int)totalR));
                pixels[x].rgbtBlue = std::max(0, std:min(255, (int)totalR));

            }
        }
        Image2->Picture->Bitmap->Assign(dest);
    }

    



}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TForm1::Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{

    Graphics::TBitmap* source = new Graphics::TBitmap;
    Graphics::TBitmap* dstS = new Graphics::TBitmap;
    source->Assign(Image1->Picture->Bitmap);
    source->PixelFormat = Image1->Picture->Bitmap->PixelFormat;

    RGBTRIPLE* linie = (RGBTRIPLE*)source->ScanLine[Y * (float)source->Height / Image1->Height];

    Chart1->Series[0]->Clear();

    for (int i = 0; i < 256; i++) {
        Chart1->Series[0]->AddY(linie[i].rgbtRed, "", clRed);
    }
    Chart1->Series[0]->Active = true;

}
//---------------------------------------------------------------------------
