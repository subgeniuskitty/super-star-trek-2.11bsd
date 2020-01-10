#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#ifndef INCLUDED
#define EXTERN extern
#else
#define EXTERN
#endif

#ifdef CLOAKING
#define ndevice (16)
#else
#define ndevice (15)	/* Number of devices */
#endif
#define phasefac (2.0)
#define PLNETMAX (10)
#define NEVENTS (8)

typedef struct {
	int x;	/* Quadrant location of planet */
	int y;
	int pclass; /* class M, N, or O (1, 2, or 3) */
	int crystals; /* has crystals */
	int known;   /* =1 contents known, =2 shuttle on this planet */
} PLANETS;

EXTERN struct foo {
		int snap,		/* snapshot taken */
		remkl,			/* remaining klingons */
	        remcom,			/* remaining commanders */
		rembase,		/* remaining bases */
		starkl,			/* destroyed stars */
		basekl,			/* destroyed bases */
		killk,			/* Klingons killed */
		killc,			/* commanders killed */
		galaxy[9][9], 	/* The Galaxy (subscript 0 not used) */
		cx[11],cy[11],	/* Commander quadrant coordinates */
		baseqx[6],		/* Base quadrant X */
		baseqy[6],		/* Base quadrant Y */
		newstuf[9][9],	/* Extended galaxy goodies */
		isx, isy,		/* Coordinate of Super Commander */
		nscrem,			/* remaining super commanders */
		nromkl,			/* Romulans killed */
		nromrem,		/* Romulans remaining */
		nsckill,		/* super commanders killed */
		nplankl;		/* destroyed planets */
	PLANETS plnets[PLNETMAX+1];  /* Planet information */
#ifdef CAPTURE
    int f1_kcaptured, f1_brigfree;
#endif
	double date,		/* stardate */
		remres,			/* remaining resources */
	    remtime;		/* remaining time */
} d, snapsht;			/* Data that is snapshot */

EXTERN char
		quad[11][11];	/* contents of our quadrant */

/*
 * Scalar variables that are needed for freezing the game
 * are placed in a structure. #defines are used to access by their
 * original names. Gee, I could have done this with the d structure,
 * but I just didn't think of it back when I started.
 */

EXTERN struct foo2 {
	int f2_inkling,
	f2_inbase,
	f2_incom,
	f2_instar,
	f2_intorps,
	f2_condit,
	f2_torps,
	f2_ship,
	f2_quadx,
	f2_quady,
	f2_sectx,
	f2_secty,
	f2_length,
	f2_skill,
	f2_basex,
	f2_basey,
	f2_klhere,
	f2_comhere,
	f2_casual,
	f2_nhelp,
	f2_nkinks,
	f2_ididit,
	f2_gamewon,
	f2_alive,
	f2_justin,
	f2_alldone,
	f2_shldchg,
	f2_thingx,
	f2_thingy,
	f2_plnetx,
	f2_plnety,
	f2_inorbit,
	f2_landed,
	f2_iplnet,
	f2_imine,
	f2_inplan,
	f2_nenhere,
	f2_ishere,
	f2_neutz,
	f2_irhere,
	f2_icraft,
	f2_ientesc,
	f2_iscraft,
	f2_isatb,
	f2_iscate,
#ifdef DEBUG
	f2_idebug,
#endif
#ifdef CLOAKING
    f2_iscloaked,
    f2_iscloaking,
    f2_ncviol,
    f2_isviolreported,
#endif
#ifdef CAPTURE
    f2_brigcapacity,
#endif
	f2_iattak,
	f2_icrystl,
	f2_tourn,
	f2_thawed,
	f2_batx,
	f2_baty,
	f2_ithere,
	f2_ithx,
	f2_ithy,
	f2_iseenit,
	f2_probecx,
	f2_probecy,
	f2_proben,
	f2_isarmed,
	f2_nprobes;

	double f2_inresor,
	f2_intime,
	f2_inenrg,
	f2_inshld,
	f2_inlsr,
	f2_indate,
	f2_energy,
	f2_shield,
	f2_shldup,
	f2_warpfac,
	f2_wfacsq,
	f2_lsupres,
	f2_dist,
	f2_direc,
	f2_Time,
	f2_docfac,
	f2_resting,
	f2_damfac,
	f2_stdamtim,
	f2_cryprob,
	f2_probex,
	f2_probey,
	f2_probeinx,
	f2_probeiny;
} a;

