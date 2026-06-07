//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <jpeg.hpp>
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1* Form1;

TPoint pt[100];
int cnt = 0;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) {}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject* Sender)
{
    OpenDialog1->Filter = "Bmp files(*.bmp)|*.BMP|JPEG images\(*.jpg)|*.jpg;";
    Image1->Proportional = true;
    Image2->Proportional = true;
    Image3->Proportional = true;

    /// ** de completat *///

    if (OpenDialog1->Execute()) {
        if (!FileExists(OpenDialog1->FileName)) {
            return;
        }
        AnsiString temp = ExtractFileExt(OpenDialog1->FileName);
        AnsiString Ext = temp.LowerCase();
        if (Ext.AnsiPos("jpg") > 0) {
            TJPEGImage* myjpeg = new TJPEGImage();
            myjpeg->LoadFromFile(OpenDialog1->FileName);
            myjpeg->DIBNeeded();
            Image1->Picture->Bitmap->Assign(myjpeg);
            delete myjpeg;
        } else if (Ext.AnsiPos("bmp")) {
            Image1->Picture->Bitmap->LoadFromFile(OpenDialog1->FileName);
        }
    }

    long Hr[256], Hg[256], Hb[256];
    memset(Hr, 0, sizeof(Hr));
    memset(Hg, 0, sizeof(Hg));
    memset(Hb, 0, sizeof(Hb));

    Graphics::TBitmap* source = new Graphics::TBitmap;
    source->Assign(Image1->Picture->Bitmap);
    source->PixelFormat = Image1->Picture->Bitmap->PixelFormat;

    Graphics::TBitmap* dest = new Graphics::TBitmap;
    dest->Width = source->Width;
    dest->Height = source->Height;
    dest->PixelFormat = source->PixelFormat;

    /// ** de completat *///

    for (int y = 0; y < source->Height; y++) {
        for (int x = 0; x < dest->Width; x++) {
            RGBTRIPLE* pixels = (RGBTRIPLE*)source->ScanLine[y];
            double avd =
                (int)(0.3 * pixels[x].rgbtRed + 0.59 * pixels[x].rgbtGreen +
                      0.11 * pixels[x].rgbtBlue);
            dest->Canvas->Pixels[x][y] = TColor(RGB(avd, avd, avd));

            Hr[(int)(pixels[x].rgbtRed)]++;
            Hg[(int)(pixels[x].rgbtGreen)]++;
            Hb[(int)(pixels[x].rgbtBlue)]++;
        }
    }

    //Image2->Picture->Bitmap->Assign(dest);

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

    delete source;
    delete dest;
}

