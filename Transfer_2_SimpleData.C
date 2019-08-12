void Transfer_2_SimpleData()
{
  bool Is_EmbedEvt = 0; //if this event is a embedding events, this opt can be set to 1.
  char NameTem[200];
  
  int RunNo = 2272;
  string EvtTag = "BDCShiftCorrection_WholeRun";
  
  sprintf(NameTem,"f1_Run%d_SimpleStructure_%s.root",RunNo,EvtTag.c_str());
  TFile* f1_SimpleData = new TFile(NameTem,"recreate");
  TTree* t1_SimpleData = new TTree("t1_SimpleData","Simple Data");
  
  int EvtIndex = 0; 
  
  double P_Embedded[3] = {0};
  
  int Is_TPCVertex = 0;
  double TPC_Vertex_Pos[3] = {0,0,0};
  int RecoTrackNum = 0;
  int Reco_IsEmbedded[200] = {0};
  int RecoTrackCharge[200] = {0};
  double Reco_dEdX_Truncated[200] = {0};
  double Reco_Px[200] = {0}; double Reco_Py[200] = {0}; double Reco_Pz[200] = {0};
  double Reco_DistVertex[200] = {0};
  int Reco_ClusterNum[200] = {0};
  
  int Is_VaVertex = 0;
  double Va_Vertex_Pos[3] = {0,0,0};
  int VaTrackNum = 0;
  int Va_IsEmbedded[200] = {0};
  int VaTrackCharge[200] = {0};
  double Va_dEdX_Truncated[200] = {0};
  double Va_Px[200] = {0}; double Va_Py[200] = {0}; double Va_Pz[200] = {0};
  double Va_DistVertex[200] = {0};
  int Va_ClusterNum[200];
  
  //the below is the simple data structure.
  t1_SimpleData->Branch("EvtIndex",&EvtIndex,"EvtIndex/I");
  t1_SimpleData->Branch("P_Embedded",P_Embedded,"P_Embedded[3]/D");
  
  t1_SimpleData->Branch("Is_TPCVertex",&Is_TPCVertex,"Is_TPCVertex/I");
  t1_SimpleData->Branch("TPC_Vertex_Pos",TPC_Vertex_Pos,"TPC_Vertex_Pos[3]/I");
  t1_SimpleData->Branch("RecoTrackNum",&RecoTrackNum,"RecoTrackNum/I");
  t1_SimpleData->Branch("Reco_IsEmbedded",Reco_IsEmbedded,"Reco_IsEmbedded[RecoTrackNum]/I");
  t1_SimpleData->Branch("RecoTrackCharge",RecoTrackCharge,"RecoTrackCharge[RecoTrackNum]/I");
  t1_SimpleData->Branch("Reco_dEdX_Truncated",Reco_dEdX_Truncated,"Reco_dEdX_Truncated[RecoTrackNum]/D");
  t1_SimpleData->Branch("Reco_Px",Reco_Px,"Reco_Px[RecoTrackNum]/D");
  t1_SimpleData->Branch("Reco_Py",Reco_Py,"Reco_Py[RecoTrackNum]/D");
  t1_SimpleData->Branch("Reco_Pz",Reco_Pz,"Reco_Pz[RecoTrackNum]/D");
  t1_SimpleData->Branch("Reco_DistVertex",Reco_DistVertex,"Reco_DistVertex[RecoTrackNum]/D");
  t1_SimpleData->Branch("Reco_ClusterNum",Reco_ClusterNum,"Reco_ClusterNum[RecoTrackNum]/I");
  
  
  t1_SimpleData->Branch("Is_VaVertex",&Is_VaVertex,"Is_VaVertex/I");
  t1_SimpleData->Branch("Va_Vertex_Pos",Va_Vertex_Pos,"Va_Vertex_Pos[3]/I");
  t1_SimpleData->Branch("VaTrackNum",&VaTrackNum,"VaTrackNum/I");
  t1_SimpleData->Branch("Va_IsEmbedded",Va_IsEmbedded,"Va_IsEmbedded[VaTrackNum]/I");
  t1_SimpleData->Branch("VaTrackCharge",VaTrackCharge,"VaTrackCharge[VaTrackNum]/I");
  t1_SimpleData->Branch("Va_dEdX_Truncated",Va_dEdX_Truncated,"Va_dEdX_Truncated[VaTrackNum]/D");
  t1_SimpleData->Branch("Va_Px",Va_Px,"Va_Px[VaTrackNum]/D");
  t1_SimpleData->Branch("Va_Py",Va_Py,"Va_Py[VaTrackNum]/D");
  t1_SimpleData->Branch("Va_Pz",Va_Pz,"Va_Pz[VaTrackNum]/D");
  t1_SimpleData->Branch("Va_DistVertex",Va_DistVertex,"Va_DistVertex[VaTrackNum]/D");
  t1_SimpleData->Branch("Va_ClusterNum",Va_ClusterNum,"Va_ClusterNum[VaTrackNum]/I");
  
  //the below is for loading the original data.
  auto tree = new TChain("cbmsim");
  
  for(int i=0;i<56;i++)
  {
    sprintf(NameTem,"/mnt/spirit/analysis/changj/SpiRITROOT.Aug2019/macros/data/Sn108/run%d_s%d.reco.develop.1854.c3e534f.root",RunNo,i);
    if(i<10) { sprintf(NameTem,"/mnt/spirit/analysis/changj/SpiRITROOT.Aug2019/macros/data/Sn108/run%d_s0%d.reco.develop.1854.c3e534f.root",RunNo,i); }
    tree->AddFile(NameTem);
  }
  
  TClonesArray *RecoTrackArray = nullptr;
  TClonesArray *VATrackArray   = nullptr;
  TClonesArray *TPCVertexArray = nullptr;
  TClonesArray *BDCVertexArray = nullptr;
  TClonesArray *VaVertexArray  = nullptr;
  TClonesArray *EmbedTrackArray  = nullptr;
  
  tree -> SetBranchAddress("STRecoTrack", &RecoTrackArray);
  tree -> SetBranchAddress("VATracks", &VATrackArray);
  tree -> SetBranchAddress("STVertex", &TPCVertexArray);
  tree -> SetBranchAddress("BDCVertex", &BDCVertexArray);
  tree -> SetBranchAddress("VAVertex", &VaVertexArray);
  if(Is_EmbedEvt==1) { tree -> SetBranchAddress("STEmbedTrack", &EmbedTrackArray); }
  
  Int_t nEvents = tree -> GetEntries();
  cout<<"EvtNum in the tree: "<<nEvents<<endl;
  
  for(int iEvt=0;iEvt<nEvents;iEvt++)
  {
    tree->GetEvent(iEvt);
    if(iEvt%1000==0) { cout<<"iEvt: "<<iEvt<<endl; }
    
    int TPCVertex_Num = TPCVertexArray->GetEntries();
    if(TPCVertex_Num==0) { continue; }
    
    int VaVertex_Num = VaVertexArray->GetEntries();
    if(VaVertex_Num==0) { continue; }
    
    STVertex* aTPCVertex = (STVertex*) TPCVertexArray->At(0);
    TVector3 Pos_TPCVertex = aTPCVertex->GetPos();
    if( !(Pos_TPCVertex.Z()> -13.2 -1.7*3 && Pos_TPCVertex.Z()< -13.2 +1.7*3) ) { continue; }
    STVertex* aVaVertex = (STVertex*) VaVertexArray->At(0);
    TVector3 Pos_VaVertex = aVaVertex->GetPos();
    if(Is_EmbedEvt==1)
    {
      int EmbedTrackNum = EmbedTrackArray->GetEntries();
      if(EmbedTrackNum==1) 
      {
        STEmbedTrack* aEmbedTrack = (STEmbedTrack*) EmbedTrackArray->At(0);
        TVector3 iMomentum = aEmbedTrack->GetInitialMom();
        P_Embedded[0] = iMomentum.X();
        P_Embedded[1] = iMomentum.Y();
        P_Embedded[2] = iMomentum.Z();
      }
    }

    EvtIndex = iEvt;
    Is_TPCVertex = 1;
    Is_VaVertex = 1;
    RecoTrackNum = 0;
    VaTrackNum = 0;
    TPC_Vertex_Pos[0] = Pos_TPCVertex.X(); 
    TPC_Vertex_Pos[1] = Pos_TPCVertex.Y(); 
    TPC_Vertex_Pos[2] = Pos_TPCVertex.Z();
    Va_Vertex_Pos[0] = Pos_VaVertex.X(); 
    Va_Vertex_Pos[1] = Pos_VaVertex.Y(); 
    Va_Vertex_Pos[2] = Pos_VaVertex.Z();
    
    RecoTrackNum = 0;
    VaTrackNum = 0;
    
    int RecoTrackNum_tem = RecoTrackArray->GetEntries();
    int VaTrackNum_tem = VATrackArray->GetEntries();
    
    for(int iRecoTrack=0;iRecoTrack<RecoTrackNum_tem;iRecoTrack++)
    {
      STRecoTrack* aRecoTrack = (STRecoTrack*) RecoTrackArray -> At(iRecoTrack);
      
      TVector3 RecoMom = aRecoTrack->GetMomentumTargetPlane();
      //if(RecoMom.Mag()==0) { continue; }
      double Charge = aRecoTrack->GetCharge();
      double dedx = aRecoTrack->GetdEdxWithCut(0.1,0.7);
      TVector3 POCA_Vertex = aRecoTrack->GetPOCAVertex();
      double distToVertex = (POCA_Vertex-Pos_TPCVertex).Mag();
      int RowClusterNum = aRecoTrack->GetNumRowClusters();
      int LayerClusterNum = aRecoTrack->GetNumLayerClusters();
      int EmbedClusterNum = aRecoTrack->GetNumEmbedClusters();
      double EmbedClusterFraction = 1.0*EmbedClusterNum/(RowClusterNum+LayerClusterNum);
      
      Reco_dEdX_Truncated[RecoTrackNum] = dedx;
      RecoTrackCharge[RecoTrackNum] = Charge;
      Reco_Px[RecoTrackNum] = RecoMom.X();
      Reco_Py[RecoTrackNum] = RecoMom.Y();
      Reco_Pz[RecoTrackNum] = RecoMom.Z();
      Reco_DistVertex[RecoTrackNum] = distToVertex;
      Reco_ClusterNum[RecoTrackNum] = LayerClusterNum+RowClusterNum;
      if(aRecoTrack->IsEmbed()==1 && EmbedClusterNum>5 && EmbedClusterFraction>0.5)
      { 
        Reco_IsEmbedded[RecoTrackNum] = 1;
        
      }
      else
      { Reco_IsEmbedded[RecoTrackNum] = 0; }
      RecoTrackNum++;
    }
    
    for(int iVaTrack=0;iVaTrack<VaTrackNum_tem;iVaTrack++)
    {
      STRecoTrack* aVaTrack = (STRecoTrack*) VATrackArray -> At(iVaTrack);
      
      TVector3 VaMom = aVaTrack->GetMomentumTargetPlane();
      //if(VaMom.Mag()==0) { continue; }
      double Charge = aVaTrack->GetCharge();
      double dedx = aVaTrack->GetdEdxWithCut(0.1,0.7);
      TVector3 POCA_Vertex = aVaTrack->GetPOCAVertex();
      double distToVertex = (POCA_Vertex-Pos_VaVertex).Mag();
      int RowClusterNum = aVaTrack->GetNumRowClusters();
      int LayerClusterNum = aVaTrack->GetNumLayerClusters();
      int EmbedClusterNum = aVaTrack->GetNumEmbedClusters();
      double EmbedClusterFraction = 1.0*EmbedClusterNum/(RowClusterNum+LayerClusterNum);
      
      Va_dEdX_Truncated[VaTrackNum] = dedx;
      VaTrackCharge[VaTrackNum] = Charge;
      Va_Px[VaTrackNum] = VaMom.X();
      Va_Py[VaTrackNum] = VaMom.Y();
      Va_Pz[VaTrackNum] = VaMom.Z();
      Va_DistVertex[VaTrackNum] = distToVertex;
      Va_ClusterNum[VaTrackNum] = LayerClusterNum+RowClusterNum;
      if(aVaTrack->IsEmbed()==1 && EmbedClusterNum>5 && EmbedClusterFraction>0.5)
      { Va_IsEmbedded[VaTrackNum] = 1; }
      else
      { Va_IsEmbedded[VaTrackNum] = 0; }
      
      VaTrackNum++;
    }
    
    t1_SimpleData->Fill();
  }
  t1_SimpleData->Write();
  f1_SimpleData->Close();
}