#define inkling a.f2_inkling		/* Initial number of klingons */
#define inbase a.f2_inbase			/* Initial number of bases */
#define incom a.f2_incom			/* Initian number of commanders */
#define instar a.f2_instar			/* Initial stars */
#define intorps a.f2_intorps		/* Initial/Max torpedoes */
#define condit a.f2_condit			/* Condition (red, yellow, green docked) */
#define torps a.f2_torps			/* number of torpedoes */
#define ship a.f2_ship				/* Ship type -- 'E' is Enterprise */
#define quadx a.f2_quadx			/* where we are */
#define quady a.f2_quady
#define sectx a.f2_sectx			/* where we are */
#define secty a.f2_secty
#define length a.f2_length			/* length of game */
#define skill a.f2_skill			/* skill level */
#define basex a.f2_basex			/* position of base in current quad */
#define basey a.f2_basey
#define klhere a.f2_klhere			/* klingons here */
#define comhere a.f2_comhere		/* commanders here */
#define casual a.f2_casual			/* causalties */
#define nhelp a.f2_nhelp			/* calls for help */
#define nkinks a.f2_nkinks
#define ididit a.f2_ididit			/* Action taken -- allows enemy to attack */
#define gamewon a.f2_gamewon		/* Finished! */
#define alive a.f2_alive			/* We are alive (not killed) */
#define justin a.f2_justin			/* just entered quadrant */
#define alldone a.f2_alldone		/* game is now finished */
#define shldchg a.f2_shldchg		/* shield is changing (affects efficiency) */
#define thingx a.f2_thingx			/* location of strange object in galaxy */
#define thingy a.f2_thingy
#define plnetx a.f2_plnetx			/* location of planet in quadrant */
#define plnety a.f2_plnety
#define inorbit a.f2_inorbit		/* orbiting */
#define landed a.f2_landed			/* party on planet (1), on ship (-1) */
#define iplnet a.f2_iplnet			/* planet # in quadrant */
#define imine a.f2_imine			/* mining */
#define inplan a.f2_inplan			/* initial planets */
#define nenhere a.f2_nenhere		/* Number of enemies in quadrant */
#define ishere a.f2_ishere			/* Super-commander in quandrant */
#define neutz a.f2_neutz			/* Romulan Neutral Zone */
#define irhere a.f2_irhere			/* Romulans in quadrant */
#define icraft a.f2_icraft			/* Kirk in Galileo */
#define ientesc a.f2_ientesc		/* Attempted escape from supercommander */
#define iscraft a.f2_iscraft		/* =1 if craft on ship, -1 if removed from game */
#define isatb a.f2_isatb			/* =1 if SuperCommander is attacking base */
#define iscate a.f2_iscate			/* Super Commander is here */
#ifdef DEBUG
#define idebug a.f2_idebug			/* Debug mode */
#endif
#ifdef CLOAKING
#define iscloaked a.f2_iscloaked  /* Cloaking is enabled */
#define iscloaking a.f2_iscloaking /* However if iscloaking is TRUE then in process of cloaking and can be attacked */
#define ncviol a.f2_ncviol		/* Treaty violations */
#define isviolreported a.f2_isviolreported /* Violation reported by Romulan in quadrant */
#endif
#ifdef CAPTURE
#define kcaptured d.f1_kcaptured   /* number of captured Klingons                   */
#define brigfree d.f1_brigfree     /* room in the brig */
#define brigcapacity a.f2_brigcapacity        /* How many Klingons the brig will hold */
#endif
#define iattak a.f2_iattak			/* attack recursion elimination (was cracks[4]) */
#define icrystl a.f2_icrystl		/* dilithium crystals aboard */
#define tourn a.f2_tourn			/* Tournament number */
#define thawed a.f2_thawed			/* Thawed game */
#define batx a.f2_batx				/* Base coordinates being attacked */
#define baty a.f2_baty
#define ithere a.f2_ithere			/* Tholean is here  */
#define ithx a.f2_ithx				/* coordinates of tholean */
#define ithy a.f2_ithy
#define iseenit a.f2_iseenit		/* Seen base attack report */
#define inresor a.f2_inresor		/* initial resources */
#define intime a.f2_intime			/* initial time */
#define inenrg a.f2_inenrg			/* Initial/Max Energy */
#define inshld a.f2_inshld			/* Initial/Max Shield */
#define inlsr a.f2_inlsr			/* initial life support resources */
#define indate a.f2_indate			/* Initial date */
#define energy a.f2_energy			/* Energy level */
#define shield a.f2_shield			/* Shield level */
#define shldup a.f2_shldup			/* Shields are up */
#define warpfac a.f2_warpfac		/* Warp speed */
#define wfacsq a.f2_wfacsq			/* squared warp factor */
#define lsupres a.f2_lsupres		/* life support reserves */
#define dist a.f2_dist				/* movement distance */
#define direc a.f2_direc			/* movement direction */
#define Time a.f2_Time				/* time taken by current operation */
#define docfac a.f2_docfac			/* repair factor when docking (constant?) */
#define resting a.f2_resting		/* rest time */
#define damfac a.f2_damfac			/* damage factor */
#define stdamtim a.f2_stdamtim		/* time that star chart was damaged */
#define cryprob a.f2_cryprob		/* probability that crystal will work */
#define probex a.f2_probex			/* location of probe */
#define probey a.f2_probey
#define probecx a.f2_probecx		/* current probe quadrant */
#define probecy a.f2_probecy	
#define probeinx a.f2_probeinx		/* Probe x,y increment */
#define probeiny a.f2_probeiny		
#define proben a.f2_proben			/* number of moves for probe */
#define isarmed a.f2_isarmed		/* Probe is armed */
#define nprobes a.f2_nprobes		/* number of probes available */