void __fastcall TForm1::Button7Click(TObject* Sender)
{
    // Histograma cumulativa

    Graphics::TBitmap* source = new Graphics::TBitmap;
    source->Assign(Image1->Picture->Bitmap);
    source->PixelFormat = Image1->Picture->Bitmap->PixelFormat;

    int L = 256, histoSursa[L];
    for (int i = 0; i < L; i++) {
        histoSursa[i] = 0;
    }

    RGBTRIPLE *pixelsS, pixelsD;
    for (int y = 0; y < source->Height; y++) {
        pixelsS = (RGBTRIPLE*)source->ScanLine[y];
        for (int x = 0; x < source->Width; x++) {
            double avd =
                (int)(0.3 * pixelsS[x].rgbtRed + 0.59 * pixelsS[x].rgbtGreen +
                      0.11 * pixelsS[x].rgbtBlue);
            histoSursa[(int)avd]++;
        }
    }

    long int LUT[L], histoCum[L];

    int total = source->Height * source->Width;
    histoCum[0] = histoSursa[0];
    for (int i = 1; i < L; i++) {
        histoCum[i] = histoCum[i - 1] + histoSursa[i];
    }

    Chart1->Series[0]->Clear();
    Chart1->Series[1]->Clear();
    Chart1->Series[2]->Clear();

    for (int i = 0; i < 256; i++) {
        Chart1->Series[0]->AddY(histoCum[i], "", clRed);
    }

    Chart1->Series[0]->Active = true;

    delete source;
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject* Sender)
{
    // select

    Graphics::TBitmap* imgs = new Graphics::TBitmap;
    imgs->Assign(Image1->Picture->Bitmap); ///org
    imgs->PixelFormat = Image1->Picture->Bitmap->PixelFormat;
    Graphics::TBitmap* imgd = new Graphics::TBitmap;
    int x1 = pt[cnt - 2].x;
    int y1 = pt[cnt - 2].y;
    int x2 = pt[cnt - 1].x;
    int y2 = pt[cnt - 1].y;

    imgd->Width = x2 - x1;
    ;
    imgd->Height = y2 - y1;
    imgd->PixelFormat = imgs->PixelFormat;

    RGBTRIPLE *pixelsS, *pixelsD;

    for (int y = y1; y < y2; y++) {
        if (y >= 0 && y < imgs->Height) {
            pixelsS = (RGBTRIPLE*)imgs->ScanLine[y];
            pixelsD = (RGBTRIPLE*)imgd->ScanLine[y - y1];
            for (int x = x1; x < x2; x++) {
                if (x >= 0 && x < imgs->Width) {
                    pixelsD[x - x1] = pixelsS[x];
                }
            }
        }
    }
    Image2->Picture->Bitmap = imgd;

    delete imgd;
    delete imgs;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject* Sender)
{
	/// ** de completat *///

	// FILTRU

	Graphics::TBitmap* src = new Graphics::TBitmap();
	if (!CheckBox1->Checked) {
		src->Assign(Image1->Picture->Bitmap);
		src->PixelFormat = Image1->Picture->Bitmap->PixelFormat;
	} else {
		src->Assign(Image2->Picture->Bitmap);
		src->PixelFormat = Image2->Picture->Bitmap->PixelFormat;
	}

    Graphics::TBitmap* dst = new Graphics::TBitmap();
	dst->Width = src->Width;
	dst->Height = src->Height;
	dst->PixelFormat = src->PixelFormat;
    double w[3][3] = { { 1.0 / 16, 2.0 / 16, 1.0 / 16 },
		{ 1.0 / 16, 4.0 / 16, 1.0 / 16 }, { 1.0 / 16, 2.0 / 16, 1.0 / 16 }

	};
	for (int y = 0; y < dst->Height; y++) {
		RGBTRIPLE* pixels = (RGBTRIPLE*)dst->ScanLine[y];
		for (int x = 0; x < dst->Width; x++) {
            int totalR = 0;
			int totalG = 0;
            int totalB = 0;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					int scX = x + j - 1;
					int scY = y + i - 1;
					if (scX >= 0 && scX < src->Width && scY >= 0 &&
						scY < src->Height) {
						RGBTRIPLE* pixelsS = (RGBTRIPLE*)src->ScanLine[scY];
						totalR += w[i][j] * pixelsS[scX].rgbtRed;
						totalG += w[i][j] * pixelsS[scX].rgbtGreen;
						totalB += w[i][j] * pixelsS[scX].rgbtBlue;
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

void __fastcall TForm1::Image1MouseDown(
	TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	/// ** de completat *///
	pt[cnt].X = X;
	pt[cnt].Y = Y;
	cnt++;

	Graphics::TBitmap* source = new Graphics::TBitmap;
	source->Assign(Image1->Picture->Bitmap);
    source->PixelFormat = Image1->Picture->Bitmap->PixelFormat;

    RGBTRIPLE* linie =
        (RGBTRIPLE*)
            source->ScanLine[Y * (float)source->Height / Image1->Height];

    Chart1->Series[0]->Clear();
    Chart1->Series[1]->Clear();
    Chart1->Series[2]->Clear();

    for (int i = 0; i < 256; i++) {
        Chart1->Series[0]->AddY(linie[i].rgbtRed, "", clRed);
        Chart1->Series[1]->AddY(linie[i].rgbtGreen, "", clGreen);
        Chart1->Series[2]->AddY(linie[i].rgbtBlue, "", clBlue);
    }

    Chart1->Series[0]->Active = true;
    Chart1->Series[1]->Active = true;
    Chart1->Series[2]->Active = true;

    delete source;
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject* Sender)
{
    /// ** de completat *///
    if (SaveDialog1->Execute()) {
        Image1->Picture->Bitmap->SaveToFile(SaveDialog1->FileName + ".bmp");
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject* Sender)
{
	/// ** de completat *///

    Graphics::TBitmap* src = new Graphics::TBitmap();
	if (!CheckBox1->Checked) {
		src->Assign(Image1->Picture->Bitmap);
		src->PixelFormat = Image1->Picture->Bitmap->PixelFormat;
	} else {
		src->Assign(Image2->Picture->Bitmap);
		src->PixelFormat = Image2->Picture->Bitmap->PixelFormat;
	}

    Graphics::TBitmap* dst = new Graphics::TBitmap();
	dst->Width = src->Width;
	dst->Height = src->Height;
	dst->PixelFormat = src->PixelFormat;
	double w[3][3] = { { 0.0, 0.0, 0.0},
		{ 1.0, 0.0, 1.0 }, { 0.0, -2.0, 0.0 }

	};
	for (int y = 0; y < dst->Height; y++) {
		RGBTRIPLE* pixels = (RGBTRIPLE*)dst->ScanLine[y];
		for (int x = 0; x < dst->Width; x++) {
            int totalR = 0;
			int totalG = 0;
            int totalB = 0;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					int scX = x + j - 1;
					int scY = y + i - 1;
					if (scX >= 0 && scX < src->Width && scY >= 0 &&
						scY < src->Height) {
						RGBTRIPLE* pixelsS = (RGBTRIPLE*)src->ScanLine[scY];
						totalR += w[i][j] * pixelsS[scX].rgbtRed;
						totalG += w[i][j] * pixelsS[scX].rgbtGreen;
						totalB += w[i][j] * pixelsS[scX].rgbtBlue;
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
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject* Sender)
{
	/// ** de completat *///

    // LUT + posterizare
    Graphics::TBitmap* src = new Graphics::TBitmap();
    if (!CheckBox1->Checked) {
        src->Assign(Image1->Picture->Bitmap);
        src->PixelFormat = Image1->Picture->Bitmap->PixelFormat;
    } else {
        src->Assign(Image2->Picture->Bitmap);
        src->PixelFormat = Image2->Picture->Bitmap->PixelFormat;
    }

    Graphics::TBitmap* dst = new Graphics::TBitmap();
    dst->Width = src->Width;
    dst->Height = src->Height;
    dst->PixelFormat = src->PixelFormat;

    int LUT[256];

    for (int i = 0; i < 256; i++) {
        LUT[i] = i - (i % 20);
    }

    for (int y = 0; y < src->Height; y++) {
        RGBTRIPLE* pixelsS = (RGBTRIPLE*)src->ScanLine[y];
        RGBTRIPLE* pixelsD = (RGBTRIPLE*)dst->ScanLine[y];
        for (int x = 0; x < src->Width; x++) {
            pixelsD[x].rgbtRed = LUT[pixelsS[x].rgbtRed];
            pixelsD[x].rgbtGreen = LUT[pixelsS[x].rgbtGreen];
            pixelsD[x].rgbtBlue = LUT[pixelsS[x].rgbtBlue];
        }
    }
    Image2->Picture->Bitmap->Assign(dst);

    // + negativizare
    if (CheckBox2->Checked) {
        for (int y = 0; y < src->Height; y++) {
            RGBTRIPLE* pixelsS = (RGBTRIPLE*)dst->ScanLine[y];

            for (int x = 0; x < src->Width; x++) {
                pixelsS[x].rgbtRed = 255 - pixelsS[x].rgbtRed;
                pixelsS[x].rgbtGreen = 255 - pixelsS[x].rgbtGreen;
                pixelsS[x].rgbtBlue = 255 - pixelsS[x].rgbtBlue;
            }
        }
        Image3->Picture->Bitmap->Assign(dst);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject* Sender)
{
    /// ** de completat *///

    Label4->Caption = "Info\n";
    Graphics::TBitmap* source = new Graphics::TBitmap;
    source->Assign(Image1->Picture->Bitmap);
    source->PixelFormat = source->PixelFormat;

    Label4->Caption += "Latime: ";
    Label4->Caption += source->Width;
    Label4->Caption += "\n";

    Label4->Caption += "Lungime: ";
    Label4->Caption += source->Height;
    Label4->Caption += "\n";

    Label4->Caption += "Pixel Format: ";
    Label4->Caption += source->PixelFormat;
    Label4->Caption += "\n";

    if (source->PixelFormat == pf1bit) {
        Label4->Caption += "1 bit";
    } else if (source->PixelFormat == pf4bit) {
        Label4->Caption += "4 bit";
    } else if (source->PixelFormat == pf8bit) {
        Label4->Caption += "8 bit";
    } else if (source->PixelFormat == pf16bit) {
        Label4->Caption += "16 bit";
    } else if (source->PixelFormat == pf24bit) {
        Label4->Caption += "24 bit";
    } else if (source->PixelFormat == pf32bit) {
        Label4->Caption += "32 bit";
    }

    Label4->Caption += "\n";
}
//---------------------------------------------------------------------------
