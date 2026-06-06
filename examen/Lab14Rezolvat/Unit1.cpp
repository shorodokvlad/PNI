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

    Graphics::TBitmap* imgs = new Graphics::TBitmap;
    imgs->Assign(Image1->Picture->Bitmap);
    imgs->PixelFormat = Image1->Picture->Bitmap->PixelFormat;
    Graphics::TBitmap* imgd = new Graphics::TBitmap;
    imgd->Width = imgs->Width;
    imgd->Height = imgs->Height;
    imgd->PixelFormat = imgs->PixelFormat;

    for (int y = 0; y < imgs->Height; y++) {
        for (int x = 0; x < imgs->Width; x++) {
            RGBTRIPLE* pixels = (RGBTRIPLE*)imgs->ScanLine[y];
            double avd =
                (int)(0.3 * pixels[x].rgbtRed + 0.11 * pixels[x].rgbtBlue +
                      0.59 * pixels[x].rgbtGreen);

            imgd->Canvas->Pixels[x][y] = TColor(RGB(avd, avd, avd));
            Hr[(int)(pixels[x].rgbtRed)]++;
            Hg[(int)(pixels[x].rgbtGreen)]++;
            Hb[(int)(pixels[x].rgbtBlue)]++;
        }
    }

    Image2->Picture->Bitmap->Assign(imgd);

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
    delete imgs;
    delete imgd;
}

void __fastcall TForm1::Button7Click(TObject* Sender)
{
    // Cumulative histogram
	Graphics::TBitmap* imgs = new Graphics::TBitmap;
    imgs->Assign(Image1->Picture->Bitmap);
    imgs->PixelFormat = Image1->Picture->Bitmap->PixelFormat;

	int L = 256, histoSursa[L];
    for (int i = 0; i < L; i++) {
        histoSursa[i] = 0;
    }
    RGBTRIPLE *pixelsS, *pixelsD;
    for (int y = 0; y < imgs->Height; y++) {
        pixelsS = (RGBTRIPLE*)imgs->ScanLine[y];
        for (int x = 0; x < imgs->Width; x++) {
            double avd =
                (int)(0.3 * pixelsS[x].rgbtRed + 0.11 * pixelsS[x].rgbtBlue +
                      0.59 * pixelsS[x].rgbtGreen);
            histoSursa[(int)avd]++;
        }
    }

    long int LUT[L], histoCum[L];

    int tot = imgs->Height * imgs->Width; //Nr. total de pixeli din imagine
    histoCum[0] = histoSursa[0];
    for (int i = 1; i < L; i++)
        histoCum[i] = histoCum[i - 1] + histoSursa[i];

	Chart1->Series[0]->Clear();
	Chart1->Series[1]->Clear();
	Chart1->Series[2]->Clear();
	for (int i = 0; i < 256; i++) {
		Chart1->Series[0]->AddY(histoCum[i], "", clRed);
	}
	Chart1->Series[0]->Active = true;

	delete imgs;
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject* Sender)
{
    // selected area
   int ZX1 = (int)pt[cnt - 2].x;
	int ZX2 = (int)pt[cnt - 1].x;
	int ZY1 = (int)pt[cnt - 2].y;
    int ZY2 = (int)pt[cnt - 1].y;
	///actualizare a noilor dimensiuni ale imaginii incarcata in image1
	Graphics::TBitmap* imgs = new Graphics::TBitmap;
	imgs->Assign(Image1->Picture->Bitmap); ///org
	imgs->PixelFormat = Image1->Picture->Bitmap->PixelFormat;
	Graphics::TBitmap* imgd = new Graphics::TBitmap;
	imgd->Width = ZX2 - ZX1;
	imgd->Height = ZY2 - ZY1;
	imgd->PixelFormat = imgs->PixelFormat;

	RGBTRIPLE *pixelsS, *pixelsD;

	for (int y = ZY1; y < ZY2; y++) {
		if (y >= 0 && y < imgs->Height) {
			pixelsS = (RGBTRIPLE*)imgs->ScanLine[y];
			pixelsD = (RGBTRIPLE*)imgd->ScanLine[y - ZY1];
			for (int x = ZX1; x < ZX2; x++) {
				if (x >= 0 && x < imgs->Width) {
					pixelsD[x - ZX1] = pixelsS[x];
				}
			}
		}
	}
	Image3->Picture->Bitmap->Assign(imgd);
	delete imgd;
	delete imgs;}

