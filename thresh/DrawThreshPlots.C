void DrawThreshPlots(const char * fname="thresh.root")
{
  TFile *infile = new TFile(fname,"READ");
  TTree *t = (TTree*)infile->Get("th");
  gStyle->SetOptStat(0);
  Float_t imax = t->GetMaximum("index");

  TGraph *JP0 = new TGraph();
  TGraph *JP1 = new TGraph();
  TGraph *JP2 = new TGraph();
  TGraph *LBitSel = new TGraph();
  TGraph *LBS1 = new TGraph();
  TGraph *LBS2 = new TGraph();
  TGraph *LBS3 = new TGraph();
  TGraph *SBitSel = new TGraph();
  TGraph *SBS1 = new TGraph();
  TGraph *SBS2 = new TGraph();
  TGraph *SBS3 = new TGraph();
  
  JP0->SetTitle("FMS-JP0");
  JP1->SetTitle("FMS-JP1");
  JP2->SetTitle("FMS-JP2");
  LBitSel->SetTitle("FMSlarge-BitSelect");
  LBS1->SetTitle("FMSlarge-BS-th1");
  LBS2->SetTitle("FMSlarge-BS-th2");
  LBS3->SetTitle("FMSlarge-BS-th3");
  SBitSel->SetTitle("FMSsmall-BitSelect");
  SBS1->SetTitle("FMSsmall-BS-th1");
  SBS2->SetTitle("FMSsmall-BS-th2");
  SBS3->SetTitle("FMSsmall-BS-th3");


  JP0->SetLineColor(kRed);
  JP1->SetLineColor(kRed);
  JP2->SetLineColor(kRed);
  LBitSel->SetLineColor(kRed);
  LBS1->SetLineColor(kRed);
  LBS2->SetLineColor(kRed);
  LBS3->SetLineColor(kRed);
  SBitSel->SetLineColor(kRed);
  SBS1->SetLineColor(kRed);
  SBS2->SetLineColor(kRed);
  SBS3->SetLineColor(kRed);

  Float_t findex;
  Float_t fJP0;
  Float_t fJP1;
  Float_t fJP2;
  Float_t fLBitSel;
  Float_t fLBS1;
  Float_t fLBS2;
  Float_t fLBS3;
  Float_t fSBitSel;
  Float_t fSBS1;
  Float_t fSBS2;
  Float_t fSBS3;

  th->SetBranchAddress("index",&findex);
  th->SetBranchAddress("JP0",&fJP0);
  th->SetBranchAddress("JP1",&fJP1);
  th->SetBranchAddress("JP2",&fJP2);
  th->SetBranchAddress("LBitSel",&fLBitSel);
  th->SetBranchAddress("LBS1",&fLBS1);
  th->SetBranchAddress("LBS2",&fLBS2);
  th->SetBranchAddress("LBS3",&fLBS3);
  th->SetBranchAddress("SBitSel",&fSBitSel);
  th->SetBranchAddress("SBS1",&fSBS1);
  th->SetBranchAddress("SBS2",&fSBS2);
  th->SetBranchAddress("SBS3",&fSBS3);

  for(Int_t i=0; i<th->GetEntries(); i++)
  {
    th->GetEntry(i);
    JP0->SetPoint(i,findex,fJP0);
    JP1->SetPoint(i,findex,fJP1);
    JP2->SetPoint(i,findex,fJP2);
    LBitSel->SetPoint(i,findex,fLBitSel);
    LBS1->SetPoint(i,findex,fLBS1);
    LBS2->SetPoint(i,findex,fLBS2);
    LBS3->SetPoint(i,findex,fLBS3);
    SBitSel->SetPoint(i,findex,fSBitSel);
    SBS1->SetPoint(i,findex,fSBS1);
    SBS2->SetPoint(i,findex,fSBS2);
    SBS3->SetPoint(i,findex,fSBS3);
  };
    
  TCanvas *cc = new TCanvas("cc","cc",700,500);
  cc->SetGrid();
  JP0->Draw("AL"); cc->Print("th_plots.pdf(","pdf");
  JP1->Draw("AL"); cc->Print("th_plots.pdf","pdf");
  JP2->Draw("AL"); cc->Print("th_plots.pdf","pdf");
  LBitSel->Draw("AL"); cc->Print("th_plots.pdf","pdf");
  LBS1->Draw("AL"); cc->Print("th_plots.pdf","pdf");
  LBS2->Draw("AL"); cc->Print("th_plots.pdf","pdf");
  LBS3->Draw("AL"); cc->Print("th_plots.pdf","pdf");
  SBitSel->Draw("AL"); cc->Print("th_plots.pdf","pdf");
  SBS1->Draw("AL"); cc->Print("th_plots.pdf","pdf");
  SBS2->Draw("AL"); cc->Print("th_plots.pdf","pdf");
  SBS3->Draw("AL"); cc->Print("th_plots.pdf)","pdf");
};
