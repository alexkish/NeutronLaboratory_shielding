/////////////////////////////////////////////////////////
//													   //
//   ++++++++++++++++++++++++++++++++++++++++++++++++  //
//   + Deuterium Fusion Neutron Generator at UZH       //
//   + Alexander Kish                  UZH, 2008	   //
// 	 ++++++++++++++++++++++++++++++++++++++++++++++++  //
//													   //
/////////////////////////////////////////////////////////

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Isotope.hh"
#include "G4Element.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Orb.hh"
#include "G4Polyhedra.hh"
#include "G4Trd.hh"
#include "G4Cons.hh"

#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"

#include "G4VisAttributes.hh"

#include "G4OpBoundaryProcess.hh"
#include "G4SDManager.hh"

#include "G4Colour.hh"
#include "globals.hh"

#include "G4ios.hh"

#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"

#include "vector"
#include "numeric"
#include "sstream"
#include "algorithm"
#include "cmath"
#include "cassert"

using std::vector;
using std::stringstream;
using std::max;

#include "NeutronLabConstruction.hh"
#include "ScreenSensitiveDetector.hh"


NeutronLabConstruction::NeutronLabConstruction()
{
}

NeutronLabConstruction::~NeutronLabConstruction()
{
}

G4VPhysicalVolume *NeutronLabConstruction::Construct()
{
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
G4cout 																			<< G4endl;
G4cout << 	" ============================================================ "	<< G4endl;
G4cout <<	"|   Neutron laboratory at UHM, Watanabe Hall 102             |"	<< G4endl;
G4cout <<	"|  -----------------------------------------------           |"	<< G4endl;
G4cout <<	"|                            Alexander Kish, UHM/CERN 2019   |"	<< G4endl;
G4cout <<	" ============================================================ "	<< G4endl;
G4cout <<	"| "																<< G4endl;
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////



	G4double	density,	// density
    			a,			// atomic weight (g/mole)
    			z;			// atomic number
	G4String	name,		// name
 				symbol;		// symbol
	G4int		ncomponents,
 				natoms;          
	G4double	temperature,
				pressure;    


	//----------	Define elements		--------------------------------------------------------
	G4Element *H  = new G4Element(name= "Hydrogen",		symbol= "H",  z= 1.,  a= 1.008   *CLHEP::g/CLHEP::mole);
	G4Element *B  = new G4Element(name= "Boron",		symbol= "B",  z= 5.,  a= 10.811  *CLHEP::g/CLHEP::mole);
	G4Element *C  = new G4Element(name= "Carbon",		symbol= "C",  z= 6.,  a= 12.011  *CLHEP::g/CLHEP::mole);
	G4Element *O  = new G4Element(name= "Oxygen",  		symbol= "O",  z= 8.,  a= 16.00   *CLHEP::g/CLHEP::mole);
	G4Element *F  = new G4Element(name= "Fluorine",		symbol= "F",  z= 9.,  a= 18.998  *CLHEP::g/CLHEP::mole);
	G4Element* Mg = new G4Element(name= "Magnesium", 	symbol= "Mg", z= 12., a= 24.305  *CLHEP::g/CLHEP::mole);
	G4Element *Al = new G4Element(name= "Aluminium", 	symbol= "Al", z= 13., a= 26.982  *CLHEP::g/CLHEP::mole);
	G4Element *Si = new G4Element(name= "Silicon",   	symbol= "Si", z= 14., a= 28.086  *CLHEP::g/CLHEP::mole);
	G4Element *P  = new G4Element(name= "Phosphorus",  	symbol= "P",  z= 15., a= 30.9738 *CLHEP::g/CLHEP::mole);
	G4Element *S  = new G4Element(name= "Sulphur",   	symbol= "S",  z= 16., a= 32.065  *CLHEP::g/CLHEP::mole);
	G4Element *K  = new G4Element(name= "Potassium", 	symbol= "K",  z= 19., a= 39.0983 *CLHEP::g/CLHEP::mole);
	G4Element* Ca = new G4Element(name= "Calcium",   	symbol= "Ca", z= 20., a= 40.078  *CLHEP::g/CLHEP::mole);
	G4Element *Ti = new G4Element(name= "Titanium",  	symbol= "Ti", z= 22., a= 47.867  *CLHEP::g/CLHEP::mole);
	G4Element *Cr = new G4Element(name= "Chromium",  	symbol= "Cr", z= 24., a= 51.996  *CLHEP::g/CLHEP::mole);
	G4Element *Mn = new G4Element(name= "Manganese", 	symbol= "Mn", z= 25., a= 54.938  *CLHEP::g/CLHEP::mole);
	G4Element *Fe = new G4Element(name= "Iron",			symbol= "Fe", z= 26., a= 55.845  *CLHEP::g/CLHEP::mole);
	G4Element *Co = new G4Element(name= "Cobalt",		symbol= "Co", z= 27., a= 58.9332 *CLHEP::g/CLHEP::mole);
	G4Element *Ni = new G4Element(name= "Nickel",    	symbol= "Ni", z= 28., a= 58.693  *CLHEP::g/CLHEP::mole);
	G4Element *Mo = new G4Element(name= "Molybdenum",	symbol= "Mo", z= 42., a= 95.94   *CLHEP::g/CLHEP::mole);
	G4Element *Pb = new G4Element(name= "Lead",			symbol= "Pb", z= 82., a= 207.2   *CLHEP::g/CLHEP::mole);
	G4Element* Li = G4NistManager::Instance()->FindOrBuildElement("Li");
	
	//----------	 Define Materials 	--------------------------------------------------------

    // Air
	G4Material *Air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

    // Vacuum
	G4Material *Vacuum = new G4Material(name = "Vacuum", z = 1., a = 1. *CLHEP::g/CLHEP::mole, density = 1.e-20 *CLHEP::g/CLHEP::cm3, kStateGas, temperature = 0.1 * CLHEP::kelvin, pressure = 1.e-20 * CLHEP::bar);

    // Lead
    G4Material *Lead_mat = new G4Material(name = "Lead_mat", density = 11.34 *CLHEP::g/CLHEP::cm3, ncomponents =1);
    Lead_mat-> AddElement(Pb, 1); 

	// Concrete
	G4Material *Concrete = G4NistManager::Instance()->FindOrBuildMaterial("G4_CONCRETE");

	// Paraffin
	G4Material *Paraffin = G4NistManager::Instance()->FindOrBuildMaterial("G4_PARAFFIN");

    // Steel
	G4Material *Steel = new G4Material(name = "Steel", density = 7.7 *CLHEP::g/CLHEP::cm3, ncomponents = 3);
	Steel->AddElement(C,  0.04);
	Steel->AddElement(Fe, 0.88);
	Steel->AddElement(Co, 0.08);

	//------------------------------- stainless steel -------------------------------
	G4Material *Steel304 = new G4Material("Steel304", density = 8.00 *CLHEP::g/CLHEP::cm3, ncomponents =10);
	Steel304->AddElement(C,  0.0008);
	Steel304->AddElement(Si, 0.01);
	Steel304->AddElement(Mn, 0.02);
	Steel304->AddElement(P,  0.00045);
	Steel304->AddElement(S,  0.0003);
	Steel304->AddElement(Ni, 0.12);
	Steel304->AddElement(Cr, 0.17);
	Steel304->AddElement(Mo, 0.025);
	Steel304->AddElement(Ti, 0.004);
	Steel304->AddElement(Fe, 0.64945);

	// Polyethylene
	//G4Material *PE = G4NistManager::Instance()->FindOrBuildMaterial("G4_POLYETHYLENE");
    // Borated polyethylene
    // PE 1000 X-Protect, 15% Bortrioxid
	//G4Material *BPE = new G4Material(name = "BPE", density = 1.010 *CLHEP::g/CLHEP::cm3, ncomponents =4);
	//BPE->AddElement(C, 0.30);
	//BPE->AddElement(H, 0.55);
	//BPE->AddElement(B, 0.06);
	//BPE->AddElement(O, 0.09);

	//http://www.shieldwerx.com/poly-neutron.html
	G4Material *boron 	= G4NistManager::Instance()->FindOrBuildMaterial("G4_B"); // pure boron
	G4Material *PE 		= G4NistManager::Instance()->FindOrBuildMaterial("G4_POLYETHYLENE"); // pure polyethylene, e.g. SWX213
	G4Material *bismuth = G4NistManager::Instance()->FindOrBuildMaterial("G4_BISMUTH"); // pure bismuth
	// 5% borated polyethylene SWX201
	G4Material *BPE5 = new G4Material(name="BPE5", density=0.95*CLHEP::g/CLHEP::cm3, ncomponents=2); 
	BPE5->AddMaterial(boron, 	0.05);
	BPE5->AddMaterial(PE, 		0.95);
	// 5% borated high-density polyethylene SWX201HD
	G4Material *BPE5HD = new G4Material(name="BPE5HD", density=1.07*CLHEP::g/CLHEP::cm3, ncomponents=2); 
	BPE5HD->AddMaterial(boron, 	0.05);
	BPE5HD->AddMaterial(PE, 	0.95);
	// 30% borated polyethylene SWX210
	G4Material *BPE30 = new G4Material(name="BPE30", density=1.19*CLHEP::g/CLHEP::cm3, ncomponents=2); 
	BPE30->AddMaterial(boron, 	0.30);
	BPE30->AddMaterial(PE, 		0.70);
	// bismuth-based PE, poly-biz SWX217
/*	G4Material *BiPE = new G4Material(name="BiPE", density=2.92*CLHEP::g/CLHEP::cm3, ncomponents=2); 
	BiPE->AddMaterial(bismuth, 	0.215);
	BiPE->AddMaterial(PE, 		0.785);
*/    // Boric Acid
	G4Material *BoricAcid = new G4Material(name="BoricAcid", density=1.435*CLHEP::g/CLHEP::cm3, ncomponents =3);
	BoricAcid->AddElement(H, natoms=3);
	BoricAcid->AddElement(B, natoms=1);
	BoricAcid->AddElement(O, natoms=3);

	//dirt (LNGS rock for now)
	G4Material* rock = new G4Material("rock", 2.71*CLHEP::g/CLHEP::cm3, 8, kStateSolid);
	rock->AddElement(O,  0.5077);
	rock->AddElement(Ca, 0.2689);
	rock->AddElement(C,  0.1217);
	rock->AddElement(Mg, 0.0832);
	rock->AddElement(Si, 0.0105);
	rock->AddElement(Al, 0.0063);
	rock->AddElement(K,  0.0010);
	rock->AddElement(H,  0.0007);

	//Lithium Polyethylene
	//G4Material* polyethylene = man->FindOrBuildMaterial("G4_POLYETHYLENE");
	G4Material *LiPE = new G4Material("LiPE", 1.06*CLHEP::g/CLHEP::cm3, 2);
	LiPE->AddMaterial(PE, 92.46*CLHEP::perCent);
	LiPE->AddElement(Li, 7.54*CLHEP::perCent);

	// Li-6 thermal neutron absorber
	G4Isotope *Li6 = new G4Isotope("Lithium6", 3, 6, 6.015123*CLHEP::g/CLHEP::mole);
	G4Element *LiPure = new G4Element("LithiumPure", "LiPure", 1);
	LiPure->AddIsotope(Li6,1);
	G4Material *LiMat = new G4Material(name = "LiMat", density=0.534*CLHEP::g/CLHEP::cm3, ncomponents=1);
	LiMat->AddElement(LiPure,1);

	// S-32 neutron filter
	G4Isotope *S32 = new G4Isotope("Sulphur32", 16, 32, 31.972071*CLHEP::g/CLHEP::mole);
	G4Element *SPure = new G4Element("SulphurPure", "SiPure", 1);
	SPure->AddIsotope(S32,1);
	G4Material *SMat = new G4Material(name = "SMat", density=2.0*CLHEP::g/CLHEP::cm3, ncomponents=1);
	SMat->AddElement(SPure,1);

	// Pure silicon for neutron moderator material
 	G4Material *SiMat =  new G4Material(name="SiMat", density=2.33*CLHEP::g/CLHEP::cm3, ncomponents=1);
  	SiMat->AddElement(Si, 1);


	////////////////////////////////////////////////////////////////
	//--------------	colours		--------------------------------
	G4Colour white  (1.0,	1.0,	1.0);
	G4Colour grey   (0.5,	0.5,	0.5);
	G4Colour lgrey  (.85,	.85,	.85);
	G4Colour red    (1.0,	0.0,	0.0);
	G4Colour lred   (0.75,	0.0,	0.0);
	G4Colour xlred  (0.5,	0.0,	0.0);
	G4Colour cyan   (0.0,	1.0,	1.0);
	G4Colour blue   (0.0,	0.0,	1.0);
	G4Colour lblue  (.5,	0.5,	1.,		1.);
	G4Colour xlblue (.5,	0.5,	1.,		0.2);
	G4Colour magenta(1.0,	0.0,	1.0);
	G4Colour yellow (1.0,	1.0,	0.0);
	G4Colour green  (0.,	.1,		0.);
	G4Colour lgreen (0.0,	.75,	0.0);
	G4Colour xlgreen(0.0,	0.5,	0.0);
	G4Colour brown  (0.7,	0.4,	0.1);
	G4Colour orange (1.0,	0.5,	0.0);
	G4Colour xlorange (1.0,	0.5,	0.0, 	0.2);

	//--------------	rotations	--------------------------------
	G4RotationMatrix *RotationXPlus225	= new G4RotationMatrix();
	RotationXPlus225->rotateX(22.5*CLHEP::deg);

	G4RotationMatrix *RotationXMinus225	= new G4RotationMatrix();
	RotationXMinus225->rotateX(-22.5*CLHEP::deg);

	G4RotationMatrix *RotationXPlus45	= new G4RotationMatrix();
	RotationXPlus45->rotateX(45.*CLHEP::deg);

	G4RotationMatrix *RotationXMinus45	= new G4RotationMatrix();
	RotationXMinus45->rotateX(-45.*CLHEP::deg);

	G4RotationMatrix *RotationXPlus90	= new G4RotationMatrix();
	RotationXPlus90->rotateX(90.*CLHEP::deg);
	RotationXPlus90->rotateY(0.*CLHEP::deg);
	RotationXPlus90->rotateZ(0.*CLHEP::deg);

	G4RotationMatrix *RotationYPlus90	= new G4RotationMatrix();
	RotationYPlus90->rotateX(0.*CLHEP::deg);
	RotationYPlus90->rotateY(90.*CLHEP::deg);
	RotationYPlus90->rotateZ(0.*CLHEP::deg);

	G4RotationMatrix *RotationXMinus90	= new G4RotationMatrix();
	RotationXMinus90->rotateX(-90.*CLHEP::deg);

	G4RotationMatrix *RotationX180		= new G4RotationMatrix();
	RotationX180->rotateX(180.*CLHEP::deg);
	
//	G4RotationMatrix *ZeroRot			= new G4RotationMatrix();
//	ZeroRot->rotateX(0. *deg);
//	ZeroRot->rotateY(0. *deg);
//	ZeroRot->rotateZ(0. *deg);
	
	G4RotationMatrix ZeroRot;
	ZeroRot.rotateX(0. *CLHEP::deg);
	ZeroRot.rotateY(0. *CLHEP::deg);
	ZeroRot.rotateZ(0. *CLHEP::deg);


	G4double opendeg	= 0.0 *CLHEP::deg;
	G4double closedeg	= 360.0 *CLHEP::deg;

	////////////////////////////////////////
	// lab hall
	G4double Lab_HalfX = 100.0 *CLHEP::ft;
	G4double Lab_HalfY = 100.0 *CLHEP::ft;
	G4double Lab_HalfZ = 50.0 *CLHEP::ft;

	G4Box *Laboratory = new G4Box("Laboratory", Lab_HalfX, Lab_HalfY, Lab_HalfZ);

	////////////////////////////////////////
	// vault dimensions
	G4double vault_X = 198.0 *CLHEP::in;
	G4double vault_Y = 920.0 *CLHEP::in;
	G4double vault_Z = 91.0 *CLHEP::in;

	G4Box *vault = new G4Box("vault", vault_X/2, vault_Y/2, vault_Z/2);

	////////////////////////////////////////
	// West wall
	G4double wallWestSteel_thick	= 9.0 *CLHEP::in;
	G4double wallWestConcrete_thick	= 3.0 *CLHEP::ft;
	
	G4double wallWestSteel_X 	= wallWestSteel_thick;
	G4double wallWestSteel_Y 	= vault_Y;
	G4double wallWestSteel_Z 	= vault_Z;
	G4double wallWestConcrete_X = wallWestConcrete_thick;
	G4double wallWestConcrete_Y = vault_Y;
	G4double wallWestConcrete_Z = vault_Z;

	G4double wallWestSteel1_posX = -vault_X/2 - wallWestSteel_X/2;
	G4double wallWestSteel1_posY = 0;
	G4double wallWestSteel1_posZ = 0;

	G4double wallWestConcrete1_posX = wallWestSteel1_posX - wallWestSteel_X/2 - wallWestConcrete_X/2;
	G4double wallWestConcrete1_posY = 0;
	G4double wallWestConcrete1_posZ = 0;

	G4double wallWestSteel2_posX = wallWestConcrete1_posX - wallWestConcrete_X/2 - wallWestSteel_X/2;
	G4double wallWestSteel2_posY = 0;
	G4double wallWestSteel2_posZ = 0;

	G4double wallWestConcrete2_posX = wallWestSteel2_posX - wallWestSteel_X/2 - wallWestConcrete_X/2;
	G4double wallWestConcrete2_posY = 0;
	G4double wallWestConcrete2_posZ = 0;

	G4Box *wallWestSteel 	= new G4Box("wallWestSteel", wallWestSteel_X/2, wallWestSteel_Y/2, wallWestSteel_Z/2);
	G4Box *wallWestConcrete = new G4Box("wallWestConcrete", wallWestConcrete_X/2, wallWestConcrete_Y/2, wallWestConcrete_Z/2);

	////////////////////////////////////////
	// East wall
	G4double wallEastConcrete_thick	= 12.0 *CLHEP::in;
	G4double wallEastDirt_thick		= 5.0 *CLHEP::ft;
	
	G4double wallEastConcrete_X = wallEastConcrete_thick;
	G4double wallEastConcrete_Y = vault_Y;
	G4double wallEastConcrete_Z = vault_Z;

	G4double wallEastDirt_X = wallEastDirt_thick;
	G4double wallEastDirt_Y = 3* vault_Y;
	G4double wallEastDirt_Z = 2* wallEastConcrete_Z;

	G4Box *wallEastConcrete = new G4Box("wallEastConcrete", wallEastConcrete_X/2, wallEastConcrete_Y/2, wallEastConcrete_Z/2);
	G4Box *wallEastDirt 	= new G4Box("wallEastDirt", wallEastDirt_X/2, wallEastDirt_Y/2, wallEastDirt_Z/2);

	G4double wallEastConcrete_posX = vault_X/2 + wallEastConcrete_X/2;
	G4double wallEastConcrete_posY = 0;
	G4double wallEastConcrete_posZ = 0;

	G4double wallEastDirt_posX = wallEastConcrete_posX + wallEastConcrete_X/2 + wallEastDirt_X/2;
	G4double wallEastDirt_posY = 0;
	G4double wallEastDirt_posZ = -vault_Z/2 + wallEastDirt_Z/2;

	////////////////////////////////////////
	// North wall
	G4double wallNorthSteel_X 	= vault_X + wallWestConcrete_thick*2 + wallWestSteel_thick*2 + wallEastConcrete_thick;
	G4double wallNorthSteel_Y 	= wallWestSteel_thick;
	G4double wallNorthSteel_Z 	= vault_Z;
	G4double wallNorthConcrete_X = wallNorthSteel_X;
	G4double wallNorthConcrete_Y = wallWestConcrete_thick;
	G4double wallNorthConcrete_Z = vault_Z;

	G4double wallNorthSteel1_posX = wallEastConcrete_posX + wallEastConcrete_thick/2 - wallNorthSteel_X/2;
	G4double wallNorthSteel1_posY = vault_Y/2 + wallNorthSteel_Y/2;
	G4double wallNorthSteel1_posZ = 0;

	G4double wallNorthConcrete1_posX = wallNorthSteel1_posX;
	G4double wallNorthConcrete1_posY = wallNorthSteel1_posY + wallNorthSteel_Y/2 + wallNorthConcrete_Y/2;
	G4double wallNorthConcrete1_posZ = 0;

	G4double wallNorthSteel2_posX = wallNorthSteel1_posX;
	G4double wallNorthSteel2_posY = wallNorthConcrete1_posY + wallNorthConcrete_Y/2 + wallNorthSteel_Y/2;
	G4double wallNorthSteel2_posZ = 0;

	G4double wallNorthConcrete2_posX = wallNorthSteel1_posX;
	G4double wallNorthConcrete2_posY = wallNorthSteel2_posY + wallNorthSteel_Y/2 + wallNorthConcrete_Y/2;
	G4double wallNorthConcrete2_posZ = 0;

	G4Box *wallNorthSteel 	= new G4Box("wallNorthSteel", wallNorthSteel_X/2, wallNorthSteel_Y/2, wallNorthSteel_Z/2);
	G4Box *wallNorthConcrete = new G4Box("wallNorthConcrete", wallNorthConcrete_X/2, wallNorthConcrete_Y/2, wallNorthConcrete_Z/2);

	// borated North wall
	G4double wallNorthBPE_thick = 5 *CLHEP::in;
	G4double wallNorthBPE_X 	= vault_X;
	G4double wallNorthBPE_Y 	= wallNorthBPE_thick;
	G4double wallNorthBPE_Z 	= vault_Z;
	
	G4Box *wallNorthBPE 	= new G4Box("wallNorthBPE", wallNorthBPE_X/2, wallNorthBPE_Y/2, wallNorthBPE_Z/2);

	G4double wallNorthBPE_posX = 0;
	G4double wallNorthBPE_posY = vault_Y/2 - wallNorthBPE_Y/2;
	G4double wallNorthBPE_posZ = 0;

	////////////////////////////////////////
	// South wall
	G4double wallSouthSteel_X 	= vault_X + wallWestConcrete_thick*2 + wallWestSteel_thick*2 + wallEastConcrete_thick;
	G4double wallSouthSteel_Y 	= wallWestSteel_thick;
	G4double wallSouthSteel_Z 	= vault_Z;
	G4double wallSouthConcrete_X = wallSouthSteel_X;
	G4double wallSouthConcrete_Y = wallWestConcrete_thick;
	G4double wallSouthConcrete_Z = vault_Z;

	G4double wallSouthSteel1_posX = wallEastConcrete_posX + wallEastConcrete_thick/2 - wallSouthSteel_X/2;
	G4double wallSouthSteel1_posY = -vault_Y/2 - wallSouthSteel_Y/2;
	G4double wallSouthSteel1_posZ = 0;

	G4double wallSouthConcrete1_posX = wallSouthSteel1_posX;
	G4double wallSouthConcrete1_posY = wallSouthSteel1_posY - wallSouthSteel_Y/2 - wallSouthConcrete_Y/2;
	G4double wallSouthConcrete1_posZ = 0;

	G4double wallSouthSteel2_posX = wallSouthSteel1_posX;
	G4double wallSouthSteel2_posY = wallSouthConcrete1_posY - wallSouthConcrete_Y/2 - wallSouthSteel_Y/2;
	G4double wallSouthSteel2_posZ = 0;

	G4double wallSouthConcrete2_posX = wallSouthSteel1_posX;
	G4double wallSouthConcrete2_posY = wallSouthSteel2_posY - wallSouthSteel_Y/2 - wallSouthConcrete_Y/2;
	G4double wallSouthConcrete2_posZ = 0;

	G4Box *wallSouthSteel 	= new G4Box("wallSouthSteel", wallSouthSteel_X/2, wallSouthSteel_Y/2, wallSouthSteel_Z/2);
	G4Box *wallSouthConcrete = new G4Box("wallSouthConcrete", wallSouthConcrete_X/2, wallSouthConcrete_Y/2, wallSouthConcrete_Z/2);

	// borated South wall
	G4double wallSouthBPE_thick = 5 *CLHEP::in;
	G4double wallSouthBPE_X 	= vault_X;
	G4double wallSouthBPE_Y 	= wallSouthBPE_thick;
	G4double wallSouthBPE_Z 	= vault_Z;
	
	G4Box *wallSouthBPE 	= new G4Box("wallSouthBPE", wallSouthBPE_X/2, wallSouthBPE_Y/2, wallSouthBPE_Z/2);

	G4double wallSouthBPE_posX = 0;
	G4double wallSouthBPE_posY = -vault_Y/2 + wallSouthBPE_Y/2;
	G4double wallSouthBPE_posZ = 0;

	////////////////////////////////////////
	// pillars (x6)
	G4double pillar_X 	    = 10.5 *CLHEP::in;
	G4double pillar_Y 	    = 12.0 *CLHEP::in;
	G4double pillar_Z 	    = 91.0 *CLHEP::in;
	G4double pillar_dist    = 132.0 *CLHEP::in;
	G4double pillar_offsetY = 5.0 *CLHEP::in;
	G4double pillar_offsetX = 41.0 *CLHEP::in;

	G4Box *pillar = new G4Box("pillar", pillar_X/2, pillar_Y/2, pillar_Z/2);

	G4double pillar_posX = vault_X/2 - pillar_offsetX - pillar_X/2;
	G4double pillar1_posY = -vault_Y/2 + pillar_offsetY + pillar_Y/2;
	G4double pillar2_posY = pillar1_posY + pillar_dist + pillar_Y/2;
	G4double pillar3_posY = pillar2_posY + pillar_dist + pillar_Y/2;
	G4double pillar4_posY = pillar3_posY + pillar_dist + pillar_Y/2;
	G4double pillar5_posY = pillar4_posY + pillar_dist + pillar_Y/2;
	G4double pillar6_posY = pillar5_posY + pillar_dist + pillar_Y/2;

	////////////////////////////////////////
	// ceiling
	G4double ceilingSteel_thick 	= 18.0 *CLHEP::in;
	G4double ceilingConcrete_thick 	= 3.0 *CLHEP::ft;
	G4double ceilingPE_thick 		= 4.0 *CLHEP::in;

	G4double ceilingSteel_X 	    = vault_X + wallWestConcrete_X*2 + wallWestSteel_X*2 + wallEastConcrete_X;
	G4double ceilingSteel_Y 	    = vault_Y + wallNorthConcrete_Y*2 + wallNorthSteel_Y*2 + wallSouthConcrete_Y*2 + wallSouthSteel_Y*2;
	G4double ceilingSteel_Z 	    = ceilingSteel_thick;

	G4double ceilingConcrete_X 	    = ceilingSteel_X;
	G4double ceilingConcrete_Y 	    = ceilingSteel_Y;
	G4double ceilingConcrete_Z 	    = ceilingConcrete_thick;

	G4double ceilingPE_X 	    	= ceilingSteel_X;
	G4double ceilingPE_Y 	    	= ceilingSteel_Y;
	G4double ceilingPE_Z 	    	= ceilingPE_thick;

	G4Box *ceilingSteel 			= new G4Box("ceilingSteel", ceilingSteel_X/2, ceilingSteel_Y/2, ceilingSteel_Z/2);
	G4Box *ceilingConcrete 			= new G4Box("ceilingConcrete", ceilingConcrete_X/2, ceilingConcrete_Y/2, ceilingConcrete_Z/2);
	G4Box *ceilingPE 				= new G4Box("ceilingPE", ceilingPE_X/2, ceilingPE_Y/2, ceilingPE_Z/2);

	G4double ceilingSteel_posX 		= vault_X/2 + wallEastConcrete_X - ceilingSteel_X/2;
	G4double ceilingSteel_posY 		= vault_Y/2 + wallNorthSteel_Y*2 + wallNorthConcrete_Y*2 - ceilingSteel_Y/2;
	G4double ceilingSteel_posZ 		= vault_Z/2 + ceilingSteel_Z/2;

	G4double ceilingConcrete_posX 	= ceilingSteel_posX;
	G4double ceilingConcrete_posY 	= ceilingSteel_posY;
	G4double ceilingConcrete_posZ 	= ceilingSteel_posZ + ceilingSteel_Z/2 + ceilingConcrete_Z/2;

	G4double ceilingPE_posX 		= ceilingSteel_posX;
	G4double ceilingPE_posY 		= ceilingSteel_posY;
	G4double ceilingPE_posZ 		= ceilingConcrete_posZ + ceilingConcrete_Z/2 + ceilingPE_Z/2;

	////////////////////////////////////////
	// floor
	G4double floorConcrete_thick 	= ceilingConcrete_thick;
	G4double floorDirt_thick 		= 5 *CLHEP::ft;

	G4double floorConcrete_X 	    = ceilingConcrete_X;
	G4double floorConcrete_Y 	    = ceilingConcrete_Y;
	G4double floorConcrete_Z 	    = floorConcrete_thick;

	G4double floorDirt_X 	    	= 3* floorConcrete_X;
	G4double floorDirt_Y 	    	= 3* floorConcrete_Y;
	G4double floorDirt_Z 	    	= floorDirt_thick;

	G4Box *floorConcrete 			= new G4Box("floorConcrete", floorConcrete_X/2, floorConcrete_Y/2, floorConcrete_Z/2);
	G4Box *floorDirt 				= new G4Box("floorDirt", floorDirt_X/2, floorDirt_Y/2, floorDirt_Z/2);

	G4double floorConcrete_posX 	= ceilingConcrete_posX;
	G4double floorConcrete_posY 	= ceilingConcrete_posY;
	G4double floorConcrete_posZ 	= -vault_Z/2 - floorConcrete_Z/2;

	G4double floorDirt_posX 		= floorConcrete_posX;
	G4double floorDirt_posY 		= floorConcrete_posY;
	G4double floorDirt_posZ 		= -vault_Z/2 - floorConcrete_Z/2 - floorDirt_Z/2;


	////////////////////////////////////////
	// Generator housing
	G4double generatorHousing_thick 		= 0.5 *CLHEP::cm;
	G4double generatorHousing_OD 			= 20.0 *CLHEP::cm;
	G4double generatorHousing_ID 			= generatorHousing_OD - 2*generatorHousing_thick;
	G4double generatorHousing_length 		= 50.0 *CLHEP::cm;

	G4double generatorHousingSteel_ID 		= 0.0 *CLHEP::mm;
	G4double generatorHousingSteel_OD 		= generatorHousing_OD;
	G4double generatorHousingSteel_length 	= generatorHousing_length;

	G4double generatorHousingVacuum_ID 		= 0.0 *CLHEP::mm;
	G4double generatorHousingVacuum_OD 		= generatorHousing_ID;
	G4double generatorHousingVacuum_length 	= generatorHousing_length - 2*generatorHousing_thick;

	G4Tubs *generatorHousingSteel	= new G4Tubs("generatorHousingSteel", generatorHousingSteel_ID/2, generatorHousingSteel_OD/2, generatorHousingSteel_length/2, opendeg, closedeg);	
	G4Tubs *generatorHousingVacuum	= new G4Tubs("generatorHousingVacuum", generatorHousingVacuum_ID/2, generatorHousingVacuum_OD/2, generatorHousingVacuum_length/2, opendeg, closedeg);	


	////////////////////////////////////////
	// Generator shield
	G4double generatorShieldCavity_width 	= 34.0 *CLHEP::cm;
	G4double generatorShieldCavity_length 	= 54.0 *CLHEP::cm;
	G4double generatorShieldLead_thick 		= 0.25 *CLHEP::in;

	G4double generatorShieldCavity_X 		= generatorShieldCavity_width;
	G4double generatorShieldCavity_Y 		= generatorShieldCavity_length;
	G4double generatorShieldCavity_Z 		= generatorShieldCavity_width;

	G4double generatorShieldLead_X 			= generatorShieldCavity_width + generatorShieldLead_thick*2;
	G4double generatorShieldLead_Y 			= generatorShieldCavity_length + generatorShieldLead_thick*2;
	G4double generatorShieldLead_Z 			= generatorShieldCavity_width + generatorShieldLead_thick*2;

	G4Box *generatorShieldLead 				= new G4Box("generatorShieldLead", generatorShieldLead_X/2, generatorShieldLead_Y/2, generatorShieldLead_Z/2);
	G4Box *generatorShieldCavity 			= new G4Box("generatorShieldCavity", generatorShieldCavity_X/2, generatorShieldCavity_Y/2, generatorShieldCavity_Z/2);

	G4double generator_height 				= 90.0 *CLHEP::cm;
	G4double beamdump_distance 				= 2.0 *CLHEP::m;

	G4double generatorShieldBPE_thick 		= 50.0 *CLHEP::cm;

	G4double generatorShieldBPE_X 			= generatorShieldLead_X + generatorShieldBPE_thick*2;
	G4double generatorShieldBPE_Y 			= generatorShieldLead_Y + generatorShieldBPE_thick*2;
	G4double generatorShieldBPE_Z 			= generator_height + generatorShieldLead_Z/2 + generatorShieldBPE_thick;

	G4Box *generatorShieldBPE 				= new G4Box("generatorShieldBPE", generatorShieldBPE_X/2, generatorShieldBPE_Y/2, generatorShieldBPE_Z/2);

	G4double generatorShieldBPE_posX 		= -vault_X/2 + 73*CLHEP::in;
	G4double generatorShieldBPE_posY 		= vault_Y/2 - beamdump_distance - generatorShieldBPE_Y/2;
	G4double generatorShieldBPE_posZ 		= -vault_Z/2 + generatorShieldBPE_Z/2;

	G4double generatorShieldLead_posX 		= 0;
	G4double generatorShieldLead_posY 		= 0;
	G4double generatorShieldLead_posZ 		= -generatorShieldBPE_Z/2 + generator_height;

	G4double generatorShieldCavity_posX 	= 0;
	G4double generatorShieldCavity_posY 	= 0;
	G4double generatorShieldCavity_posZ 	= 0;

	G4double generatorShieldChannel_width 	= 10.0 *CLHEP::cm;

	G4double generatorShieldBPEchannel_X 	= generatorShieldChannel_width;
	G4double generatorShieldBPEchannel_Y 	= generatorShieldBPE_thick;
	G4double generatorShieldBPEchannel_Z 	= generatorShieldChannel_width;

	G4double generatorShieldBPEchannel_posX = 0;
	G4double generatorShieldBPEchannel_posY = generatorShieldBPE_Y/2 - generatorShieldBPE_thick/2;
	G4double generatorShieldBPEchannel_posZ = -generatorShieldBPE_Z/2 + generator_height;

	G4Box *generatorShieldBPEchannel 		= new G4Box("generatorShieldBPEchannel", generatorShieldBPEchannel_X/2, generatorShieldBPEchannel_Y/2, generatorShieldBPEchannel_Z/2);

	G4cout <<"Generator shield dimensions: "<< G4endl;
	G4cout <<"ShieldBPE X: "<< generatorShieldBPE_X *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"ShieldBPE Y: "<< generatorShieldBPE_Y *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"ShieldBPE Z: "<< generatorShieldBPE_Z *CLHEP::mm <<" mm"<< G4endl;

	////////////////////////////////////////
	// tunnel for experiments
	//G4double tunnelBPE_thick 	= 20.0 *CLHEP::cm;
	G4double tunnelBPE_thick 	= 1.0 *CLHEP::mm;

	G4double tunnel_width 		= generatorShieldBPE_X;
	G4double tunnel_length 		= wallNorthBPE_posY - wallNorthBPE_thick/2 - generatorShieldBPE_posY - generatorShieldBPE_Y/2;
	G4double tunnel_height 		= generatorShieldBPE_Z;

	G4double tunnelBPE_X 		= tunnel_width;
	G4double tunnelBPE_Y 		= tunnel_length;
	G4double tunnelBPE_Z 		= tunnel_height;

	G4Box *tunnelBPE 			= new G4Box("tunnelBPE", tunnelBPE_X/2, tunnelBPE_Y/2, tunnelBPE_Z/2);

	G4cout <<"BPE tunnel dimensions: "<< G4endl;
	G4cout <<"BPE tunnel X: "<< tunnelBPE_X *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"BPE tunnel Y: "<< tunnelBPE_Y *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"BPE tunnel Z: "<< tunnelBPE_Z *CLHEP::mm <<" mm"<< G4endl;

	G4double tunnelCavity_X 	= tunnelBPE_X - tunnelBPE_thick*2;
	G4double tunnelCavity_Y 	= tunnelBPE_Y;
	G4double tunnelCavity_Z 	= tunnelBPE_Z - tunnelBPE_thick*2;

	G4Box *tunnelCavity 		= new G4Box("tunnelCavity", tunnelCavity_X/2, tunnelCavity_Y/2, tunnelCavity_Z/2);

	G4cout <<"Experimental cavity dimensions: "<< G4endl;
	G4cout <<"Tunnel X: "<< tunnelCavity_X *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"Tunnel Y: "<< tunnelCavity_Y *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"Tunnel Z: "<< tunnelCavity_Z *CLHEP::mm <<" mm"<< G4endl;

	G4double tunnelBPE_posX 	= generatorShieldBPE_posX;
	G4double tunnelBPE_posY 	= wallNorthBPE_posY - wallNorthBPE_thick/2 - tunnelBPE_Y/2;
	G4double tunnelBPE_posZ 	= generatorShieldBPE_posZ;

	G4double tunnelCavity_posX 	= 0;
	G4double tunnelCavity_posY 	= 0;
	G4double tunnelCavity_posZ 	= 0;

	// SMALL borated North wall
	//G4double wallNorthBPEsmall_thick = 5 *CLHEP::in;
	G4double wallNorthBPEsmall_thick = 1.0 *CLHEP::mm;
	G4double wallNorthBPEsmall_X 	= tunnelBPE_X;
	G4double wallNorthBPEsmall_Y 	= wallNorthBPEsmall_thick;
	G4double wallNorthBPEsmall_Z 	= tunnelBPE_Z;
	
	G4Box *wallNorthBPEsmall 	= new G4Box("wallNorthBPEsmall", wallNorthBPEsmall_X/2, wallNorthBPEsmall_Y/2, wallNorthBPEsmall_Z/2);

	G4double wallNorthBPEsmall_posX = tunnelBPE_posX;
	G4double wallNorthBPEsmall_posY = vault_Y/2 - wallNorthBPEsmall_Y/2;
	G4double wallNorthBPEsmall_posZ = tunnelBPE_posZ;

	G4cout <<"Beamdump dimensions: "<< G4endl;
	G4cout <<"Beamdump X: "<< wallNorthBPEsmall_X *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"Beamdump Y: "<< wallNorthBPEsmall_Y *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"Beamdump Z: "<< wallNorthBPEsmall_Z *CLHEP::mm <<" mm"<< G4endl;

	// screen to detect particles
	G4double screen0_thick 	= 0.001 *CLHEP::mm;
	G4double screen0_gap 	= 0.002 *CLHEP::mm;

	// solid block
	G4double screen0l_X 	= generatorShieldBPE_X + screen0_thick*2 + screen0_gap*2;
	G4double screen0l_Y 	= generatorShieldBPE_Y + screen0_thick*2 + screen0_gap*2;
	G4double screen0l_Z 	= generatorShieldBPE_Z + screen0_thick + screen0_gap;

	// pocket in the block to fit the generator shield
	G4double screen0s_X 	= generatorShieldBPE_X + screen0_gap*2;
	G4double screen0s_Y 	= generatorShieldBPE_Y + screen0_gap*2;
	G4double screen0s_Z 	= generatorShieldBPE_Z + screen0_gap;

	G4Box *screen0l 		= new G4Box("screen0l", screen0l_X/2, screen0l_Y/2, screen0l_Z/2);
	G4Box *screen0s 		= new G4Box("screen0s", screen0s_X/2, screen0s_Y/2, screen0s_Z/2);

	G4ThreeVector screen0_V	(0.0, 0.0, -screen0l_Z/2 + screen0s_Z/2);	
	G4Transform3D screen0_T	(ZeroRot, screen0_V);

	G4SubtractionSolid *screen0	= new G4SubtractionSolid( "screen0", screen0l, screen0s, screen0_T);

	// screen 0, box around the neutron generator shield
	G4double screen0_posX 	= generatorShieldBPE_posX;
	G4double screen0_posY 	= generatorShieldBPE_posY;
	G4double screen0_posZ 	= -vault_Z/2 + screen0l_Z/2;

	G4cout <<"Screen dimensions: "<< G4endl;
	G4cout <<"Screen X: "<< screen0l_X *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"Screen Y: "<< screen0l_Y *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"Screen Z: "<< screen0l_Z *CLHEP::mm <<" mm"<< G4endl;

	G4cout <<"Screen 0 position: "<< G4endl;
	G4cout <<"Screen X0: "<< screen0_posX *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"Screen Y0: "<< screen0_posY *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"Screen Z0: "<< screen0_posZ *CLHEP::mm <<" mm"<< G4endl;

	// screen 1, panel 1m from the generator exit
	G4double screen1_distance 	= 1.0 *CLHEP::m;
	
	G4double screen1_X 	= vault_X;
	G4double screen1_Y 	= 0.001 *CLHEP::mm;
	G4double screen1_Z 	= vault_Z;
	G4Box *screen1 		= new G4Box("screen1", screen1_X/2, screen1_Y/2, screen1_Z/2);
	G4double screen1_posX 	= 0;
	G4double screen1_posY 	= vault_Y/2 - screen1_distance;
	G4double screen1_posZ 	= 0;

	G4cout <<"Screen 1 position: "<< G4endl;
	G4cout <<"Screen X1: "<< screen1_posX *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"Screen Y1: "<< screen1_posY *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"Screen Z1: "<< screen1_posZ *CLHEP::mm <<" mm"<< G4endl;

	// screen 2, panel 1m behing the generator shield
	G4double screen2_distance 	= 1.0 *CLHEP::m;
	
	G4double screen2_X 	= vault_X;
	G4double screen2_Y 	= 0.001 *CLHEP::mm;
	G4double screen2_Z 	= vault_Z;
	G4Box *screen2 		= new G4Box("screen1", screen2_X/2, screen2_Y/2, screen2_Z/2);

	G4double screen2_posX 	= 0;
	G4double screen2_posY 	= generatorShieldBPE_posY - generatorShieldBPE_Y/2 - screen2_distance;
	G4double screen2_posZ 	= 0;
	G4cout <<"Screen 2 position: "<< G4endl;
	G4cout <<"Screen X2: "<< screen2_posX *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"Screen Y2: "<< screen2_posY *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"Screen Z2: "<< screen2_posZ *CLHEP::mm <<" mm"<< G4endl;

	// screen 3, steel block 2m behind the shield
	G4double screen3_distance 	= 2.0 *CLHEP::m;
	
	G4double screen3_X 	= 50.0 *CLHEP::cm;
	G4double screen3_Y 	= screen3_X;
	G4double screen3_Z 	= screen3_X;
	G4Box *screen3 		= new G4Box("screen3", screen3_X/2, screen3_Y/2, screen3_Z/2);

	G4double screen3_posX 	= generatorShieldBPE_posX;
	G4double screen3_posY 	= generatorShieldBPE_posY - generatorShieldBPE_Y/2 - screen3_distance;
	G4double screen3_posZ 	= generatorShieldBPE_posZ;
	G4cout <<"Screen 3 position: "<< G4endl;
	G4cout <<"Screen X3: "<< screen3_posX *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"Screen Y3: "<< screen3_posY *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"Screen Z3: "<< screen3_posZ *CLHEP::mm <<" mm"<< G4endl;

	// screen 4, in the middle of the experimental tunnel
	G4double screen4_X 	= tunnelCavity_X;
	G4double screen4_Y 	= 0.001 *CLHEP::mm;
	G4double screen4_Z 	= tunnelCavity_Z;
	G4Box *screen4 		= new G4Box("screen4", screen4_X/2, screen4_Y/2, screen4_Z/2);

	G4cout <<"Screen 4 dimensions: "<< G4endl;
	G4cout <<"Screen X4: "<< screen4_X *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"Screen Y4: "<< screen4_Y *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"Screen Z4: "<< screen4_Z *CLHEP::mm <<" mm"<< G4endl;

	G4double screen4_posX 	= 0;
	G4double screen4_posY 	= 0;
	G4double screen4_posZ 	= 0;
	G4cout <<"Screen 4 position: "<< G4endl;
	G4cout <<"Screen X4: "<< screen4_posX *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"Screen Y4: "<< screen4_posY *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"Screen Z4: "<< screen4_posZ *CLHEP::mm <<" mm"<< G4endl;

	// Li-6 thermal neutron absorber
	G4double LiLayer_X 	= tunnelCavity_X;
	G4double LiLayer_Y 	= 0.5 *CLHEP::mm;
	G4double LiLayer_Z 	= tunnelCavity_Z;
	G4Box *LiLayer 		= new G4Box("LiLayer", LiLayer_X/2, LiLayer_Y/2, LiLayer_Z/2);

	G4cout <<"LiLayer dimensions: "<< G4endl;
	G4cout <<"LiLayer X4: "<< LiLayer_X *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"LiLayer Y4: "<< LiLayer_Y *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"LiLayer Z4: "<< LiLayer_Z *CLHEP::mm <<" mm"<< G4endl;

	G4double LiLayer_posX 	= 0;
	G4double LiLayer_posY 	= tunnelCavity_posY - tunnelCavity_Y/2 + LiLayer_Y/2;
	G4double LiLayer_posZ 	= 0;
	G4cout <<"LiLayer position: "<< G4endl;
	G4cout <<"LiLayer X: "<< LiLayer_posX *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"LiLayer Y: "<< LiLayer_posY *CLHEP::mm <<" mm"<< G4endl;
	G4cout <<"LiLayer Z: "<< LiLayer_posZ *CLHEP::mm <<" mm"<< G4endl;

	// sulfur filter
	G4double filter_thick 		= 5*CLHEP::cm;	
	G4double filter_X			= generatorShieldChannel_width;
	G4double filter_Y			= filter_thick;
	G4double filter_Z			= generatorShieldChannel_width;
	G4Box *filter 				= new G4Box("filter", filter_X/2, filter_Y/2, filter_Z/2);

	G4double filter_posX 		= 0;
	G4double filter_posY 		= generatorShieldBPEchannel_Y/2 - filter_thick/2;
	G4double filter_posZ 		= 0;

	// moderator
	G4double moderator_thick 	= 5*CLHEP::cm;	
	G4double moderator_X		= generatorShieldChannel_width;
	G4double moderator_Y		= moderator_thick;
	G4double moderator_Z		= generatorShieldChannel_width;
	G4Box *moderator 			= new G4Box("moderator", moderator_X/2, moderator_Y/2, moderator_Z/2);

	G4double moderator_posX 	= 0;
	G4double moderator_posY 	= filter_posY/2 - filter_thick/2 - moderator_thick/2;
	G4double moderator_posZ 	= 0;

	///////////////////////////////////////////////////////////////
	// Logical Volumes (declared in 'NeutronLabDetectorGeometry.hh')
	G4LogicalVolume *Laboratory_log 				= new G4LogicalVolume(Laboratory, 				 Air, 		"Laboratory_log");
	G4LogicalVolume *vault_log 						= new G4LogicalVolume(vault, 					 Air, 		"vault_log");
	G4LogicalVolume *wallWestSteel_log 				= new G4LogicalVolume(wallWestSteel, 			 Steel, 	"wallWestSteel_log");
	G4LogicalVolume *wallWestConcrete_log 			= new G4LogicalVolume(wallWestConcrete, 		 Concrete, 	"wallWestConcrete_log");
	G4LogicalVolume *wallEastConcrete_log 			= new G4LogicalVolume(wallEastConcrete, 		 Concrete, 	"wallEastConcrete_log");
	G4LogicalVolume *wallEastDirt_log 				= new G4LogicalVolume(wallEastDirt, 			 rock, 		"wallEastDirt_log");
	G4LogicalVolume *wallNorthSteel_log 			= new G4LogicalVolume(wallNorthSteel, 			 Steel, 	"wallNorthSteel_log");
	G4LogicalVolume *wallNorthConcrete_log 			= new G4LogicalVolume(wallNorthConcrete, 		 Concrete, 	"wallNorthConcrete_log");
	//G4LogicalVolume *wallNorthBPE_log 				= new G4LogicalVolume(wallNorthBPE, 			 BoricAcid, "wallNorthBPE_log");
	G4LogicalVolume *wallSouthSteel_log 			= new G4LogicalVolume(wallSouthSteel, 			 Steel, 	"wallSouthSteel_log");
	G4LogicalVolume *wallSouthConcrete_log 			= new G4LogicalVolume(wallSouthConcrete, 		 Concrete, 	"wallSouthConcrete_log");
	G4LogicalVolume *pillar_log 					= new G4LogicalVolume(pillar, 					 Concrete, 	"pillar_log");
	G4LogicalVolume *ceilingSteel_log 				= new G4LogicalVolume(ceilingSteel, 			 Steel, 	"ceilingSteel_log");
	G4LogicalVolume *ceilingConcrete_log 			= new G4LogicalVolume(ceilingConcrete, 			 Concrete, 	"ceilingConcrete_log");
	G4LogicalVolume *ceilingPE_log 					= new G4LogicalVolume(ceilingPE, 				 PE, 		"ceilingPE_log");
	G4LogicalVolume *floorConcrete_log 				= new G4LogicalVolume(floorConcrete, 			 Concrete, 	"floorConcrete_log");
	G4LogicalVolume *floorDirt_log 					= new G4LogicalVolume(floorDirt, 				 rock, 		"floorDirt_log");
	G4LogicalVolume *generatorShieldBPE_log  		= new G4LogicalVolume(generatorShieldBPE, 		 BPE5, 		"generatorShieldBPE_log");
	G4LogicalVolume *generatorShieldLead_log  		= new G4LogicalVolume(generatorShieldLead, 		 Lead_mat, 	"generatorShieldLead_log");
	G4LogicalVolume *generatorShieldCavity_log  	= new G4LogicalVolume(generatorShieldCavity, 	 Air, 		"generatorShieldCavity_log");
	G4LogicalVolume *generatorHousingSteel_log 		= new G4LogicalVolume(generatorHousingSteel, 	 Steel304, 	"generatorHousingSteel_log");
	G4LogicalVolume *generatorHousingVacuum_log 	= new G4LogicalVolume(generatorHousingVacuum, 	 Vacuum, 	"generatorHousingVacuum_log");
	G4LogicalVolume *generatorShieldBPEchannel_log  = new G4LogicalVolume(generatorShieldBPEchannel, Air, 		"generatorShieldBPE_log");

	G4LogicalVolume *wallNorthBPEsmall_log 			= new G4LogicalVolume(wallNorthBPEsmall, 		 BPE5,		"wallNorthBPEsmall_log");
	G4LogicalVolume *tunnelBPE_log  				= new G4LogicalVolume(tunnelBPE, 				 BPE5, 		"tunnelBPE_log");
	G4LogicalVolume *tunnelCavity_log  				= new G4LogicalVolume(tunnelCavity, 			 Air, 		"tunnelCavity_log");

	G4LogicalVolume *screen0_log  					= new G4LogicalVolume(screen0, 					 Air, 		"screen0_log");
	G4LogicalVolume *screen1_log  					= new G4LogicalVolume(screen1, 					 Air, 		"screen1_log");
	G4LogicalVolume *screen2_log  					= new G4LogicalVolume(screen2, 					 Air, 		"screen2_log");
	G4LogicalVolume *screen3_log  					= new G4LogicalVolume(screen3, 					 Steel304, 	"screen3_log");
	G4LogicalVolume *screen4_log  					= new G4LogicalVolume(screen4, 					 Air, 		"screen4_log");

	G4LogicalVolume *LiLayer_log  					= new G4LogicalVolume(LiLayer, 					 LiMat, 	"LiLayer_log");
	G4LogicalVolume *moderator_log  				= new G4LogicalVolume(moderator, 				 SiMat, 	"moderator_log");
	G4LogicalVolume *filter_log  					= new G4LogicalVolume(filter, 				 	 SMat, 		"filter_log");

	// Physical Volumes
	G4PVPlacement *Laboratory_phys 			= new G4PVPlacement(0, G4ThreeVector(0, 0, 0), Laboratory_log, "Laboratory_phys", 0, false, 0);
	G4PVPlacement *vault_phys 				= new G4PVPlacement(0, G4ThreeVector(0, 0, 0), vault_log, "vault_phys", Laboratory_log, false, 0);
	G4PVPlacement *wallWestSteel1_phys 		= new G4PVPlacement(0, G4ThreeVector(wallWestSteel1_posX, wallWestSteel1_posY, wallWestSteel1_posZ), wallWestSteel_log, "wallWestSteel1_phys", Laboratory_log, false, 0);
	G4PVPlacement *wallWestConcrete1_phys 	= new G4PVPlacement(0, G4ThreeVector(wallWestConcrete1_posX, wallWestConcrete1_posY, wallWestConcrete1_posZ), wallWestConcrete_log, "wallWestConcrete1_phys", Laboratory_log, false, 0);
	G4PVPlacement *wallWestSteel2_phys 		= new G4PVPlacement(0, G4ThreeVector(wallWestSteel2_posX, wallWestSteel2_posY, wallWestSteel2_posZ), wallWestSteel_log, "wallWestSteel2_phys", Laboratory_log, false, 0);
	G4PVPlacement *wallWestConcrete2_phys 	= new G4PVPlacement(0, G4ThreeVector(wallWestConcrete2_posX, wallWestConcrete2_posY, wallWestConcrete2_posZ), wallWestConcrete_log, "wallWestConcrete2_phys", Laboratory_log, false, 0);
	G4PVPlacement *wallEastConcrete_phys 	= new G4PVPlacement(0, G4ThreeVector(wallEastConcrete_posX, wallEastConcrete_posY, wallEastConcrete_posZ), wallEastConcrete_log, "wallEastConcrete_phys", Laboratory_log, false, 0);
	G4PVPlacement *wallEastDirt_phys 		= new G4PVPlacement(0, G4ThreeVector(wallEastDirt_posX, wallEastDirt_posY, wallEastDirt_posZ), wallEastDirt_log, "wallEastDirt_phys", Laboratory_log, false, 0);
	G4PVPlacement *wallNorthSteel1_phys 	= new G4PVPlacement(0, G4ThreeVector(wallNorthSteel1_posX, wallNorthSteel1_posY, wallNorthSteel1_posZ), wallNorthSteel_log, "wallNorthSteel1_phys", Laboratory_log, false, 0);
	G4PVPlacement *wallNorthConcrete1_phys 	= new G4PVPlacement(0, G4ThreeVector(wallNorthConcrete1_posX, wallNorthConcrete1_posY, wallNorthConcrete1_posZ), wallNorthConcrete_log, "wallNorthConcrete1_phys", Laboratory_log, false, 0);
	G4PVPlacement *wallNorthSteel2_phys 	= new G4PVPlacement(0, G4ThreeVector(wallNorthSteel2_posX, wallNorthSteel2_posY, wallNorthSteel2_posZ), wallNorthSteel_log, "wallNorthSteel2_phys", Laboratory_log, false, 0);
	G4PVPlacement *wallNorthConcrete2_phys 	= new G4PVPlacement(0, G4ThreeVector(wallNorthConcrete2_posX, wallNorthConcrete2_posY, wallNorthConcrete2_posZ), wallNorthConcrete_log, "wallNorthConcrete2_phys", Laboratory_log, false, 0);
	//G4PVPlacement *wallNorthBPE_phys 		= new G4PVPlacement(0, G4ThreeVector(wallNorthBPE_posX, wallNorthBPE_posY, wallNorthBPE_posZ), wallNorthBPE_log, "wallNorthBPE_phys", Laboratory_log, false, 0);
	G4PVPlacement *wallSouthSteel1_phys 	= new G4PVPlacement(0, G4ThreeVector(wallSouthSteel1_posX, wallSouthSteel1_posY, wallSouthSteel1_posZ), wallSouthSteel_log, "wallSouthSteel1_phys", Laboratory_log, false, 0);
	G4PVPlacement *wallSouthConcrete1_phys 	= new G4PVPlacement(0, G4ThreeVector(wallSouthConcrete1_posX, wallSouthConcrete1_posY, wallSouthConcrete1_posZ), wallSouthConcrete_log, "wallSouthConcrete1_phys", Laboratory_log, false, 0);
	G4PVPlacement *wallSouthSteel2_phys 	= new G4PVPlacement(0, G4ThreeVector(wallSouthSteel2_posX, wallSouthSteel2_posY, wallSouthSteel2_posZ), wallSouthSteel_log, "wallSouthSteel2_phys", Laboratory_log, false, 0);
	G4PVPlacement *wallSouthConcrete2_phys 	= new G4PVPlacement(0, G4ThreeVector(wallSouthConcrete2_posX, wallSouthConcrete2_posY, wallSouthConcrete2_posZ), wallSouthConcrete_log, "wallSouthConcrete2_phys", Laboratory_log, false, 0);
	G4PVPlacement *pillar1_phys 			= new G4PVPlacement(0, G4ThreeVector(pillar_posX, pillar1_posY, 0), pillar_log, "pillar1_phys", vault_log, false, 0);
	G4PVPlacement *pillar2_phys 			= new G4PVPlacement(0, G4ThreeVector(pillar_posX, pillar2_posY, 0), pillar_log, "pillar2_phys", vault_log, false, 0);
	G4PVPlacement *pillar3_phys 			= new G4PVPlacement(0, G4ThreeVector(pillar_posX, pillar3_posY, 0), pillar_log, "pillar3_phys", vault_log, false, 0);
	G4PVPlacement *pillar4_phys 			= new G4PVPlacement(0, G4ThreeVector(pillar_posX, pillar4_posY, 0), pillar_log, "pillar4_phys", vault_log, false, 0);
	G4PVPlacement *pillar5_phys 			= new G4PVPlacement(0, G4ThreeVector(pillar_posX, pillar5_posY, 0), pillar_log, "pillar5_phys", vault_log, false, 0);
	G4PVPlacement *pillar6_phys 			= new G4PVPlacement(0, G4ThreeVector(pillar_posX, pillar6_posY, 0), pillar_log, "pillar6_phys", vault_log, false, 0);
	G4PVPlacement *ceilingSteel_phys 		= new G4PVPlacement(0, G4ThreeVector(ceilingSteel_posX, ceilingSteel_posY, ceilingSteel_posZ), ceilingSteel_log, "ceilingSteel_phys", Laboratory_log, false, 0);
	G4PVPlacement *ceilingConcrete_phys 	= new G4PVPlacement(0, G4ThreeVector(ceilingConcrete_posX, ceilingConcrete_posY, ceilingConcrete_posZ), ceilingConcrete_log, "ceilingConcrete_phys", Laboratory_log, false, 0);
	G4PVPlacement *ceilingPE_phys 			= new G4PVPlacement(0, G4ThreeVector(ceilingPE_posX, ceilingPE_posY, ceilingPE_posZ), ceilingPE_log, "ceilingPE_phys", Laboratory_log, false, 0);
	G4PVPlacement *floorConcrete_phys 		= new G4PVPlacement(0, G4ThreeVector(floorConcrete_posX, floorConcrete_posY, floorConcrete_posZ), floorConcrete_log, "floorConcrete_phys", Laboratory_log, false, 0);
	G4PVPlacement *floorDirt_phys 			= new G4PVPlacement(0, G4ThreeVector(floorDirt_posX, floorDirt_posY, floorDirt_posZ), floorDirt_log, "floorDirt_phys", Laboratory_log, false, 0);

	G4PVPlacement *generatorShieldBPE_phys 			= new G4PVPlacement(0, G4ThreeVector(generatorShieldBPE_posX, generatorShieldBPE_posY, generatorShieldBPE_posZ), generatorShieldBPE_log, "generatorShieldBPE_phys", vault_log, false, 0);
	G4PVPlacement *generatorShieldLead_phys 		= new G4PVPlacement(0, G4ThreeVector(generatorShieldLead_posX, generatorShieldLead_posY, generatorShieldLead_posZ), generatorShieldLead_log, "generatorShieldLead_phys", generatorShieldBPE_log, false, 0);
	G4PVPlacement *generatorShieldCavity_phys 		= new G4PVPlacement(0, G4ThreeVector(generatorShieldCavity_posX, generatorShieldCavity_posY, generatorShieldCavity_posZ), generatorShieldCavity_log, "generatorShieldCavity_phys", generatorShieldLead_log, false, 0);
	G4PVPlacement *generatorHousingSteel_phys 		= new G4PVPlacement(RotationXPlus90, G4ThreeVector(0, 0, 0), generatorHousingSteel_log, "generatorHousingSteel_phys", generatorShieldCavity_log, false, 0);
	G4PVPlacement *generatorHousingVacuum_phys 		= new G4PVPlacement(0, G4ThreeVector(0, 0, 0), generatorHousingVacuum_log, "generatorHousingVacuum_phys", generatorHousingSteel_log, false, 0);
	G4PVPlacement *generatorShieldBPEchannel_phys 	= new G4PVPlacement(0, G4ThreeVector(generatorShieldBPEchannel_posX, generatorShieldBPEchannel_posY, generatorShieldBPEchannel_posZ), generatorShieldBPEchannel_log, "generatorShieldBPEchannel_phys", generatorShieldBPE_log, false, 0);

	G4PVPlacement *wallNorthBPEsmall_phys 	= new G4PVPlacement(0, G4ThreeVector(wallNorthBPEsmall_posX, wallNorthBPEsmall_posY, wallNorthBPEsmall_posZ), wallNorthBPEsmall_log, "wallNorthBPEsmall_phys", Laboratory_log, false, 0);
	G4PVPlacement *tunnelBPE_phys 			= new G4PVPlacement(0, G4ThreeVector(tunnelBPE_posX, tunnelBPE_posY, tunnelBPE_posZ), tunnelBPE_log, "tunnelBPE_phys", vault_log, false, 0);
	G4PVPlacement *tunnelCavity_phys 		= new G4PVPlacement(0, G4ThreeVector(tunnelCavity_posX, tunnelCavity_posY, tunnelCavity_posZ), tunnelCavity_log, "tunnelCavity_phys", tunnelBPE_log, false, 0);

	//G4PVPlacement *screen0_phys 			= new G4PVPlacement(0, G4ThreeVector(screen0_posX, screen0_posY, screen0_posZ), screen0_log, "screen0_phys", vault_log, false, 0);
	//G4PVPlacement *screen1_phys 			= new G4PVPlacement(0, G4ThreeVector(screen1_posX, screen1_posY, screen1_posZ), screen1_log, "screen1_phys", vault_log, false, 0);
	G4PVPlacement *screen2_phys 			= new G4PVPlacement(0, G4ThreeVector(screen2_posX, screen2_posY, screen2_posZ), screen2_log, "screen2_phys", vault_log, false, 0);
	G4PVPlacement *screen3_phys 			= new G4PVPlacement(0, G4ThreeVector(screen3_posX, screen3_posY, screen3_posZ), screen3_log, "screen3_phys", vault_log, false, 0);
	G4PVPlacement *screen4_phys 			= new G4PVPlacement(0, G4ThreeVector(screen4_posX, screen4_posY, screen4_posZ), screen4_log, "screen4_phys", tunnelCavity_log, false, 0);

	//G4PVPlacement *LiLayer_phys 			= new G4PVPlacement(0, G4ThreeVector(LiLayer_posX, LiLayer_posY, LiLayer_posZ), LiLayer_log, "LiLayer_phys", tunnelCavity_log, false, 0);
	//G4PVPlacement *filter_phys 				= new G4PVPlacement(0, G4ThreeVector(filter_posX, filter_posY, filter_posZ), filter_log, "filter_phys", generatorShieldBPEchannel_log, false, 0);
	//G4PVPlacement *moderator_phys 			= new G4PVPlacement(0, G4ThreeVector(moderator_posX, moderator_posY, moderator_posZ), moderator_log, "moderator_phys", generatorShieldBPEchannel_log, false, 0);

	///////////////////////////////////////////////////////////////////////
	// Visual Attributes
	Laboratory_log				->SetVisAttributes(G4VisAttributes::Invisible);		
	ceilingSteel_log			->SetVisAttributes(G4VisAttributes::Invisible);
	ceilingConcrete_log			->SetVisAttributes(G4VisAttributes::Invisible);
	ceilingPE_log				->SetVisAttributes(G4VisAttributes::Invisible);
	generatorShieldCavity_log	->SetVisAttributes(G4VisAttributes::Invisible);
	//generatorHousingVacuum_log	->SetVisAttributes(G4VisAttributes::Invisible);

	G4VisAttributes *vault_vis = new G4VisAttributes(yellow);
	vault_vis					->SetVisibility(false);
	vault_log					->SetVisAttributes(vault_vis);

	G4VisAttributes *steel_vis = new G4VisAttributes(blue);
	steel_vis					->SetVisibility(true);
	steel_vis					->SetForceSolid(true);
	wallWestSteel_log			->SetVisAttributes(steel_vis);
	wallNorthSteel_log			->SetVisAttributes(steel_vis);
	wallSouthSteel_log			->SetVisAttributes(steel_vis);
	//ceilingSteel_log			->SetVisAttributes(steel_vis);
	generatorHousingSteel_log	->SetVisAttributes(steel_vis);

	G4VisAttributes *concrete_vis = new G4VisAttributes(grey);
	concrete_vis				->SetVisibility(true);
	concrete_vis				->SetForceSolid(true);
	wallWestConcrete_log		->SetVisAttributes(concrete_vis);
	wallEastConcrete_log		->SetVisAttributes(concrete_vis);
	wallNorthConcrete_log		->SetVisAttributes(concrete_vis);
	wallSouthConcrete_log		->SetVisAttributes(concrete_vis);
	pillar_log					->SetVisAttributes(concrete_vis);
	//ceilingConcrete_log		->SetVisAttributes(concrete_vis);
	floorConcrete_log			->SetVisAttributes(concrete_vis);

	G4VisAttributes *PE_vis = new G4VisAttributes(white);
	PE_vis						->SetVisibility(true);
	PE_vis						->SetForceSolid(true);
	//ceilingPE_log				->SetVisAttributes(PE_vis);
	tunnelCavity_log			->SetVisAttributes(PE_vis);

	G4VisAttributes *BPE_vis = new G4VisAttributes(orange);
	BPE_vis						->SetVisibility(true);
	BPE_vis						->SetForceSolid(true);
	//wallNorthBPE_log			->SetVisAttributes(BPE_vis);
	generatorShieldBPE_log		->SetVisAttributes(BPE_vis);
	tunnelBPE_log				->SetVisAttributes(BPE_vis);

	G4VisAttributes *BPElight_vis = new G4VisAttributes(brown);
	BPElight_vis				->SetVisibility(true);
	BPElight_vis				->SetForceSolid(true);
	wallNorthBPEsmall_log		->SetVisAttributes(BPElight_vis);

	G4VisAttributes *dirt_vis = new G4VisAttributes(brown);
	dirt_vis					->SetVisibility(false);
	dirt_vis					->SetForceSolid(true);
	floorDirt_log				->SetVisAttributes(dirt_vis);
	wallEastDirt_log			->SetVisAttributes(dirt_vis);

	G4VisAttributes *lead_vis = new G4VisAttributes(green);
	lead_vis					->SetVisibility(true);
	lead_vis					->SetForceSolid(true);
	generatorShieldLead_log		->SetVisAttributes(lead_vis);

	G4VisAttributes *screen_vis = new G4VisAttributes(xlgreen);
	generatorHousingVacuum_log	->SetVisAttributes(screen_vis);
	screen_vis					->SetVisibility(true);
	screen_vis					->SetForceSolid(true);
	//screen0_log					->SetVisAttributes(screen_vis);
	//screen1_log					->SetVisAttributes(screen_vis);
	screen2_log					->SetVisAttributes(screen_vis);
	screen3_log					->SetVisAttributes(steel_vis);
	screen4_log					->SetVisAttributes(screen_vis);

	G4VisAttributes *Li_vis = new G4VisAttributes(red);
	Li_vis					->SetVisibility(true);
	Li_vis					->SetForceSolid(true);
	LiLayer_log				->SetVisAttributes(Li_vis);

	G4VisAttributes *moderator_vis = new G4VisAttributes(magenta);
	moderator_vis				->SetVisibility(true);
	moderator_vis				->SetForceSolid(true);
	moderator_log				->SetVisAttributes(moderator_vis);

	G4VisAttributes *filter_vis = new G4VisAttributes(yellow);
	filter_vis				->SetVisibility(true);
	filter_vis				->SetForceSolid(true);
	filter_log				->SetVisAttributes(filter_vis);

	// get mass of borated PE wall
	//G4double mass_beamdump = 0.;
	//mass_beamdump += wallNorthBPE_log ->GetMass(false, false)/CLHEP::kg;
	//G4cout 	<< "Mass of beamdump/BPE:" << mass_beamdump << " kg" << G4endl;
	G4double mass_beamdumpSmall = 0.;
	mass_beamdumpSmall += wallNorthBPEsmall_log ->GetMass(false, false)/CLHEP::kg;
	G4cout 	<< "Mass of beamdumpSmall/BPE:" << mass_beamdumpSmall << " kg" << G4endl;

	// get mass of generator shield: BPE
	G4double mass_generatorShieldBPE = 0.;
	mass_generatorShieldBPE += generatorShieldBPE_log ->GetMass(false, false)/CLHEP::kg;
	G4cout 	<< "Mass of generator shield (BPE):" << mass_generatorShieldBPE << " kg" << G4endl;

	// get mass of generator shield: BPE
	G4double mass_tunnelBPE = 0.;
	mass_tunnelBPE += tunnelBPE_log ->GetMass(false, false)/CLHEP::kg;
	G4cout 	<< "Mass of the tunnel (BPE):" << mass_tunnelBPE << " kg" << G4endl;

	// get mass of generator shield: Lead
	G4double mass_generatorShieldLead = 0.;
	mass_generatorShieldLead += generatorShieldLead_log ->GetMass(false, false)/CLHEP::kg;
	G4cout 	<< "Mass of generator shield (Lead):" << mass_generatorShieldLead << " kg" << G4endl;

	// get mass of generator steel housing
	G4double mass_generatorHousingSteel = 0.;
	mass_generatorHousingSteel += generatorHousingSteel_log ->GetMass(false, false)/CLHEP::kg;
	G4cout 	<< "Mass of generator housing (Steel):" << mass_generatorHousingSteel << " kg" << G4endl;




	// SENSITIVE DETECTOR
	G4SDManager *pSDManager = G4SDManager::GetSDMpointer();

	// make entire laboratory sensitive
	ScreenSensitiveDetector *pScreenSD = new ScreenSensitiveDetector("ScreenSD");
	pSDManager->AddNewDetector(pScreenSD);
	//screen0_log	->SetSensitiveDetector(pScreenSD);
	//screen1_log	->SetSensitiveDetector(pScreenSD);
	//screen2_log	->SetSensitiveDetector(pScreenSD);
	//screen3_log	->SetSensitiveDetector(pScreenSD);
	screen4_log	->SetSensitiveDetector(pScreenSD);

	//wallWestSteel_log	->SetSensitiveDetector(pScreenSD);
	//wallNorthSteel_log	->SetSensitiveDetector(pScreenSD);
	//wallSouthSteel_log	->SetSensitiveDetector(pScreenSD);


//--------------	END PROGRAM	------------------------
return Laboratory_phys;
//------------------------------------------------------

}


//NeutronLabDetectorConstruction::~NeutronLabDetectorConstruction()
//{
//delete m_pDetectorMessenger;
//}








