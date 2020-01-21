/////////////////////////////////////////////////////////
//													   //
//   ++++++++++++++++++++++++++++++++++++++++++++++++  //
//   + Alexander Kish for XENON-XUERICH experiment	+  //
//   + UZH, 2008									+  //
// 	 ++++++++++++++++++++++++++++++++++++++++++++++++  //
//													   //
/////////////////////////////////////////////////////////
#ifndef NeutronLabConstruction_h
#define NeutronLabConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "NeutronLabConstruction.hh"


class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VisAttributes;
class G4Material;
class G4MaterialPropertiesTable;
class G4SubtractionSolid;
class G4UnionSolid;




class NeutronLabConstruction : public G4VUserDetectorConstruction
{

  public:
    NeutronLabConstruction();
   ~NeutronLabConstruction();

  public:
    G4VPhysicalVolume *Construct();
	   
  private:

	G4Material *	Air;
	G4Material *	Vacuum;
	G4Material *	Lead_mat;
	G4Material *	Concrete;
	G4Material *	Paraffin;
	G4Material *	BPE;
	G4Material *	Steel;
	
//----------	Logical Volumes		----------------
	G4LogicalVolume *	Laboratory_log;	
	G4LogicalVolume *	vault_log;	
	G4LogicalVolume *	pillar_log;	
/*	G4LogicalVolume *	ConcreteShield_log;
	G4LogicalVolume *	BPEshield_log;
	G4LogicalVolume *	ShieldCavity_log;
	G4LogicalVolume *	ParaffinContainer_log;
	G4LogicalVolume *	Platform_log;

	G4LogicalVolume *	WallLeft_log;
	G4LogicalVolume *	WallFront_log;
	G4LogicalVolume *	WallRight_log;
	G4LogicalVolume *	WallBack_log;

	G4LogicalVolume *	GeneratorContainer_log;
	G4LogicalVolume *	GeneratorCavity_log;
*/
	
//----------	Physical Volumes	----------------
	G4VPhysicalVolume *	Laboratory_phys;
	G4VPhysicalVolume *	vault_phys;
	G4VPhysicalVolume *	pillar1_phys;	
/*	G4VPhysicalVolume * ConcreteShield_phys;
	G4VPhysicalVolume * BPEshield_phys;
	G4VPhysicalVolume * ShieldCavity_phys;
	G4VPhysicalVolume * ParaffinContainer_phys;
	G4VPhysicalVolume * Platform_phys;

	G4VPhysicalVolume * WallLeft_phys;
	G4VPhysicalVolume * WallFront_phys;
	G4VPhysicalVolume * WallRight_phys;
	G4VPhysicalVolume * WallBack_phys;

	G4VPhysicalVolume * GeneratorContainer_phys;
	G4VPhysicalVolume * GeneratorCavity_phys;
*/

//----------	Visual Attributes	----------------
	G4VisAttributes *	Concrete_vis;
	G4VisAttributes *	BPE_vis;
	G4VisAttributes *	Cavity_vis;
	G4VisAttributes *	Paraffin_vis;
	G4VisAttributes *	Steel_vis;

	//G4VisAttributes *	GeneratorContainer_vis;
	//G4VisAttributes *	GeneratorCavity_vis;

	
//----------	Sensitive Detectors	----------------
	//XeScintSD *LXeSD;
	

	//NeutronLabDetectorMessenger *m_pDetectorMessenger;

};

#endif