void __fastcall TForm1::Image1MouseDown(
    TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    pt[cnt].X = X;
    pt[cnt].Y = Y;
    cnt++;

    if (Image1->Picture->Graphic->Empty == true)
        return;
	Graphics::TBitmap* imgs = new Graphics::TBitmap;
	imgs->Assign(Image1->Picture->Bitmap);
	imgs->PixelFormat = Image1->Picture->Bitmap->PixelFormat;
	RGBTRIPLE* pixels;
    pixels =
		(RGBTRIPLE*)
			imgs->ScanLine[Y * (float)imgs->Height / Image1->Height];
	Chart1->Series[0]->Clear();
	Chart1->Series[1]->Clear();
	Chart1->Series[2]->Clear();
	for (int i = 0; i < imgs->Width; i++) {
		Chart1->Series[0]->AddY(pixels[i].rgbtRed, "", clRed);
		Chart1->Series[1]->AddY(pixels[i].rgbtGreen, "", clGreen);
		Chart1->Series[2]->AddY(pixels[i].rgbtBlue, "", clBlue);
    }
	Chart1->Series[0]->Active = true;
	Chart1->Series[1]->Active = true;
	Chart1->Series[2]->Active = true;
	delete imgs;
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject* Sender)
{
	/// un alt save
    	if (SaveDialog1->Execute()) {
		Image2->Picture->Bitmap->SaveToFile(SaveDialog1->FileName+".bmp");
	}


	/*AnsiString Ext;
    // Este necesar sa includem fisierul "jpeg.hpp"
    SaveDialog1->Filter =
        "Bmp files (*.bmp)|*.BMP| JPEG images (*.jpg;*.jpeg)|*.jpg;*; jpeg ";
    SaveDialog1->Title = "Save Image";
    SaveDialog1->DefaultExt = "jpg";
    SaveDialog1->FilterIndex = 1;
    if (SaveDialog1->Execute()) {
        AnsiString temp = ExtractFileExt(SaveDialog1->FileName);
        Ext = temp.LowerCase();
    }
    if (Ext.AnsiPos("jpg") > 0) {
        TJPEGImage* jp = new TJPEGImage();
        try {
            jp->Assign(Image3->Picture->Bitmap);
            jp->SaveToFile(SaveDialog1->FileName);
        } __finally
        {
            delete jp;
        }
    } else if (Ext.AnsiPos("bmp") > 0)
		Image3->Picture->Bitmap->SaveToFile(SaveDialog1->FileName); */
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject* Sender)
{
    // Edge Detection
	Graphics::TBitmap* imgs = new Graphics::TBitmap;
    if (CheckBox1->Checked) {
        imgs->Assign(Image3->Picture->Bitmap);
        imgs->PixelFormat = Image3->Picture->Bitmap->PixelFormat;
    } else {
        imgs->Assign(Image1->Picture->Bitmap);
        imgs->PixelFormat = Image1->Picture->Bitmap->PixelFormat;
    }

    Graphics::TBitmap* imgd = new Graphics::TBitmap;
    imgd->Width = imgs->Width;
    imgd->Height = imgs->Height;
    imgd->PixelFormat = imgs->PixelFormat;

    int gx[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };
    int gy[3][3] = { { 1, 0, -1 }, { 2, 0, -2 }, { 1, 0, -1 } };
    for (int y = 1; y < imgs->Height - 1; y++) {
        RGBTRIPLE* psD = (RGBTRIPLE*)imgd->ScanLine[y];
        for (int x = 1; x < imgs->Width - 1; x++) {
            int gxR = 0, gxG = 0, gxB = 0;
            int gyR = 0, gyG = 0, gyB = 0;
            for (int i = 0; i < 3; i++) {
                RGBTRIPLE* ps = (RGBTRIPLE*)imgs->ScanLine[y - 1 + i];
                for (int j = 0; j < 3; j++) {
                    int nx = x - 1 + j;
                    int ny = y - 1 + i;
                    gxR += ps[nx].rgbtRed * gx[i][j];
                    gxG += ps[nx].rgbtGreen * gx[i][j];
                    gxB += ps[nx].rgbtBlue * gx[i][j];
                    gyR += ps[nx].rgbtRed * gy[i][j];
                    gyG += ps[nx].rgbtGreen * gy[i][j];
                    gyB += ps[nx].rgbtBlue * gy[i][j];
                }
            }
            int red = std::min(255, (int)sqrt(gxR * gxR + gyR * gyR));
            int green = std::min(255, (int)sqrt(gxG * gxG + gyG * gyG));
            int blue = std::min(255, (int)sqrt(gxB * gxB + gyB * gyB));
            psD[x].rgbtRed = 255 - red;
            psD[x].rgbtGreen = 255 - green;
            psD[x].rgbtBlue = 255 - blue;
        }
    }
    Image2->Picture->Bitmap->Assign(imgd);
    delete imgs;
    delete imgd;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject* Sender)
{
    // Posterization
	Graphics::TBitmap* imgs = new Graphics::TBitmap;
    if (CheckBox1->Checked) {
        imgs->Assign(Image3->Picture->Bitmap);
        imgs->PixelFormat = Image3->Picture->Bitmap->PixelFormat;
    } else {
        imgs->Assign(Image1->Picture->Bitmap);
        imgs->PixelFormat = Image1->Picture->Bitmap->PixelFormat;
    }

    Graphics::TBitmap* imgd = new Graphics::TBitmap;
    imgd->Width = imgs->Width;
    imgd->Height = imgs->Height;
    imgd->PixelFormat = imgs->PixelFormat;
	int L = 256;
    long int LUT[L];
	int xt = Edit1->Text.ToIntDef(0);
    for (int i = 0; i < L; i++) {
        LUT[i] = i - (i % xt);
    }
    RGBTRIPLE *pixelsS, *pixelsD;
    for (int y = 0; y < imgd->Height; y++) {
        pixelsS = (RGBTRIPLE*)imgs->ScanLine[y];
        pixelsD = (RGBTRIPLE*)imgd->ScanLine[y];
        for (int x = 0; x < imgd->Width; x++) {
            if (CheckBox2->Checked) {
                pixelsD[x].rgbtRed = 255 - LUT[pixelsS[x].rgbtRed];
                pixelsD[x].rgbtGreen = 255 - LUT[pixelsS[x].rgbtGreen];
                pixelsD[x].rgbtBlue = 255 - LUT[pixelsS[x].rgbtBlue];
            } else {
                pixelsD[x].rgbtRed = LUT[pixelsS[x].rgbtRed];
                pixelsD[x].rgbtGreen = LUT[pixelsS[x].rgbtGreen];
                pixelsD[x].rgbtBlue = LUT[pixelsS[x].rgbtBlue];
            }
        }
    }

    Image2->Picture->Bitmap->Assign(imgd);
    delete imgs;
    delete imgd;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject* Sender)
{
    // info
	Label4->Caption = "Info:\n";
    Graphics::TBitmap* imgs = new Graphics::TBitmap;
    imgs->Assign(Image1->Picture->Bitmap);
    imgs->PixelFormat = imgs->PixelFormat;
    Label4->Caption += " Latime ";
    Label4->Caption += imgs->Width;
    Label4->Caption += "\n";
    Label4->Caption += " Lungime ";
    Label4->Caption += imgs->Height;
    Label4->Caption += "\n";

    Label4->Caption += " Pixel Format ";
    Label4->Caption += imgs->PixelFormat;
	Label4->Caption += "\n";

    String pixelFormatStr;
	switch (imgs->PixelFormat) {
		case pf1bit:
			Label4->Caption += "1-bit";
			break;
		case pf4bit:
			Label4->Caption += "4-bit";
			break;
		case pf8bit:
			Label4->Caption += "8-bit";
			break;
		case pf16bit:
			Label4->Caption += "16-bit";
			break;
		case pf24bit:
			Label4->Caption += "24-bit";
			break;
		case pf32bit:
			Label4->Caption += "32-bit";
			break;
		default:
			Label4->Caption += "Necunoscut";
			break;
	}
	Label4->Caption += "\n";

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button9Click(TObject *Sender)
{
   // Simple select
	Graphics::TBitmap* imgs = new Graphics::TBitmap;
	imgs->Assign(Image1->Picture->Bitmap); ///org
	imgs->PixelFormat = Image1->Picture->Bitmap->PixelFormat;
	Graphics::TBitmap* imgd = new Graphics::TBitmap;
	int x1 = pt[cnt - 2].x;
	int y1 = pt[cnt - 2].y;
	int x2 = pt[cnt - 1].x;
	int y2 = pt[cnt - 1].y;

	imgd->Width = x2-x1;;
	imgd->Height = y2-y1;
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
	Image3->Picture->Bitmap = imgd;

	/*
	 Aceasta metoda nu va functiona corect pentru cazul in care rezolutia imaginii
	 este mai mare decat rezolutia componentei imagine

	*/

	//
	// decomentam pentru zoom
	/** TRect ZoomRect =
		Rect(pt[cnt - 2].x, pt[cnt - 2].y, pt[cnt - 1].x, pt[cnt - 1].y);
	Graphics::TBitmap* ZoomBitmap = new Graphics::TBitmap;
    // de ex: zoom out -> *1/2, zoom in -> *2
	ZoomBitmap->Width = (pt[cnt - 1].x - pt[cnt - 2].x)*1.0/2;//*2;
	ZoomBitmap->Height = (pt[cnt - 1].y - pt[cnt - 2].y)*1.0/2;//*2;
	ZoomBitmap->PixelFormat = Image2->Picture->Bitmap->PixelFormat;
	ZoomBitmap->Canvas->StretchDraw(
		TRect(0, 0, ZoomBitmap->Width, ZoomBitmap->Height), imgd);

	Image3->Picture->Bitmap = ZoomBitmap;   */

	delete imgd;
	delete imgs;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
    // Mean Filtering
}
//---------------------------------------------------------------------------
