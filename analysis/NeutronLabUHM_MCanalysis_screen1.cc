#include "TROOT.h"
#include "TSystem.h"
#include "TProof.h"
#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"
#include "TChain.h"
#include "TObject.h"
#include "TColor.h"
#include "TMath.h"
#include "TRandom.h"
#include "TChain.h"
#include "TPad.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TH2Poly.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TCut.h"
#include "TList.h"
#include "TLine.h"
#include "TMarker.h"
#include "TPaletteAxis.h"
#include "TGraph.h"
#include "TF1.h"
#include "TPaveStats.h"
#include "TText.h"
#include "TEllipse.h"

#include "vector"
#include "string"
#include "iostream"
#include "fstream"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

using namespace std;

void NeutronLabUHM_MCanalysis_v1()
{	
	gStyle	->SetOptStat(0);
	gStyle	->SetOptFit(0);
	//gStyle	->SetOptFit(1);

	//------ define color gradinet
	const Int_t NRGBs = 5;
    const Int_t NCont = 255;

    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
	//------

	gStyle->SetStatBorderSize(0);
	gStyle->SetTitleBorderSize(0);
	gStyle->SetTitleFillColor(10);
	gStyle->SetStatColor(10);
	gStyle->SetStatFont(42);
	
	//gStyle->SetMarkerStyle(7);
	gStyle->SetMarkerColor(1);
	
	//gStyle->SetCanvasPreferGL(kTRUE);
	gStyle->SetCanvasPreferGL(kFALSE);

	////////////////////////////////////////////////////////////////////////
	// MC
	const char *DataFolder 	= "/Users/alexkish1/Documents/darkside/GEANT4/NeutronLabUHM/NeutronLabUHM_idl/macros";
	const char *ext  		= ".root";
	const char *outpath     = "./plots";
	const char *extPlots     = ".png";

	//const char *FileName 	= "neutronISO-DD_confine-EmissionPoint_1e6_screen";
	const char *FileName 	= "neutronISO-DT_confine-EmissionPoint_1e6_screen";
	double nsim = 1e6;

	char DataFile[500];
	sprintf(DataFile, "%s/%s%s", DataFolder, FileName, ext);			

	// Load data with TChain
	TChain *t1	= new TChain("t1");
			t1	->Add(DataFile);

	// Switch off all branches, then load the required ones
	t1->SetBranchStatus("*", 		1);
	t1->SetBranchStatus("ekin_pre", 1);
	t1->SetBranchStatus("type", 	1);
	t1->SetBranchStatus("xp", 		1);
	t1->SetBranchStatus("yp", 		1);
	t1->SetBranchStatus("zp", 		1);
	t1->SetBranchStatus("mX_pre", 	1);
	t1->SetBranchStatus("mY_pre", 	1);
	t1->SetBranchStatus("mZ_pre", 	1);
	t1->SetBranchStatus("time", 	1);

	t1->SetAlias("time_ms", "time*1000");
	t1->SetAlias("time_us", "time*1000000");

	const double xzero = -660.4;
	const double yzero = 8907.65;
	const double zzero = -367.524;
	//const double xzero = -650;
	//const double yzero = 0;
	//const double zzero = -350;

	const double xsize = 1352.71;
	const double ysize = 1552.71;
	const double zsize = 1576.35;
	
	t1->SetAlias("xp_zero", "xp - xzero");
	t1->SetAlias("yp_zero", "yp - yzero");
	t1->SetAlias("zp_zero", "zp - zzero");

	// Define cuts
	TCut cut_neutron 	= "type==\"neutron\"";
	TCut cut_gamma 		= "type==\"gamma\"";
	TCut cut_front 		= "yp>9660";
	TCut cut_forward 	= "mY_pre>0";
	TCut cut_backward 	= "mY_pre<0";
	TCut cut_exithole 	= "xp>-710.4 && xp<-610.4 && zp>-317.524 && zp<-217.524";
	TCut cut_fast 		= "time_us>0.5";


	// energy spectra

	// neutrons
	int nbins_Eneutron 	= 200;
	double min_Eneutron	= 0;
	double max_Eneutron	= 15000;
	double binwidth_Eneutron = (max_Eneutron-min_Eneutron)/nbins_Eneutron;

	TH1F *h1_1 = new TH1F("h1_1", "", nbins_Eneutron, min_Eneutron, max_Eneutron);
		  h1_1->SetLineColor(kBlack);
		  h1_1->SetLineWidth(3);
	TH1F *h1_2 = new TH1F("h1_2", "", nbins_Eneutron, min_Eneutron, max_Eneutron);
		  h1_2->SetLineColor(kRed+2);
		  h1_2->SetLineWidth(3);
	TH1F *h1_3 = new TH1F("h1_3", "", nbins_Eneutron, min_Eneutron, max_Eneutron);
		  h1_3->SetLineColor(kGreen+2);
		  h1_3->SetLineWidth(3);

	int nbins_EneutronLE 	= 100;
	double min_EneutronLE	= 0;
	double max_EneutronLE	= 200;
	double binwidth_EneutronLE = (max_EneutronLE-min_EneutronLE)/nbins_EneutronLE;

	TH1F *h1_1le = new TH1F("h1_1le", "", nbins_EneutronLE, min_EneutronLE, max_EneutronLE);
		  h1_1le->SetLineColor(kBlack);
		  h1_1le->SetLineWidth(3);
	TH1F *h1_2le = new TH1F("h1_2le", "", nbins_EneutronLE, min_EneutronLE, max_EneutronLE);
		  h1_2le->SetLineColor(kRed+2);
		  h1_2le->SetLineWidth(3);
	TH1F *h1_3le = new TH1F("h1_3le", "", nbins_EneutronLE, min_EneutronLE, max_EneutronLE);
		  h1_3le->SetLineColor(kGreen+2);
		  h1_3le->SetLineWidth(3);

	TCanvas *c1 = new TCanvas("c1", "c1", 0, 0, 700, 500);
			 c1->SetFillColor(10);
    		 c1->SetLogy();
		t1	->	Draw("ekin_pre>>h1_1", cut_neutron && cut_front && cut_fast, "");
		t1	->	Draw("ekin_pre>>h1_2", cut_neutron && cut_front && cut_forward && cut_fast, "same");
		t1	->	Draw("ekin_pre>>h1_3", cut_neutron && cut_front && cut_backward && cut_fast, "same");
		h1_1 ->SetMinimum(1);
		h1_1	->	GetXaxis()	->	SetTitle("Neutron kinetic energy [keV]");
		h1_1	->	GetYaxis()	->	SetTitle("Counts");
		h1_1	-> 	GetXaxis() 	-> 	CenterTitle(true);
		h1_1	-> 	GetYaxis() 	-> 	CenterTitle(true);
	Double_t xl1=.5, yl1=0.65, xl2=xl1+.3, yl2=yl1+.175;
	TLegend *leg1 = new TLegend(xl1,yl1,xl2,yl2,NULL,"brNDC");
			 leg1 ->SetBorderSize(0);
			 leg1 ->SetFillColor(10);
			 leg1 ->AddEntry(h1_1,	"all front neutrons",	"l");
			 leg1 ->AddEntry(h1_2,	"forward neutrons",		"l");
			 leg1 ->AddEntry(h1_3,	"reflected neutrons",	"l");
			 leg1 ->Draw();
	c1->Update();

	double n1_1 = h1_1->GetEntries();
	double n1_2 = h1_2->GetEntries();
	double n1_3 = h1_3->GetEntries();
	cout <<"N all neutrons       = "<< n1_1 << endl;
	cout <<"N forward neutrons   = "<< n1_2 << endl;
	cout <<"N scattered neutrons = "<< n1_3 << endl;

	TCanvas *c1le = new TCanvas("c1le", "c1le", 0, 0, 700, 500);
			 c1le->SetFillColor(10);
    		 c1le->SetLogy();
		t1	->	Draw("ekin_pre>>h1_1le", cut_neutron && cut_front && cut_fast, "");
		t1	->	Draw("ekin_pre>>h1_2le", cut_neutron && cut_front && cut_fast, "same");
		t1	->	Draw("ekin_pre>>h1_3le", cut_neutron && cut_front && cut_backward && cut_fast, "same");
		h1_1le ->SetMinimum(1);
		h1_1le	->	GetXaxis()	->	SetTitle("Neutron kinetic energy [keV]");
		h1_1le	->	GetYaxis()	->	SetTitle("Counts");
		h1_1le	-> 	GetXaxis() 	-> 	CenterTitle(true);
		h1_1le	-> 	GetYaxis() 	-> 	CenterTitle(true);
	Double_t xl1le=.5, yl1le=0.65, xl2le=xl1le+.3, yl2le=yl1le+.175;
	TLegend *leg1le = new TLegend(xl1le,yl1le,xl2le,yl2le,NULL,"brNDC");
			 leg1le ->SetBorderSize(0);
			 leg1le ->SetFillColor(10);
			 leg1le ->AddEntry(h1_1le,	"all front neutrons",	"l");
			 leg1le ->AddEntry(h1_2le,	"forward neutrons",		"l");
			 leg1le ->AddEntry(h1_3le,	"reflected neutrons",	"l");
			 leg1le ->Draw();
	c1le->Update();


	// gamma
	int nbins_Egamma 	= 750;
	double min_Egamma	= 0;
	double max_Egamma	= 15000;
	double binwidth_Egamma = (max_Egamma-min_Egamma)/nbins_Egamma;

	TH1F *h1g_1 = new TH1F("h1g_1", "", nbins_Egamma, min_Egamma, max_Egamma);
		  h1g_1->SetLineColor(kBlack);
		  h1g_1->SetLineWidth(3);
	TH1F *h1g_2 = new TH1F("h1g_2", "", nbins_Egamma, min_Egamma, max_Egamma);
		  h1g_2->SetLineColor(kRed+2);
		  h1g_2->SetLineWidth(3);
	TH1F *h1g_3 = new TH1F("h1g_3", "", nbins_Egamma, min_Egamma, max_Egamma);
		  h1g_3->SetLineColor(kGreen+2);
		  h1g_3->SetLineWidth(3);

	int nbins_EgammaLE 	= 200;
	double min_EgammaLE	= 0;
	double max_EgammaLE	= 200;
	double binwidth_EgammaLE = (max_EgammaLE-min_EgammaLE)/nbins_EgammaLE;

	TH1F *h1g_1le = new TH1F("h1g_1le", "", nbins_EgammaLE, min_EgammaLE, max_EgammaLE);
		  h1g_1le->SetLineColor(kBlack);
		  h1g_1le->SetLineWidth(3);
	TH1F *h1g_2le = new TH1F("h1g_2le", "", nbins_EgammaLE, min_EgammaLE, max_EgammaLE);
		  h1g_2le->SetLineColor(kRed+2);
		  h1g_2le->SetLineWidth(3);
	TH1F *h1g_3le = new TH1F("h1g_3le", "", nbins_EgammaLE, min_EgammaLE, max_EgammaLE);
		  h1g_3le->SetLineColor(kGreen+2);
		  h1g_3le->SetLineWidth(3);

	TCanvas *c2 = new TCanvas("c2", "c2", 0, 0, 700, 500);
			 c2->SetFillColor(10);
    		 c2->SetLogy();
		t1	->	Draw("ekin_pre>>h1g_1", cut_gamma && cut_front && cut_fast, "");
		t1	->	Draw("ekin_pre>>h1g_2", cut_gamma && cut_front && cut_forward && cut_fast, "same");
		t1	->	Draw("ekin_pre>>h1g_3", cut_gamma && cut_front && cut_backward && cut_fast, "same");
		h1g_1 ->SetMinimum(1);
		h1g_1	->	GetXaxis()	->	SetTitle("Gamma kinetic energy [keV]");
		h1g_1	->	GetYaxis()	->	SetTitle("Counts");
		h1g_1	-> 	GetXaxis() 	-> 	CenterTitle(true);
		h1g_1	-> 	GetYaxis() 	-> 	CenterTitle(true);
	Double_t xl21=.5, yl21=0.65, xl22=xl21+.3, yl22=yl21+.175;
	TLegend *leg2 = new TLegend(xl21,yl21,xl22,yl22,NULL,"brNDC");
			 leg2 ->SetBorderSize(0);
			 leg2 ->SetFillColor(10);
			 leg2 ->AddEntry(h1g_1,	"all front gammas",	"l");
			 leg2 ->AddEntry(h1g_2,	"forward gammas",	"l");
			 leg2 ->AddEntry(h1g_3,	"reflected gammas",	"l");
			 leg2 ->Draw();
	c2->Update();

	double n1g_1 = h1g_1->GetEntries();
	double n1g_2 = h1g_2->GetEntries();
	double n1g_3 = h1g_3->GetEntries();
	cout <<"N all gammas       = "<< n1g_1 << endl;
	cout <<"N forward gammas   = "<< n1g_2 << endl;
	cout <<"N scattered gammas = "<< n1g_3 << endl;

	TCanvas *c2le = new TCanvas("c2le", "c2le", 0, 0, 700, 500);
			 c2le->SetFillColor(10);
    		 c2le->SetLogy();
		t1	->	Draw("ekin_pre>>h1g_1le", cut_gamma && cut_front && cut_fast, "");
		t1	->	Draw("ekin_pre>>h1g_2le", cut_gamma && cut_front && cut_forward && cut_fast, "same");
		t1	->	Draw("ekin_pre>>h1g_3le", cut_gamma && cut_front && cut_backward && cut_fast, "same");
		h1g_1le ->SetMinimum(1);
		h1g_1le	->	GetXaxis()	->	SetTitle("Gamma kinetic energy [keV]");
		h1g_1le	->	GetYaxis()	->	SetTitle("Counts");
		h1g_1le	-> 	GetXaxis() 	-> 	CenterTitle(true);
		h1g_1le	-> 	GetYaxis() 	-> 	CenterTitle(true);
	Double_t xl21le=.5, yl21le=0.65, xl22le=xl21le+.3, yl22le=yl21le+.175;
	TLegend *leg2le = new TLegend(xl21le,yl21le,xl22le,yl22le,NULL,"brNDC");
			 leg2le ->SetBorderSize(0);
			 leg2le ->SetFillColor(10);
			 leg2le ->AddEntry(h1g_1le,	"all front gammas",	"l");
			 leg2le ->AddEntry(h1g_2le,	"forward gammas",	"l");
			 leg2le ->AddEntry(h1g_3le,	"reflected gammas",	"l");
			 leg2le ->Draw();
	c2le->Update();

	// time
	int nbins_time 	= 300;
	double min_time	= 0;
	double max_time	= 3;
	double binwidth_time = (max_time-min_time)/nbins_time;

	TH1F *h1time_1 = new TH1F("h1time_1", "", nbins_time, min_time, max_time);
		  h1time_1->SetLineColor(kBlack);
		  h1time_1->SetLineWidth(3);
	TH1F *h1time_2 = new TH1F("h1time_2", "", nbins_time, min_time, max_time);
		  h1time_2->SetLineColor(kRed+2);
		  h1time_2->SetLineWidth(3);
	TH1F *h1time_3 = new TH1F("h1time_3", "", nbins_time, min_time, max_time);
		  h1time_3->SetLineColor(kGreen+2);
		  h1time_3->SetLineWidth(3);

	int nbins_timeFast 	= 100;
	double min_timeFast	= 0;
	double max_timeFast	= 1;
	double binwidth_timeFast = (max_timeFast-min_timeFast)/nbins_timeFast;

	TH1F *h1time_1fast = new TH1F("h1time_1fast", "", nbins_timeFast, min_timeFast, max_timeFast);
		  h1time_1fast->SetLineColor(kBlack);
		  h1time_1fast->SetLineWidth(3);
	TH1F *h1time_2fast = new TH1F("h1time_2fast", "", nbins_timeFast, min_timeFast, max_timeFast);
		  h1time_2fast->SetLineColor(kRed+2);
		  h1time_2fast->SetLineWidth(3);
	TH1F *h1time_3fast = new TH1F("h1time_3fast", "", nbins_timeFast, min_timeFast, max_timeFast);
		  h1time_3fast->SetLineColor(kGreen+2);
		  h1time_3fast->SetLineWidth(3);

	TCanvas *ctime = new TCanvas("ctime", "ctime", 0, 0, 700, 500);
			 ctime->SetFillColor(10);
    		 ctime->SetLogy();
		t1	->	Draw("time_ms>>h1time_1", cut_neutron && cut_front && cut_fast, "");
		t1	->	Draw("time_ms>>h1time_2", cut_neutron && cut_front && cut_forward && cut_fast, "same");
		t1	->	Draw("time_ms>>h1time_3", cut_neutron && cut_front && cut_backward && cut_fast, "same");
		h1time_1 ->SetMinimum(1);
		h1time_1	->	GetXaxis()	->	SetTitle("Neutron arrival time [ms]");
		h1time_1	->	GetYaxis()	->	SetTitle("Counts");
		h1time_1	-> 	GetXaxis() 	-> 	CenterTitle(true);
		h1time_1	-> 	GetYaxis() 	-> 	CenterTitle(true);
	Double_t xltime1=.5, yltime1=0.65, xltime2=xltime1+.3, yltime2=yltime1+.175;
	TLegend *legTime = new TLegend(xltime1,yltime1,xltime2,yltime2,NULL,"brNDC");
			 legTime ->SetBorderSize(0);
			 legTime ->SetFillColor(10);
			 legTime ->AddEntry(h1time_1,	"all front neutrons",	"l");
			 legTime ->AddEntry(h1time_2,	"forward neutrons",		"l");
			 legTime ->AddEntry(h1time_3,	"reflected neutrons",	"l");
			 legTime ->Draw();
	ctime->Update();

	TCanvas *ctimeFast = new TCanvas("ctimeFast", "ctimeFast", 0, 0, 700, 500);
			 ctimeFast->SetFillColor(10);
    		 ctimeFast->SetLogy();
		t1	->	Draw("time_us>>h1time_1fast", cut_neutron && cut_front && cut_fast, "");
		t1	->	Draw("time_us>>h1time_2fast", cut_neutron && cut_front && cut_forward && cut_fast, "same");
		t1	->	Draw("time_us>>h1time_3fast", cut_neutron && cut_front && cut_backward && cut_fast, "same");
		h1time_1fast ->SetMinimum(1);
		h1time_1fast	->	GetXaxis()	->	SetTitle("Neutron arrival time [#mus]");
		h1time_1fast	->	GetYaxis()	->	SetTitle("Counts");
		h1time_1fast	-> 	GetXaxis() 	-> 	CenterTitle(true);
		h1time_1fast	-> 	GetYaxis() 	-> 	CenterTitle(true);
	Double_t xltimeFast1=.5, yltimeFast1=0.65, xltimeFast2=xltimeFast1+.3, yltimeFast2=yltimeFast1+.175;
	TLegend *legTimeFast = new TLegend(xltimeFast1,yltimeFast1,xltimeFast2,yltimeFast2,NULL,"brNDC");
			 legTimeFast ->SetBorderSize(0);
			 legTimeFast ->SetFillColor(10);
			 legTimeFast ->AddEntry(h1time_1fast,	"all front neutrons",	"l");
			 legTimeFast ->AddEntry(h1time_2fast,	"forward neutrons",		"l");
			 legTimeFast ->AddEntry(h1time_3fast,	"reflected neutrons",	"l");
			 legTimeFast ->Draw();
	ctimeFast->Update();


	// spatial distribution
	double nbins_xp = 100;
	double min_xp	= -850;
	double max_xp	= 850;
	double nbins_zp = 100;
	double min_zp	= -850;
	double max_zp	= 850;

	TH2F *h2_xz1 = new TH2F("h2_xz1", "", nbins_xp, min_xp, max_xp, nbins_zp, min_zp, max_zp);
	TH2F *h2_xz2 = new TH2F("h2_xz2", "", nbins_xp, min_xp, max_xp, nbins_zp, min_zp, max_zp);
	TH2F *h2_xz3 = new TH2F("h2_xz3", "", nbins_xp, min_xp, max_xp, nbins_zp, min_zp, max_zp);

	TCanvas *cxz1 = new TCanvas("cxz1", "cxz1", 0, 0, 700, 700);
			 cxz1->SetFillColor(10);
		t1	->	Draw("zp+250:xp+650>>h2_xz1", cut_neutron && cut_front && cut_fast, "COLZ");
		h2_xz1	->	GetXaxis()	->	SetTitle("X [mm]");
		h2_xz1	->	GetYaxis()	->	SetTitle("Z [mm]");
		h2_xz1	-> 	GetXaxis() 	-> 	CenterTitle(true);
		h2_xz1	-> 	GetYaxis() 	-> 	CenterTitle(true);
		h2_xz1	-> 	GetYaxis() 	-> 	SetTitleOffset(1.25);
	cxz1->Update();

	TCanvas *cxz2 = new TCanvas("cxz2", "cxz2", 0, 0, 700, 700);
			 cxz2->SetFillColor(10);
		t1	->	Draw("zp+250:xp+650>>h2_xz2", cut_neutron && cut_front && cut_forward && cut_fast, "COLZ");
		h2_xz2	->	GetXaxis()	->	SetTitle("X [mm]");
		h2_xz2	->	GetYaxis()	->	SetTitle("Z [mm]");
		h2_xz2	-> 	GetXaxis() 	-> 	CenterTitle(true);
		h2_xz2	-> 	GetYaxis() 	-> 	CenterTitle(true);
		h2_xz2	-> 	GetYaxis() 	-> 	SetTitleOffset(1.25);
	cxz2->Update();

	TCanvas *cxz3 = new TCanvas("cxz3", "cxz3", 0, 0, 700, 700);
			 cxz3->SetFillColor(10);
		t1	->	Draw("zp+250:xp+650>>h2_xz3", cut_neutron && cut_front && cut_backward && cut_fast, "COLZ");
		h2_xz3	->	GetXaxis()	->	SetTitle("X [mm]");
		h2_xz3	->	GetYaxis()	->	SetTitle("Z [mm]");
		h2_xz3	-> 	GetXaxis() 	-> 	CenterTitle(true);
		h2_xz3	-> 	GetYaxis() 	-> 	CenterTitle(true);
		h2_xz3	-> 	GetYaxis() 	-> 	SetTitleOffset(1.25);
	cxz3->Update();


	// x
	int nbins_r 	= 300;
	double min_r	= -500;
	double max_r	= 500;

	TH1F *h1_r1 = new TH1F("h1_r1", "", nbins_r, min_r, max_r);
		  h1_r1->SetLineColor(kBlack);
		  h1_r1->SetLineWidth(3);
	TH1F *h1_r2 = new TH1F("h1_r2", "", nbins_r, min_r, max_r);
		  h1_r2->SetLineColor(kRed+2);
		  h1_r2->SetLineWidth(3);
	TH1F *h1_r3 = new TH1F("h1_r3", "", nbins_r, min_r, max_r);
		  h1_r3->SetLineColor(kGreen+2);
		  h1_r3->SetLineWidth(3);

	TCanvas *cr1 = new TCanvas("cr1", "cr1", 0, 0, 700, 500);
			 cr1->SetFillColor(10);
    		 //cr1->SetLogy();
		//t1	->	Draw("sqrt((xp+650)*(xp+650)+(zp+250)*(zp+250))>>h1_r1", cut_neutron && cut_front, "");
		//t1	->	Draw("sqrt((xp+650)*(xp+650)+(zp+250)*(zp+250))>>h1_r1", cut_neutron && cut_front && cut_forward, "same");
		//t1	->	Draw("sqrt((xp+650)*(xp+650)+(zp+250)*(zp+250))>>h1_r1", cut_neutron && cut_front && cut_backward, "same");
		t1	->	Draw("sqrt(xp*xp+zp*zp)-700>>h1_r1", cut_neutron && cut_front && cut_fast, "");
		t1	->	Draw("sqrt(xp*xp+zp*zp)-700>>h1_r2", cut_neutron && cut_front && cut_forward && cut_fast, "same");
		t1	->	Draw("sqrt(xp*xp+zp*zp)-700>>h1_r3", cut_neutron && cut_front && cut_backward && cut_fast, "same");
		h1_r1 ->SetMinimum(1);
		h1_r1	->	GetXaxis()	->	SetTitle("Radial beam spread [mm]");
		h1_r1	->	GetYaxis()	->	SetTitle("Counts");
		h1_r1	-> 	GetXaxis() 	-> 	CenterTitle(true);
		h1_r1	-> 	GetYaxis() 	-> 	CenterTitle(true);
	Double_t xlr1=.55, ylr1=0.7, xlr2=xlr1+.3, ylr2=ylr1+.175;
	TLegend *leg_cr1 = new TLegend(xlr1,ylr1,xlr2,ylr2,NULL,"brNDC");
			 leg_cr1 ->SetBorderSize(0);
			 leg_cr1 ->SetFillColor(10);
			 leg_cr1 ->AddEntry(h1_r1,	"all front neutrons",	"l");
			 leg_cr1 ->AddEntry(h1_r2,	"forward neutrons",		"l");
			 leg_cr1 ->AddEntry(h1_r3,	"reflected neutrons",	"l");
			 leg_cr1 ->Draw();
	cr1->Update();


	//////////////////////////////////////////////////////////////////////////////////////
	// save plots
	const char *cutname 	= "FullFrontScreen_Slow500ns";
	cout <<"cutname = "<< cutname << endl;

	const char *ext_plots 	= "png";
	char char_outpathPre[500];
	sprintf(char_outpathPre, "./plots/%s", FileName);
	char char_outpath[500];
	sprintf(char_outpath, "%s/%s", char_outpathPre, cutname);

	std::stringstream cmd1(""); //Used to submit commands to the shell
	cmd1.str("");
	cmd1 << "mkdir "<< char_outpathPre;
	system( cmd1.str().c_str() );

	std::stringstream cmd2(""); //Used to submit commands to the shell
	cmd2.str("");
	cmd2 << "mkdir "<< char_outpath;
	system( cmd2.str().c_str() );


	char char_c1[500];
	char char_c1le[500];
	char char_c2[500];
	char char_c2le[500];
	char char_ctime[500];
	char char_ctimeFast[500];
	char char_cxz1[500];
	char char_cxz2[500];
	char char_cxz3[500];
	char char_cr1[500];

	sprintf(char_c1, 		"%s/%s_c1_%s.%s", 			char_outpath, FileName, cutname, ext_plots);
	sprintf(char_c1le, 		"%s/%s_c1le_%s.%s", 		char_outpath, FileName, cutname, ext_plots);
	sprintf(char_c2, 		"%s/%s_c2_%s.%s", 			char_outpath, FileName, cutname, ext_plots);
	sprintf(char_c2le, 		"%s/%s_c2le_%s.%s", 		char_outpath, FileName, cutname, ext_plots);
	sprintf(char_ctime, 	"%s/%s_ctime_%s.%s", 		char_outpath, FileName, cutname, ext_plots);
	sprintf(char_ctimeFast, "%s/%s_ctimeFast_%s.%s", 	char_outpath, FileName, cutname, ext_plots);
	sprintf(char_cxz1, 		"%s/%s_cxz1_%s.%s", 		char_outpath, FileName, cutname, ext_plots);
	sprintf(char_cxz2, 		"%s/%s_cxz2_%s.%s", 		char_outpath, FileName, cutname, ext_plots);
	sprintf(char_cxz3, 		"%s/%s_cxz3_%s.%s", 		char_outpath, FileName, cutname, ext_plots);
	sprintf(char_cr1, 		"%s/%s_cr1_%s.%s", 			char_outpath, FileName, cutname, ext_plots);

	c1			->SaveAs(char_c1);
	c1le		->SaveAs(char_c1le);
	c2			->SaveAs(char_c2);
	c2le		->SaveAs(char_c2le);
	ctime		->SaveAs(char_ctime);
	ctimeFast	->SaveAs(char_ctimeFast);
	cxz1		->SaveAs(char_cxz1);
	cxz2		->SaveAs(char_cxz2);
	cxz3		->SaveAs(char_cxz3);
	cr1			->SaveAs(char_cr1);

}