EXTERN int
		kx[21],			/* enemy sector locations */
		ky[21],
		starch[9][9];	/* star chart */

EXTERN int fromcommandline; /* Game start from command line options */
EXTERN int coordfixed; /* Fix those dumb coordinates.  */

EXTERN char	passwd[10],		/* Self Destruct password */
		*device[ndevice+1];

EXTERN PLANETS nulplanet;	/* zeroed planet structure */

EXTERN double
		kpower[21],		/* enemy energy levels */
		kdist[21],		/* enemy distances */
		kavgd[21],		/* average distances */
		damage[ndevice+1],		/* damage encountered */
		future[NEVENTS+1];		/* future events */

EXTERN int iscore, iskill; /* Common PLAQ */
EXTERN double perdate;

typedef enum {FWON, FDEPLETE, FLIFESUP, FNRG, FBATTLE,
              FNEG3, FNOVA, FSNOVAED, FABANDN, FDILITHIUM,
			  FMATERIALIZE, FPHASER, FLOST, FMINING, FDPLANET,
			  FPNOVA, FSSC, FSTRACTOR, FDRAY, FTRIBBLE,
			  FHOLE
#ifdef CLOAKING
   , FCLOAK
#endif
} FINTYPE ;

/* Skill levels */
typedef enum {SNOVICE=1, SFAIR, SGOOD, SEXPERT, SEMERITUS} SKILLTYPE;

EXTERN double aaitem;
EXTERN char citem[24];


