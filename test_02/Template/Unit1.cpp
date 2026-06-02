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
        AnsiString temp = ExtractFileExt(OpenDialog1->FileName);
        Ansistring Ext = temp.LowerCase();
        if (Ext.AnsiPos("jpg") > 0) {
            TJPEGImage* myjpeg = new TJPEGImage();
            myjpeg->LoadFromFile(OpenDialog1->FileName);
            myjpeg->DIBNeeded();
            Image1->Picture->Bitmap->Assign(myjpeg);
            delete myjpeg;
        } else if (Ext.AnsiString("bmp") > 0) {
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

    if (CheckBox4->Checked) {
        double avg = 0;
        for (int y = 0; y < source->Height; y++) {
            RGBTRIPLE* pixels = (RGBTRIPLE*)source->ScanLine[y]; 
            for (int x = 0; x < source->Width; x++) {
                avg = (pixels[x].rgbtRed * 0.33 + pixels[x].rgbtGreen * 0.33 + pixels[x].rgbtBlue * 0.33);
                pixels[x].rgbtRed = avg;
                pixels[x].rgbtGreen = avg;
                pixels[x].rgbtBlue = avg;
            }
        }
        Image2->Picture->Bitmap->Assign(source);
    }

    if (CheckBox5->Checked) {
        double avg = 0;
        for (int y = 0; y < source->Height; y++) {
            RGBTRIPLE* pixels = (RGBTRIPLE*)source->ScanLine[y]; 
            for (int x = 0; x < source->Width; x++) {
                avg = (pixels[x].rgbtRed * 0.33 + pixels[x].rgbtGreen * 0.33 + pixels[x].rgbtBlue * 0.33);
                if (Edit1->Text.ToIntDef(0) * 255.0 / 100 > avg) {
                    source->Canvas->Pixels[x][y] = TColor(RGB(0, 0, 0));
                } else {
                    soruce->Canvas->Pixels[x][y] = TColor(RGB(255, 255, 255));
                }
            }
        }
    }

    if (CheckBox6->Checked) {
        double mSize = 8;
        for (int y = 0; y < source->Height; y++) {
            for (int x = 0; x < source->Width; x++) {
                int newX = mSize - x % mSize;
                int newY = mSize - y % mSize;
                
                if (x + newX > 0 && x + newX < source->Width && y + newY > 0 && y + newY < source->Height) {
                    TColor color = source->Canvas->Pixels[x + newX][y + newY];
                    int r = GetValue(color);
                    int g = GetValue(color);
                    int b = GetValue(color);

                    source->Canvas->Pixels[x][y] = TColor(RGB(r, g, b));
                } else {
                    source->Canvas->Pixels[x][y] = TColor(RGB(255, 255, 255));
                }
            }
        }
        Image2->Picture->Bitmap->Assign(source);
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

        for (int y= 0 ; y < dest->Height; y++) {
            RGBTRIPLE* pixels = (RGBTRIPLE*)dest->ScanLine[y];
            for (int x = 0; x < dest->Width; x++) {
                double totalR = 0, totalG = 0, totalB = 0;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        int srcX = x + i - 1;
                        int srcY = y + j - 1;

                        if (srcX >= 0 && srcX < source->Width && srcY >= 0 && srcY < source->Height) {
                            RGBTRIPLE* pixelsS = (RGBTRIPLE*)source->ScanLine[srcy];

                            totalR += w[i][j] * pixelsS[x].rgbtRed;
                            totalG += w[i][j] * pixelsS[x].rgbtGreen;
                            totalB += w[i][j] * pixelsS[x].rgbtBlue;

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

    if (CheckBox2->Checked) {
        double w[3][3] = { { 0, 0, 0 }, { 1, -1, 0 }, { 0, 0, 0 } };
        double w1[3][3] = { { 0, 1, 0 }, { 0, -1, 0 }, { 0, 0, 0 } };

        for (int y= 0 ; y < dest->Height; y++) {
            RGBTRIPLE* pixels = (RGBTRIPLE*)dest->ScanLine[y];
            for (int x = 0; x < dest->Width; x++) {
                double totalR = 0, totalG = 0, totalB = 0;
                double totalR1 = 0, totalG1 = 0, totalB1 = 0;

                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        int srcX = x + i - 1;
                        int srcY = y + j - 1;

                        if (srcX >= 0 && srcX < source->Width && srcY >= 0 && srcY < source->Height) {
                            RGBTRIPLE* pixelsS = (RGBTRIPLE*)source->ScanLine[srcy];

                            totalR += w[i][j] * pixelsS[x].rgbtRed;
                            totalG += w[i][j] * pixelsS[x].rgbtGreen;
                            totalB += w[i][j] * pixelsS[x].rgbtBlue;

                            totalR1 += w1[i][j] * pixelsS[x].rgbtRed;
                            totalG1 += w1[i][j] * pixelsS[x].rgbtGreen;
                            totalB1 += w1[i][j] * pixelsS[x].rgbtBlue;

                        }
                    }
                }
                pixels[x].rgbtRed = std::max(0, std::min(255, (int)sqrt(totalR * totalR + totalR1 * totalR1)));
                pixels[x].rgbtGreen = std::max(0, std::min(255, (int)totalG));
                pixels[x].rgbtBlue = std::max(0, std::min(255, (int)totalB));

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

    RBTRIPLE* linie = (RBTRIPLE*)source->ScanLine[y * (float)source->Height / Image1->Height];

}
//---------------------------------------------------------------------------
