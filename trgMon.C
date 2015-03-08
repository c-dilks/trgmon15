// plots trigger rates vs. fill
// use trg to control trg=fmsOnly setup
//  - 2 to use all trg setups
//  - 1 pp200_production_fms_2012
//  - 0 pp200_production_2012
// use rCutType to cut by run number
//  - <0 for runs before and equal to rCut
//  - =0 for all runs (ignore rCut)
//  - >0 for runs after rCut

void trgMon(Int_t beam_en=200, Int_t trg=2, Int_t rCutType=0,Int_t rCut=0)
{
  gStyle->SetOptStat(0);



  /* trigger list */
  ///////////////////////////////////////////////////
  const Int_t N_TRIG = 13;
  enum trigger_enum {kJP0,kJP1,kJP2,
                     kLgBS1,kLgBS2,kLgBS3,
                     kSmBS1,kSmBS2,kSmBS3,
                     kDiBS,kDiJP,kRP,kLED};
  trigger_enum trigger;

  char trigger_str[N_TRIG][16];
  char trigger_ps_str[N_TRIG][16];
  strcpy(trigger_str[kJP0],"JP0");
  strcpy(trigger_str[kJP1],"JP1");
  strcpy(trigger_str[kJP2],"JP2");
  strcpy(trigger_str[kLgBS1],"LgBS1");
  strcpy(trigger_str[kLgBS2],"LgBS2");
  strcpy(trigger_str[kLgBS3],"LgBS3");
  strcpy(trigger_str[kSmBS1],"SmBS1");
  strcpy(trigger_str[kSmBS2],"SmBS2");
  strcpy(trigger_str[kSmBS3],"SmBS3");
  strcpy(trigger_str[kDiBS],"DiBS");
  strcpy(trigger_str[kDiJP],"DiJP");
  strcpy(trigger_str[kRP],"RP");
  strcpy(trigger_str[kLED],"LED");
  for(Int_t i=0; i<N_TRIG; i++) sprintf(trigger_ps_str[i],"%sps",trigger_str[i]);

  Color_t trigger_col[N_TRIG];
  trigger_col[kJP0] = kMagenta+2;
  trigger_col[kJP1] = kMagenta+2;
  trigger_col[kJP2] = kMagenta+2;
  trigger_col[kLgBS1] = kOrange;
  trigger_col[kLgBS2] = kOrange;
  trigger_col[kLgBS3] = kOrange;
  trigger_col[kSmBS1] = kGreen+2;
  trigger_col[kSmBS2] = kGreen+2;
  trigger_col[kSmBS3] = kGreen+2;
  trigger_col[kDiBS] = kBlue;
  trigger_col[kDiJP] = kBlue;
  trigger_col[kRP] = kCyan+2;
  trigger_col[kLED] = kRed;

  ///////////////////////////////////////////////////
  


  /* exclusion list */
  ///////////////////////////////////////////////////
  const Int_t N_EXCLUDE = 2;
  Int_t exclusion_list[N_EXCLUDE] = {2,204};
  /*
   * 2 -- high in all triggers
   * 204 -- high in all triggers
  */
  ///////////////////////////////////////////////////
  
  
  
  /* define time and outlier cut */
  ///////////////////////////////////////////////////
  Int_t time_cut=180; // [seconds]
  ///////////////////////////////////////////////////






  /* read trigger table */
  TFile * fout = new TFile("runlog.root","RECREATE");
  char trigger_tree_str[2048];
  strcpy(trigger_tree_str,"index/I:runnum/I:day/I:run/I:type/C:fill/F:energy/F:time/I");
  for(Int_t i=0; i<N_TRIG; i++) 
    sprintf(trigger_tree_str,"%s:%s/D:%s/D",trigger_tree_str,trigger_str[i],trigger_ps_str[i]);
  sprintf(trigger_tree_str,"%s:MB/D:MBps/D",trigger_tree_str);
  printf("%s\n",trigger_tree_str);
  TTree * tr = new TTree("tr","tr");
  tr->ReadFile("trigger.dat",trigger_tree_str);
  tr->Print();


  /* obtain run index numbers */
  Int_t nRuns = tr->GetEntries();
  Int_t index;
  tr->SetBranchAddress("index",&index);
  tr->GetEntry(0);
  Int_t iterl = 50;
  tr->GetEntry(nRuns-1);
  Int_t iterh = index;
  Int_t niter = iterh-iterl;
  printf("plotting run index %d-%d, %d total\n",iterl,iterh,niter);
  const Int_t N_RUNS = nRuns;

  /* initialize fail code
   * bits 1-13 -- trigger outside range
   * bit 14 -- run too short
   * bit 15 -- mb = 0 or number of events < 50 in any trigger (except LED)
   * bit 16 -- on exclusion list
   */
  Long_t failcode[N_RUNS];
  for(Int_t r=0; r<N_RUNS; r++) failcode[r]=0;

  /* initialise bad run array */
  Bool_t badrun[N_RUNS];
  for(Int_t r=0; r<N_RUNS; r++)
  {
    if(r<iterl || r>iterh) 
    {
      badrun[r]=true;
      failcode[r] = failcode[r] | 0x8000;
    }
    else badrun[r]=false;
  };


  /* epoch lines */
  ///////////////////////////////////////////////////
  const Int_t N_EPOCH=3;
  Int_t epoch[N_EPOCH+1][N_TRIG];
  for(Int_t i=0; i<N_TRIG; i++)
  {
    epoch[0][i] = iterl;
    epoch[1][i] = 190;
    epoch[2][i] = 255;
    epoch[3][i] = iterh;
  };
  ///////////////////////////////////////////////////




  /* run QA lines */
  ///////////////////////////////////////////////////
  Double_t qa_cut_high[N_TRIG];
  Double_t qa_cut_low[N_TRIG];
  ////
  qa_cut_high[kJP0] = 9;
  qa_cut_high[kJP1] = 9;
  qa_cut_high[kJP2] = 9;
  qa_cut_high[kLgBS1] = 9;
  qa_cut_high[kLgBS2] = 9;
  qa_cut_high[kLgBS3] = 9;
  qa_cut_high[kSmBS1] = 9;
  qa_cut_high[kSmBS2] = 9;
  qa_cut_high[kSmBS3] = 9;
  qa_cut_high[kDiBS] = 9;
  qa_cut_high[kDiJP] = 9;
  qa_cut_high[kRP] = 9;
  qa_cut_high[kLED] = 9;
  ////
  qa_cut_low[kJP0] = 0.2;
  qa_cut_low[kJP1] = 0.2;
  qa_cut_low[kJP2] = 0.2;
  qa_cut_low[kLgBS1] = 0.2;
  qa_cut_low[kLgBS2] = 0.2;
  qa_cut_low[kLgBS3] = 0.2;
  qa_cut_low[kSmBS1] = 0.2;
  qa_cut_low[kSmBS2] = 0.2;
  qa_cut_low[kSmBS3] = 0.2;
  qa_cut_low[kDiBS] = 0.2;
  qa_cut_low[kDiJP] = 0.2;
  qa_cut_low[kRP] = 0.2;
  qa_cut_low[kLED] = 0.2;
  ///////////////////////////////////////////////////


  /* print exclusion list */
  char exclusion_list_print[400];
  strcpy(exclusion_list_print,"");
  for(Int_t it=0; it<N_EXCLUDE; it++)
  {
    if(it+1 == N_EXCLUDE) sprintf(exclusion_list_print,"%sindex==%d",exclusion_list_print,exclusion_list[it]);
    else sprintf(exclusion_list_print,"%sindex==%d||",exclusion_list_print,exclusion_list[it]);
  };
  printf("%s\n",exclusion_list_print);
  printf("\nOUTLIER LIST:\n");
  tr->SetScanField(0);
  tr->Scan("index:runnum",exclusion_list_print);
  system("touch outliers.dat; rm outliers.dat"); 
  gSystem->RedirectOutput("outliers.dat");
  tr->Scan("index:runnum",exclusion_list_print);
  gSystem->RedirectOutput(0);
  printf("these runs are manually removed from the plots\n\n");



  /* set trigger tree branch addresses */
  Double_t nev[N_TRIG];
  Double_t nev_ps[N_TRIG];
  Double_t mb,mb_ps;
  Int_t runnum,day,run,time;
  Float_t energy,fill;
  tr->SetBranchAddress("runnum",&runnum);
  tr->SetBranchAddress("day",&day);
  tr->SetBranchAddress("run",&run);
  tr->SetBranchAddress("fill",&fill);
  tr->SetBranchAddress("energy",&energy);
  tr->SetBranchAddress("time",&time);
  tr->SetBranchAddress("MB",&mb);
  tr->SetBranchAddress("MBps",&mb_ps);
  for(Int_t i=0; i<N_TRIG; i++)
  {
    tr->SetBranchAddress(trigger_str[i],&(nev[i]));
    tr->SetBranchAddress(trigger_ps_str[i],&(nev_ps[i]));
  };


  /* initialise plots */
  TGraph * ev_gr[N_EPOCH][N_TRIG]; // (nev*nev_ps) / (mb*mb_ps)
  TGraph * nv_gr[N_EPOCH][N_TRIG]; // (nev*nev_ps) / (mb*mb_ps*epochMean) [normalised ev_gr]
  for(Int_t i=0; i<N_TRIG; i++)
  {
    for(Int_t e=0; e<N_EPOCH; e++)
    {
      ev_gr[e][i] = new TGraph();
      nv_gr[e][i] = new TGraph();
    };
  };


  /* build ev_gr plots */
  Bool_t PLOT;
  Int_t current_epoch[N_TRIG];
  Int_t gr_i[N_EPOCH][N_TRIG];
  for(Int_t e=0; e<N_EPOCH; e++) 
    for(Int_t i=0; i<N_TRIG; i++)
      gr_i[e][i]=0;
  for(Int_t r=0; r<tr->GetEntries(); r++)
  {
    tr->GetEntry(r);
    PLOT=true;

    // check if run duration is greater than time_cut
    if(time < time_cut) 
    {
      PLOT=false;
      failcode[index] = failcode[index] | 0x2000;
    };

    // check if run is in exclusion list
    for(Int_t o=0; o<N_EXCLUDE; o++)
    {
      if(index == exclusion_list[o]) 
      {
        PLOT=false;
        failcode[index] = failcode[index] | 0x8000;
      };
    };

    // make sure we won't divide by zero
    if(mb*mb_ps<1) 
    {
      PLOT=false;
      failcode[index] = failcode[index] | 0x4000;
    };

    // check to see if we have enough events in each
    // trigger, except for LED trigger
    for(Int_t i=0; i<N_TRIG; i++)
    {
      if(i != kLED && nev[i] < 50)
      {
        PLOT=false;
        failcode[index] = failcode[index] | 0x4000;
      };
    };

    if(PLOT)
    {
      // determine which epoch we are in
      for(Int_t i=0; i<N_TRIG; i++)
      {
        current_epoch[i] = -1;
        for(Int_t e=0; e<N_EPOCH; e++)
        {
          if(index>=epoch[e][i] && index<=epoch[e+1][i] && epoch[e][i]!=epoch[e+1][i])
            current_epoch[i] = e;
        };
      };

      // add point to ev_gr
      for(Int_t i=0; i<N_TRIG; i++)
      {
        if(current_epoch[i]>=0)
        {
          ev_gr[current_epoch[i]][i]->SetPoint(gr_i[current_epoch[i]][i],index,(nev[i]*nev_ps[i])/(mb*mb_ps));
          (gr_i[current_epoch[i]][i])++;
        };
      };
    }
    else badrun[index]=true;
  };
  

  /* compute means and build nv_gr plots (normalised ev_gr) */
  Double_t epoch_mean[N_EPOCH][N_TRIG];
  Double_t xx,yy;
  for(Int_t i=0; i<N_TRIG; i++)
  {
    for(Int_t e=0; e<N_EPOCH; e++)
    {
      epoch_mean[e][i] = ev_gr[e][i]->GetMean(2);
      if(ev_gr[e][i]->GetN())
      {
        for(Int_t p=0; p<ev_gr[e][i]->GetN(); p++)
        {
          ev_gr[e][i]->GetPoint(p,xx,yy);
          yy /= epoch_mean[e][i];
          nv_gr[e][i]->SetPoint(p,xx,yy);
          if(yy>qa_cut_high[i] || yy<qa_cut_low[i]) 
          {
            badrun[(Int_t)xx]=true;
            failcode[(Int_t)xx] = failcode[(Int_t)xx] | (Int_t) pow(2,i);
          };
        };
      };
    };
  };

  
  /* build multigraphs */
  TMultiGraph * ev_mgr[N_TRIG];
  TMultiGraph * nv_mgr[N_TRIG];
  for(Int_t i=0; i<N_TRIG; i++)
  {
    ev_mgr[i] = new TMultiGraph();
    nv_mgr[i] = new TMultiGraph();
    for(Int_t e=0; e<N_EPOCH; e++)
    {
      if(ev_gr[e][i]->GetN())
      {
        ev_mgr[i]->Add(ev_gr[e][i]);
        nv_mgr[i]->Add(nv_gr[e][i]);
      };
    };
  };


  /* obtain ev_mgr maxima */
  Double_t ev_mgr_max[N_TRIG];
  for(Int_t i=0; i<N_TRIG; i++)
  {
    ev_mgr_max[i] = 0;
    for(Int_t e=0; e<N_EPOCH; e++)
    {
      ev_mgr_max[i] = (ev_gr[e][i]->GetHistogram()->GetMaximum()>ev_mgr_max[i]) ?
                       ev_gr[e][i]->GetHistogram()->GetMaximum() : ev_mgr_max[i];
    };
    printf("%s max = %f\n",trigger_str[i],ev_mgr_max[i]);
  };
  printf("\n");


  /* multigraph aesthetics */
  char ev_mgr_t[N_TRIG][64];
  char nv_mgr_t[N_TRIG][64];
  for(Int_t i=0; i<N_TRIG; i++)
  {
    sprintf(ev_mgr_t[i],"%s events",trigger_str[i]);
    sprintf(nv_mgr_t[i],"%s epoch-normalised events",trigger_str[i]);
    ev_mgr[i]->SetTitle(ev_mgr_t[i]);
    nv_mgr[i]->SetTitle(nv_mgr_t[i]);
    for(Int_t e=0; e<N_EPOCH; e++)
    {
      ev_gr[e][i]->SetMarkerStyle(kFullCircle);
      nv_gr[e][i]->SetMarkerStyle(kFullCircle);
      ev_gr[e][i]->SetMarkerSize(0.5);
      nv_gr[e][i]->SetMarkerSize(0.5);
      ev_gr[e][i]->SetMarkerColor(trigger_col[i]);
      nv_gr[e][i]->SetMarkerColor(trigger_col[i]);
    };
  };
  for(Int_t i=0; i<N_TRIG; i++)
  {
    ev_mgr[i]->SetMinimum(0);
    ev_mgr[i]->SetMaximum(ev_mgr_max[i]*1.01);
    nv_mgr[i]->SetMinimum(0.1);
    nv_mgr[i]->SetMaximum(10);
    for(Int_t e=0; e<N_EPOCH; e++)
    {
      //nv_gr[e][i]->GetYaxis()->SetLimits(0.1,10);
    };
  };
  


  /* epoch, mean, and QA line aesthetics */
  TLine * epoch_ev_line[N_EPOCH+1][N_TRIG];
  TLine * epoch_nv_line[N_EPOCH+1][N_TRIG];
  TLine * mean_line[N_EPOCH][N_TRIG];
  TLine * qa_line_high[N_TRIG];
  TLine * qa_line_low[N_TRIG];
  TLine * unity_line = new TLine(iterl,1,iterh,1);
  Double_t mean;
  for(Int_t i=0; i<N_TRIG; i++)
  {
    for(Int_t e=0; e<N_EPOCH+1; e++)
    {
      epoch_ev_line[e][i] = new TLine(epoch[e][i],0,epoch[e][i],ev_mgr_max[i]*1.01);
      epoch_nv_line[e][i] = new TLine(epoch[e][i],0.1,epoch[e][i],10);
      epoch_ev_line[e][i]->SetLineColor(kAzure);
      epoch_nv_line[e][i]->SetLineColor(kAzure);
      epoch_ev_line[e][i]->SetLineWidth(2);
      epoch_nv_line[e][i]->SetLineWidth(2);
    };
    for(Int_t e=0; e<N_EPOCH; e++)
    {
      mean = ev_gr[e][i]->GetMean(2);
      printf("%s epoch %d mean = %f\n",trigger_str[i],e,mean);
      mean_line[e][i] = new TLine(epoch[e][i],mean,epoch[e+1][i],mean);
      mean_line[e][i]->SetLineColor(kBlack);
      mean_line[e][i]->SetLineWidth(2);
    };
    qa_line_high[i] = new TLine(iterl,qa_cut_high[i],iterh,qa_cut_high[i]);
    qa_line_low[i] = new TLine(iterl,qa_cut_low[i],iterh,qa_cut_low[i]);
    qa_line_high[i]->SetLineColor(kOrange-6);
    qa_line_low[i]->SetLineColor(kOrange-6);
    qa_line_high[i]->SetLineWidth(2);
    qa_line_low[i]->SetLineWidth(2);
  };
  unity_line->SetLineColor(kBlack);
  unity_line->SetLineWidth(2);


  /*
   * Canvas Layout:
   *
   * 1.JP0     2.LgBS1     3.SmBS1     4.DiBS
   * 5.JP1     6.LgBS2     7.SmBS2     8.DiJP   
   * 9.JP2     10.LgBS3    11.SmBS3    12.RP
   * 13.LED
   *
   */
  Int_t padn[N_TRIG];
  padn[kJP0] = 1;
  padn[kJP1] = 5;
  padn[kJP2] = 9;
  padn[kLgBS1] = 2;
  padn[kLgBS2] = 6;
  padn[kLgBS3] = 10;
  padn[kSmBS1] = 3;
  padn[kSmBS2] = 7;
  padn[kSmBS3] = 11;
  padn[kDiBS] = 4;
  padn[kDiJP] = 8;
  padn[kRP] = 12;
  padn[kLED] = 13;

  // draw canvases
  TCanvas * ev_canv = new TCanvas("ev_canv","ev_canv",1200,250*(N_TRIG/4+1));
  TCanvas * nv_canv = new TCanvas("nv_canv","nv_canv",1200,250*(N_TRIG/4+1));
  ev_canv->Clear();
  nv_canv->Clear();
  ev_canv->Divide(4,(N_TRIG-1)/4+1);
  nv_canv->Divide(4,(N_TRIG-1)/4+1);
  for(Int_t i=0; i<N_TRIG; i++)
  {
    ev_canv->cd(padn[i]);
    ev_mgr[i]->Draw("AP");
    for(Int_t e=0; e<N_EPOCH+1; e++) 
    {
      epoch_ev_line[e][i]->Draw();
      if(e<N_EPOCH) mean_line[e][i]->Draw();
    };

    nv_canv->cd(padn[i]); 
    nv_canv->GetPad(padn[i])->SetLogy();
    nv_mgr[i]->Draw("AP");
    for(Int_t e=0; e<N_EPOCH+1; e++) epoch_nv_line[e][i]->Draw();
    unity_line->Draw();
    qa_line_high[i]->Draw();
    qa_line_low[i]->Draw();
  };


  /* luminosity tracker */
  TGraph * lum_all = new TGraph();
  TGraph * lum_good = new TGraph();
  TGraph * lum_prog_all = new TGraph();
  TGraph * lum_prog_good = new TGraph();
  Int_t lum_all_i=0;
  Int_t lum_good_i=0;
  Double_t intlum_all=0;
  Double_t intlum_good=0;
  for(Int_t i=0; i<tr->GetEntries(); i++)
  {
    tr->GetEntry(i);
    lum_all->SetPoint(lum_all_i,index,mb*mb_ps/(26.1e+9));
    intlum_all += mb*mb_ps/(26.1e+9);
    lum_prog_all->SetPoint(lum_all_i,index,intlum_all);
    lum_all_i++;
    if(badrun[index]==false)
    {
      lum_good->SetPoint(lum_good_i,index,mb*mb_ps/(26.1e+9));
      intlum_good += mb*mb_ps/(26.1e+9);
      lum_prog_good->SetPoint(lum_good_i,index,intlum_good);
      lum_good_i++;
    };
  };
  lum_all->SetTitle("Luminosity -- All Runs");
  lum_good->SetTitle("Luminosity -- Good Runs");
  lum_all->SetMarkerStyle(kFullCircle);
  lum_good->SetMarkerStyle(kFullCircle);
  lum_all->SetMarkerSize(0.5);
  lum_good->SetMarkerSize(0.5);
  lum_all->SetMarkerColor(kBlack);
  lum_good->SetMarkerColor(kRed);
  lum_prog_all->SetLineColor(kBlack);
  lum_prog_good->SetLineColor(kRed);
  lum_prog_all->SetLineWidth(2);
  lum_prog_good->SetLineWidth(2);
  TMultiGraph * lum_prog = new TMultiGraph();
  lum_prog->Add(lum_prog_all);
  lum_prog->Add(lum_prog_good);
  lum_prog->SetTitle("Integrated Luminosity -- (black=all red=good)");


  /* luminosity canvas */
  TCanvas * lum_canv = new TCanvas("lum_canv","lum_canv",1200,400);
  lum_canv->Clear();
  lum_canv->Divide(3,1);
  lum_canv->cd(1); lum_all->Draw("AP");
  lum_canv->cd(2); lum_good->Draw("AP");
  lum_canv->cd(3); lum_prog->Draw("AL");


  /* print data table */
  gROOT->ProcessLine(".! touch goodruns.dat ; rm goodruns.dat");
  gROOT->ProcessLine(".! touch badruns.dat ; rm badruns.dat");
  char printstr[2048];
  for(Int_t i=0; i<tr->GetEntries(); i++)
  {
    tr->GetEntry(i);
    sprintf(printstr,"%d %d %d %d",runnum,index,(Int_t)fill,time);
    for(Int_t j=0; j<N_TRIG; j++) sprintf(printstr,"%s %d %d",printstr,nev[j],nev_ps[j]);
    sprintf(printstr,"%s %d %d %16b",printstr,mb,mb_ps,failcode[index]);
    if(badrun[index]) gSystem->RedirectOutput("badruns.dat");
    else gSystem->RedirectOutput("goodruns.dat");
    printf("%s\n",printstr);
    gSystem->RedirectOutput(0);
  };


  /* ratio plots */
  const Int_t N_RAT_PLOTS = 24;
  Int_t nn[N_RAT_PLOTS]; // numerator plot
  Int_t dd[N_RAT_PLOTS]; // denominator plot
  Color_t ratcol[N_RAT_PLOTS];
  Int_t rpn[N_RAT_PLOTS]; // pad number (4x6 pads in the canvas.. you gotta draw it to understand)
  Int_t ii=0;
  nn[ii] = kLgBS1;  dd[ii] = kSmBS1;  rpn[ii] = 13;  ratcol[ii++] = kRed;    // LgBSn : SmBSn
  nn[ii] = kLgBS2;  dd[ii] = kSmBS2;  rpn[ii] = 17;  ratcol[ii++] = kRed;
  nn[ii] = kLgBS3;  dd[ii] = kSmBS3;  rpn[ii] = 21;  ratcol[ii++] = kRed;
  nn[ii] = kLgBS1;  dd[ii] = kLgBS3;  rpn[ii] = 1;  ratcol[ii++] = kOrange;    // LgBS : LgBStakeall
  nn[ii] = kLgBS2;  dd[ii] = kLgBS3;  rpn[ii] = 2;  ratcol[ii++] = kOrange;
  nn[ii] = kSmBS1;  dd[ii] = kSmBS3;  rpn[ii] = 5;  ratcol[ii++] = kGreen+2;    // SmBS : SmBStakeall
  nn[ii] = kSmBS2;  dd[ii] = kSmBS3;  rpn[ii] = 6;  ratcol[ii++] = kGreen+2;
  nn[ii] = kJP0;    dd[ii] = kJP2;    rpn[ii] = 9;  ratcol[ii++] = kMagenta+2;    // JP : JPtakeall
  nn[ii] = kJP1;    dd[ii] = kJP2;    rpn[ii] = 10; ratcol[ii++] = kMagenta+2;
  nn[ii] = kDiBS;   dd[ii] = kLgBS1;  rpn[ii] = 14;  ratcol[ii++] = kYellow-2;   // diBS : LgBS
  nn[ii] = kDiBS;   dd[ii] = kLgBS2;  rpn[ii] = 18;  ratcol[ii++] = kYellow-2;
  nn[ii] = kDiBS;   dd[ii] = kLgBS3;  rpn[ii] = 22;  ratcol[ii++] = kYellow-2;
  nn[ii] = kDiBS;   dd[ii] = kSmBS1;  rpn[ii] = 15;  ratcol[ii++] = kGreen-5;    // diBS : SmBS
  nn[ii] = kDiBS;   dd[ii] = kSmBS2;  rpn[ii] = 19;  ratcol[ii++] = kGreen-5;
  nn[ii] = kDiBS;   dd[ii] = kSmBS3;  rpn[ii] = 23;  ratcol[ii++] = kGreen-5;
  nn[ii] = kDiJP;   dd[ii] = kJP0;    rpn[ii] = 16;  ratcol[ii++] = kViolet;     // diJP : JP
  nn[ii] = kDiJP;   dd[ii] = kJP1;    rpn[ii] = 20;  ratcol[ii++] = kViolet;
  nn[ii] = kDiJP;   dd[ii] = kJP2;    rpn[ii] = 24;  ratcol[ii++] = kViolet;
  nn[ii] = kRP;     dd[ii] = kLgBS3;  rpn[ii] = 3;  ratcol[ii++] = kAzure-3;    // RP : takealls
  nn[ii] = kRP;     dd[ii] = kSmBS3;  rpn[ii] = 7;  ratcol[ii++] = kAzure-3;
  nn[ii] = kRP;     dd[ii] = kJP2;    rpn[ii] = 11;  ratcol[ii++] = kAzure-3;
  nn[ii] = kLED;    dd[ii] = kLgBS3;  rpn[ii] = 4;  ratcol[ii++] = kPink+9;    // LED : takealls
  nn[ii] = kLED;    dd[ii] = kSmBS3;  rpn[ii] = 8;  ratcol[ii++] = kPink+9;
  nn[ii] = kLED;    dd[ii] = kJP2;    rpn[ii] = 12;  ratcol[ii++] = kPink+9;

  
  /* compute ratios */
  TGraph * rat_gr[N_RAT_PLOTS];
  Int_t rat_gr_i[N_RAT_PLOTS];
  char rat_gr_t[N_RAT_PLOTS][64];
  Double_t yynn[N_RUNS];
  Double_t yydd[N_RUNS];
  for(Int_t i=0; i<N_RAT_PLOTS; i++)
  {
    rat_gr[i] = new TGraph();
    rat_gr_i[i]=0;

    for(Int_t r=0; r<N_RUNS; r++)
    {
      yynn[r] = 0;
      yydd[r] = 0;
    };

    // get numerator points
    for(Int_t e=0; e<N_EPOCH; e++)
    {
      for(Int_t p=0; p<ev_gr[e][nn[i]]->GetN(); p++)
      {
        ev_gr[e][nn[i]]->GetPoint(p,xx,yy);
        yynn[(Int_t)xx] = yy;
      };
      for(Int_t p=0; p<ev_gr[e][dd[i]]->GetN(); p++)
      {
        ev_gr[e][dd[i]]->GetPoint(p,xx,yy);
        yydd[(Int_t)xx] = yy;
      };
    };

    // compute ratios
    for(Int_t r=0; r<N_RUNS; r++)
    {
      if(yynn[r]*yydd[r]>0) 
      {
        rat_gr[i]->SetPoint(rat_gr_i[i],r,yynn[r]/yydd[r]);
        (rat_gr_i[i])++;
      };
    };

    // aesthetics
    sprintf(rat_gr_t[i],"%s / %s trigger rato",trigger_str[nn[i]],trigger_str[dd[i]]);
    rat_gr[i]->SetTitle(rat_gr_t[i]);
    rat_gr[i]->SetMarkerStyle(kFullCircle);
    rat_gr[i]->SetMarkerSize(0.5);
    rat_gr[i]->SetMarkerColor(ratcol[i]);
  };


  /* ratio canvas layout */
  TCanvas * rat_canv = new TCanvas("rat_canv","rat_canv",1200,250*(N_RAT_PLOTS/4+1));
  rat_canv->Clear();
  rat_canv->Divide(4,(N_RAT_PLOTS-1)/4+1);
  for(Int_t i=0; i<N_RAT_PLOTS; i++)
  {
    rat_canv->cd(rpn[i]);
    rat_gr[i]->Draw("AP");
  };


  /* prescale vs. run index plots */
  TGraph * prescale_gr[N_TRIG];
  Int_t prescale_gr_i[N_TRIG];
  char prescale_gr_t[N_TRIG][64];
  for(Int_t i=0; i<N_TRIG; i++)
  {
    prescale_gr[i] = new TGraph();
    prescale_gr_i[i] = 0;
    sprintf(prescale_gr_t[i],"%s prescale factor",trigger_str[i]);
    prescale_gr[i]->SetTitle(prescale_gr_t[i]);
    prescale_gr[i]->SetMarkerStyle(kFullCircle);
    prescale_gr[i]->SetMarkerSize(0.5);
    prescale_gr[i]->SetMarkerColor(trigger_col[i]);
  };
  for(Int_t i=0; i<tr->GetEntries(); i++)
  {
    tr->GetEntry(i);
    if(badrun[index] != 1)
    {
      for(Int_t j=0; j<N_TRIG; j++)
      {
        prescale_gr[j]->SetPoint(prescale_gr_i[j],index,nev_ps[j]);
        (prescale_gr_i[j])++;
      };
    };
  };

  
  /* draw prescale canvas */
  TCanvas * prescale_canv = new TCanvas("prescale_canv","prescale_canv",1200,250*(N_TRIG/4+1));
  prescale_canv->Clear();
  prescale_canv->Divide(4,(N_TRIG-1)/4+1);
  for(Int_t i=0; i<N_TRIG; i++)
  {
    prescale_canv->cd(padn[i]);
    prescale_gr[i]->Draw("AP");
  };
  

  /* write run index table */
  Int_t good_count=0;
  for(Int_t r=0; r<N_RUNS; r++)
  {
    if(badrun[r]==false) good_count++;
  };
  gROOT->ProcessLine(".! touch runindex.txt ; rm runindex.txt");
  gSystem->RedirectOutput("runindex.txt");
  printf("Estimated integrated luminosity for all runs: %f pb<sup>-1</sup>\n",intlum_all);
  printf("Estimated integrated luminosity for good runs: %f pb<sup>-1</sup>\n\n",intlum_good);
  printf("Total number of runs: %d\n",N_RUNS);
  printf("Number of good runs: %d\n",good_count);
  printf("\nRun Index <--> Run Number\n");
  tr->SetScanField(0);
  tr->Scan("index:runnum","");
  gSystem->RedirectOutput(0);
  printf("---> Run Index: runindex.txt\n");


  /* print pngs */
  ev_canv->Print("ev_canv.png","png");
  nv_canv->Print("nv_canv.png","png");
  lum_canv->Print("lum_canv.png","png");
  rat_canv->Print("rat_canv.png","png");
  prescale_canv->Print("prescale_canv.png","png");
  
  /* write rootfile */
  ev_canv->Write();
  nv_canv->Write();
  lum_canv->Write();
  rat_canv->Write();
  prescale_canv->Write();
  tr->Write("tr");
  fout->Close();
};