/* Define devices */
#define DSRSENS 1
#define DLRSENS 2
#define DPHASER 3
#define DPHOTON 4
#define DLIFSUP 5
#define DWARPEN 6
#define DIMPULS 7
#define DSHIELD 8
#define DRADIO  9
#define DSHUTTL 10
#define DCOMPTR 11
#define DTRANSP 12
#define DSHCTRL 13
#define DDRAY   14  /* Added deathray */
#define DDSP    15  /* Added deep space probe */
#define DCLOAK  16  /* Added cloaking device */

/* Define future events */
#define FSPY	0	/* Spy event happens always (no future[] entry) */
					/* can cause SC to tractor beam Enterprise */
#define FSNOVA  1   /* Supernova */
#define FTBEAM  2   /* Commander tractor beams Enterprise */
#define FSNAP   3   /* Snapshot for time warp */
#define FBATTAK 4   /* Commander attacks base */
#define FCDBAS  5   /* Commander destroys base */
#define FSCMOVE 6   /* Supercommander moves (might attack base) */
#define FSCDBAS 7   /* Supercommander destroys base */
#define FDSPROB 8   /* Move deep space probe */

#ifdef INCLUDED
PLANETS nulplanet = {0};
char *device[ndevice+1] = {
	"",
	"S. R. Sensors",
	"L. R. Sensors",
	"Phasers",
	"Photon Tubes",
	"Life Support",
	"Warp Engines",
	"Impulse Engines",
	"Shields",
	"Subspace Radio",
	"Shuttle Craft",
	"Computer",
	"Transporter",
	"Shield Control",
	"Death Ray",
	"D. S. Probe"
#ifdef CLOAKING
	,"Cloaking Device"
#endif
};									
#endif

#define ALGERON (2311) /* Date of the Treaty of Algeron */

#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

#define IHR 'R'
#define IHK 'K'
#define IHC 'C'
#define IHS 'S'
#define IHSTAR '*'
#define IHP 'P'
#define IHB 'B'
#define IHBLANK ' '
#define IHDOT '.'
#define IHQUEST '?'
#define IHE 'E'
#define IHF 'F'
#define IHT 'T'
#define IHWEB '#'
#define IHGREEN 'G'
#define IHYELLOW 'Y'
#define IHRED 'R'
#define IHDOCKED 'D'


/* Function prototypes */
void prelim();
void attack();
int choose();
void setup();
void score();
void atover();
void srscan();
void lrscan();
void phasers();
void photon();
void warp();
void sheild();
void dock();
void dreprt();
void chart();
void impuls();
void waiting();
void setwrp();
void events();
void report();
void eta();
void help();
void abandn();
void finish();
void dstrct();
void kaboom();
void freeze();
void thaw();
void plaque();
int scan();
#define IHEOL (0)
#define IHALPHA (1)
#define IHREAL (2)
void chew();
void chew2();
void skip();
void prout();
void proutn();
void stars();
void newqad();
int ja();
void cramen();
void crmshp();
void cramlc();
double expran();
double Rand();
void iran8();
void iran10();
double square();
void dropin();
void newcnd();
void sortkl();
void lmove();
void ram();
void crmena();
void deadkl();
void timwrp();
void movcom();
void torpedo();
void cramf();
void crami();
void huh();
void pause();
void nova();
void snova();
void scom();
void hittem();
void prouts();
int isit();
void preport();
void orbit();
void sensor();
void beam();
void mine();
void usecrystals();
void shuttle();
void deathray();
void debugme();
void attakreport();
void movetho();
void probe();

int min();
int max();
void randomize();
int getch();

#ifdef CLOAKING
void cloak();
#endif
#ifdef CAPTURE
void capture();
#endif

#ifdef CLOAKING
#define REPORTS ((condit==IHDOCKED || damage[DRADIO]<=0.0) && !iscloaked)
#else
#define REPORTS (condit==IHDOCKED || damage[DRADIO]<=0.0)
#endif
