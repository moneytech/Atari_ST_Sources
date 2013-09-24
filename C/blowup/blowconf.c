/*  VIDEOCONFIG for FALCON  
(c) 8.2.1993 Georg Acher
V final (c) 1996 Michael Eberl */ 

/*	�nderungen:
 	210693  game: Richtiger auf Test Treibernr<11 f�r Speedo
 *	280693: ga, me: TV-Modus f�r 16 bis 65535 Farben
 *	280693: me    : Speichern auf C:, wenn mit CAUTO von A: gebootet wurde
 *	290693: ga, me: MultiTOS und NEWDESK.INF-Bug zertreten (in BB)
	??????: ?????? LOCH NESS
	250893: ga: Saver in Farbparameterblock eingeb., reset_formel
	260893: ga: diverse �nderungen f�r Demo
	030993 ga: Doppelklickfehler in form_do weg
	??????
	270993 ga: &&-Fehler in super_reset_pll entdeckt :-(
	280993 ga: Saver nicht bei SM124 aktivierbar
	041093 gaga: Paddle-fine-adjustment
	111093 ga: Pll-Regelung jetzt auch in Blowconf aktiv
	121093 ga: n�chster &&-Fehler in UNDO weg
	161093 ga: paddle-wert f�r SAVE wird evtl. direkt dem Feedback entnommen bei H2_V2
	281093 ga: Abtrennung der Untertassensektion �h.. MGR.C
	281093 ga: Arbeiten am TV-Modus 16/256/TC
	011193 ga: Immer noch TV, aber nun 4&2 Farben
	0x1193 me: Mal wieder CRITICAL repariert
	041193 ga: Configurations-scanner f�r TV
	101193 ga: Erkennung von H2_V3
	141193 ga: Einladen der Werte aus BLOWBOOT repariert (�hem...)
	1?1193 mega: Quickswitch
	241193 mega: Flickerfree Compatibilty
	011293 game: Flydial
V 1.7
	061293 mega: Flydialfehler weg (Klaus: Alles Gute zum Namenstag!)
	121293 ga: TC-TV-bug (mcreg) behoben/Flydial auch nach oben verschiebbar
	161293 game: Abspeichern/Laden von eigenen Konfs
	211293 ga: REMOVE-Button mit Sicherheitskopie
	231293 ga: Thorsten G�nther Effekt behoben bei Hard2 Erkennung
	311293 ga: Configscanner fuer Standard/eigene BCFs verbessert (jaja...)
	311293 ga: Bei TV Blank-R�nder jetzt auch alle richtig gesetzt
	010194 ga: Bei TV noch ein paar Sch�nheitsfehler beseitigt (hicks...)
	030194 ga: move_dialog: mehr Zeit f�r Redraw der anderen Fenster
	110194 ga: acc: Jetzt wird beim Sm124 wirklich SAVER disabled
	04/140194 me/ga: VSCR-Cookie implementiert
	140194 ga: Scroll-Rand bei vscr. etwas gr��er
	??0294 ga: Pixeldoub ein und wieder ausgebaut...
	totale Zufriedenheit :-)
	140594 ga: TC-Bugs (charlinewidth/wei�e Buttons) weg. (BB/BC)
	140594 ga: Zur�cksetzen der Bank2 beim Speichern behoben (ach, was bin ich gut...)
	und dann wollte Ewald Version 2.0 ....
	040994 ga: Anzeige Real/Virtuell
	040994 ga: ACC: SAVE saved SAVER (noch nicht ganz :-[]
	040994 ga: Eigenbaukonfs haben jetzt ein '' am Ende
	091094 ga: T_WORD/SPEEDO-Bug endlich weg (halleluja!) (BB)
	121094 ga: Screensaver/Hard2/Paddle-Problem weg (PLL)
	141094 ga: Nach saverdeactivate PLL nochmal auf Volldampf stellen(BB)
	141094 ga: quickswitch merkt sich paddle-wert der Hard2(II?)(QK)
	141094 ga: Erkennung der Bank1/Bank2, Radiobuttons
	141094 ga: Load-conf-box verschiebbar
	141094 ga: wei�er Rand bei (links+rechts)%4=2 weg
	231094 ga: SM124: Wunderflag wurde f�lschlicherweise gesetzt->weg
	231094 ga: Hsync mit Shift �nderbar/Popup f�r 32 MHz
	231094 ga: Accessory-Save saved Saver jetzt sicher ab!
	051194 ga: Saver reagiert nicht mehr auf MIDI-Ints
	061194 ga: Shift/alternate beim aufl�sungswechsel, 'n' beim booten
	071194 ga: Critical bei Hard1/25/32MHz
	071194 ga: Options (FFC/Critical) extra
	081194 ga: Accessory-Save saved Saver jetzt gaaanz sicher ab!
	101194 me: Von wegen!
V final (me)
	210996 me: Auf geht's.
			   - Abbruch mit Ctrl-Q.
			   - FFC auch f�r nichtkonfigurierte
			     Nichtkompatibilit�tsmodi.
			   - Accesory Save saved den Saver.
			   - Cheat-Modus, Teil 1
	270996 me: - Cheat-Modus, Teil 2
			   - Erkennung der Landessprache und laden des
			     entsprechenden RSC-Files:
			     	BLOWCONF.RSC f�r Deutschland
			     	BLOWFRA.RSC  f�r Frankreich
			     	BLOWGB.RSC	 f�r Englang/Amerika
			   - FX-Takterkennung
	040197 ga: Bei SM124/TV sollte man keine MS-Daten einstellen k�nnen
	040197 ga: Etwas gro�z�gigere Frequenzbereiche TV/SM
 */

#pragma warn -use
#pragma warn -aus
#pragma warn -par

#include <stdio.h>
#include <stdlib.h>
#include <aes.h>
#include <vdi.h>
#include <tos.h>
#include <math.h>
#include <string.h>
#include "blowconf.h" 

typedef struct
{
	int 	Version;
	int 	FXconfig;
	long	memstart;
	long	memlen;
	long	(*page_fn)();
	long	(*rpage_fn)();
	long	(*speeder_fn)();
	long	(*rspeeder_fn)();
	long	start_bits;
	long 	speed_tab;
	long	res3;
	long 	res4;
	int		mem_add;
	long 	ems_start;
	long	ems_len;
	int		ems_add;
	int		start_speed;
	int		speeder_conf;
	int 	page0;
	int		page1;
	char	confpath[60];
} FXParblk;
int fx_systemclock;
int vfreq_min=60, vfreq_max=60;
int hfreq_min=314, hfreq_max=315;
int lastmon;

#define Vsetmode(a) (int)xbios(88,(int)a)
#define  regaus printf("\033H%x %x %x %x %x %x  \n\n",vcreg[0],vcreg[1],vcreg[2],\
vcreg[3],vcreg[4],vcreg[5]); \
printf("%x %x %x %x %x %x     cr:%x ",xcreg[0],xcreg[1],xcreg[2],\
xcreg[3],xcreg[4],xcreg[5],ccreg[0]);\
 printf("%x %x %x %x %x %x         ",vcreg1[0],vcreg1[1],vcreg[2],\
vcreg1[3],vcreg1[4],vcreg1[5]); \
printf("%x %x %x %x %x %x %x ",xcreg1[0],xcreg1[1],xcreg1[2],\
xcreg1[3],xcreg1[4],xcreg1[5],ccreg1[0]);

#ifndef VERSION 
#define VERSION 1
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
#endif

#define PLL_VERSION 2
#define XTAL_VERSION 1
#define SOFT_VERSION 0

#ifndef FRANCAISE
#define FRANCAISE 0
#endif

#define OLD_HARD2 1

#define XTAL_FREQU 50.0

#define TEST 0

#define DEMO_VERSION 0

#define XSCRFAC 8
#define YSCRFAC 8

#define PLL_DIV 1

#define trxres(a) ((a-320)/16)
#define tryres(a) ((a-200)/8)
#define min(a,b) (a>b?b:a)
#define max(a,b) (b>a?b:a)
#define dabs(a) 	 (((a)<0.0)?(-a):(a))


/*
4*((int)(clock_Mhz)%4))) 
*/
#define pll_calc(clock_Mhz,ww,hh)\
(int)((int)(7.0*38400.0/(clock_Mhz*1e6/((double)(ww)*(double)(hh)))))

int ap_id;
int w_id=-1;
double clock_tab[3]={25.17,32.08,XTAL_FREQU};
/* Wert wird sp�ter verstellt    ^^^^ */
int clock=0;
double clock_Mhz=XTAL_FREQU;

double hfrequ,vfrequ;
int xst,yst,ww=800,hh=524,x_off=80,y_off=32;
int old_ww,old_hh;
int xres=640,yres=480,interlace=0,tc=0,tc_sel=0;
int vsync=2,hsync=40;
int u1=0,u2=0;
int pix_doub=0;
int pl_idx,x_start;
int vdi_x,vdi_y;
int vscreen_flag=0;
int monitor_type;
int undo_flag=0,sledgehammer=-1;
int allow_hsync=0;
int ffc_on=0,use_critical=0;

char *boot_prg_name="C:\\AUTO\\BLOWBOOT.PRG";
char tmp[20];
OBJECT *tree1,*tree2,*tree3,*tree4,*tree5;
int cx,cy,cw,ch;
int mcx,mcy,mcw,mch;
int last_slider=-1;
#define slider_count 10
int slider_tab[slider_count]=
{XMINUS,YMINUS,FSMINUS,FEMINUS,VMINUS,
LSMINUS,LEMINUS,HMINUS,CLOCKMIN,CNFMINUS};
int last_x,last_y;

int systemclockindex=0;
MENU my_clk_menu={(OBJECT*)0L,0,0,1,0};
double systemclocklist[9]={32.0,33.0,34.0,36.0,38.0,40.0,44.0,48.0,50.0};



typedef int vcregtype[6];

/* vreg : vertical control ,vcreg1 urspr�ngliche Werte f�r undo */
vcregtype vcreg,vcreg1={0x419,0x3ff,0x3f,0x3f,0x3ff,0x415};
/* ccreg: controller control ... */
vcregtype ccreg,ccreg1={0,0};
/* xcreg: x(horizontal) control */
vcregtype xcreg,xcreg1={0xc6,0x8d,0x15,0x2a3,0x7c,0x96};
/* mcreg: memory control */
vcregtype mcreg,mcreg1;

int coltable[5]={16,8,4,2,1};/* PIXEL/WORD */
int planetab[5]={1,2,4,8,16};
/* Erfahrungswerte ... */
#if !DEMO_VERSION
int x88[5]={0x273,0x20e,0x2a3,0x2ab,0x2ac};
int x8a[5]={0x50,0xd,0x7c,0x84,0x91};

int x88_tv[5]={0x3ef,0x2,0x4d,0x5d,0x71};
int x8a_tv[5]={0xa0,0x20,0xfe,0x10e,0x122};

#else
int x88[5]={0,0,0x2a3,0,0};
int x8a[5]={0,0,0x7c,0,0};
#endif

typedef struct
 {
char monitor[12];
int xres,yres;
int interlace;
int tc;
int clock;
double clock_Mhz;
int width,leb,rib,hsync;
int height,upb,lob,vsync;
int flags;
int own;
} config_type;

typedef struct
{
	long	value;
	void	*param;
} cookie;

config_type configuration[256];
int config_nr=0,config_pos=0,config_sel;
int handle,n;

void schreib_regs(vcregtype vreg,vcregtype creg,
	vcregtype xreg,vcregtype mreg);
void quit_all(void); void init(void); void undo(int);
void set_vcdata(int cholera, int cholerb);
void change_res(int parent,int type, int res1, int res2,int offset);
void set_clock(int wert);
void change_rsc(void);
int save(void);
void read_regs();
/* in MGR.C */
extern int alert(int);
extern void show_dialog(int nr, int start,int how,int center_ob);
extern void find_slider(int nr,int* last_slider);
extern int find_object( OBJECT *tree, int start_object, int which );
extern int ini_field( OBJECT *tree, int start_field );
extern void reset_key_buffer();
extern int do_dialog( int nr, int start );
extern void move_box();
extern void clean_up(void);
extern int handle_slider(int pd_ob, OBJECT *tree,
		int parent, int child, int xplus, int xminus,
		int yplus,int yminus, int xss_add,int xms_add,
		int yss_add, int yms_add, int flag,int *xl, int *yl);

extern void load_configuration_file();
extern void save_own_conf(char* name);
extern void load_own_conf(char* name);
extern void do_save_conf();

int get_color(void);
int get_clock(void);
int check_monitor(void);
void load_configuration(void);
void super_reset_pll();

void install_vbl();void de_install_vbl();void reset_pll(int,int,int);
int paddle_adjust(int,int);
volatile int pll_on=0,pll_mode=0;
volatile extern int c_frequ,pll_lc_flag,pll_lc,l_delta;
volatile extern own_vbl_count;
int old_c_frequ,reset_wert;
double old_clock=33.0;
double old_cm;
int pll_preset_tab[8]={98,107,120,128,140,148,148};
int pll_paddle_tab[8]={125,149,170,188,207,212,212,212};
int h2_v3=-1; 

int zweite_bank=0;
int cur_col_res[8]={0,0,0,0,0,0,0,0};
 
#if VERSION==SOFT_VERSION
int version=0;
#else
#if VERSION==PLL_VERSION
int version=2;
#else
int version=1;
#endif
#endif

int reset_formel(double clo)
{
double diff;
int a1,a2;
a1=(clo-32)/4;
a2=1+(clo-32)/4;
diff=clo-4*((int)clo/4);
#if !OLD_HARD2
return 0.9*(pll_preset_tab[a1]+(pll_preset_tab[a2]-pll_preset_tab[a1])*diff/4);
#else
return 0.26*(pll_preset_tab[a1]+(pll_preset_tab[a2]-pll_preset_tab[a1])*diff/4);
#endif
}
/*---------------------------------------------------*/
int paddle_formel(double clo)
{
double diff;
int a1,a2;
a1=(clo-32)/4;
a2=1+(clo-32)/4;
diff=clo-4*((int)clo/4);

if (h2_v3!=1)
	return 0.9*(pll_paddle_tab[a1]+(pll_paddle_tab[a2]-pll_paddle_tab[a1])*diff/4);
else
	return 0.9*(pll_paddle_tab[a1]+(pll_paddle_tab[a2]-pll_paddle_tab[a1])*diff/4)-27;	
}
/*---------------------------------------------------*/
void* get_tabele()
{
long old_stack;
cookie *keks;

old_stack=Super(0L);	/* Cookie holen im Supervisormode */
keks =(cookie*)(*(long*)0x5a0);
while ((keks->value!='BLOW') && (keks->value!=0))
{
/*	char str[6];
	
	memcpy(str,&(keks->value),4);
	str[4]=0;
	printf("%s \n",str);*/ 
	keks++;
}
if (keks->value=='BLOW')
{
	char *adr;
	
/*	puts("BlowBoot found!");*/
	adr = (char*)keks->param;
	Super((void *) old_stack); 
	return adr;
}
Super((void *) old_stack); 
return 0L;
}
/*---------------------------------------------------*/
void quit_all()
{
wind_update(END_MCTRL);
wind_update(END_UPDATE);
(void)rsrc_free();
(void)appl_exit();
exit(0);
}
/*---------------------------------------------------*/
long get_cookie( long );
void init()
{
	int work_in[11];
	int work_out[58];
	int dummy,bootdev;
	long ldummy;
	int language=0;
	long old_stack;
	char ausnahme[30]="A:\\AUTO\\CAUTO.PRG";
	
	ap_id=appl_init();


	if (ap_id==-1) {puts("ERROR: ap_id=-1 !");exit(1);}

	language=(get_cookie((long)'_AKP')&0xff00)>>8;
	if ((language==1)) {
		/* Man spricht Deutsh */

		if (rsrc_load("BLOWCONF.RSC")==0)
		{
			(void)form_alert(1,"[3][Resource-Datei|BLOWCONF.RSC fehlt!][  OK  ]" );
			quit_all();
		}
	} else if (language==2) {
		if (rsrc_load("BLOWFRA.RSC")==0)
		{
			(void)form_alert(1,"[3][ BLOWFRA.RSC | introuvable !][  OK  ]"); 
			quit_all();
		}
	} else{
		if (rsrc_load("BLOWGB.RSC")==0)
		{
			(void)form_alert(1,"[3][  No Resource  |BLOWGB.RSC missing][  OK  ]");
			quit_all();
		}
	}
	
#if DEMO_VERSION
	alert(ERROR8);
	form_alert(0,"[1][BlowUP030 for the ATARI-Falcon|         (c) by|       Georg Acher|            &|\
      Michael Eberl][  OK  ]");
#endif	
	handle=graf_handle(&dummy,&dummy,&dummy,&dummy);
	for (dummy=0;dummy<=9;dummy++) work_in[dummy]=1;
	work_in[10]=2;
	v_opnvwk(work_in,  &handle, work_out );
	vdi_x=work_out[0];vdi_y=work_out[1];
	v_clsvwk(handle);
	handle=graf_handle(&dummy,&dummy,&dummy,&dummy);
	for(dummy=0;(dummy<5)&&(_GemParBlk.global[10]!=planetab[dummy]);dummy++);
#if DEMO_VERSION
	if (dummy!=2) {alert(ERROR4);exit(0);}
#endif
	pl_idx=dummy;
	wind_update(BEG_UPDATE);
	wind_update(BEG_MCTRL);
	(void)graf_mouse(ARROW,0);
	old_stack=Super(0L);
	bootdev=*((int*)0x446);		/* Bootdevice */
	boot_prg_name[0]=(char)(bootdev+65);
	ausnahme[0]=(char)(bootdev+65);
	Super((void *) old_stack);
	if (bootdev==0)	/* Wenn von A: gebootet ... */
	{
		ldummy=Fopen( ausnahme, FO_READ ); /* Existiert ein CAUTO-Programm? */
		if (ldummy>=0)	/* Ja? Dann ist das Bootlaufwerk nicht das Bootlaufwerk */
		{
			bootdev=2;	/* Laufwerk C ist richtig! */
			boot_prg_name[0]=(char)(bootdev+65);	/* Name korrigieren */
			(void)Fclose( (int)ldummy );
		}
	}
#if DEMO_VERSION
	if (pl_idx!=2) {alert(ERROR4);exit(0);}
#endif
}
/*---------------------------------------------------*/
void load_monitor_definition()
{
	FILE *file;
	file=fopen("MONITOR.DEF","r");
	if (NULL!=file) {
		int type;
		(void)fscanf(file,"%4i",&type);
		if (type==lastmon)
			(void)fscanf(file,"%4i %4i %4i %4i", &hfreq_min,
				&hfreq_max, &vfreq_min, &vfreq_max );
		fclose(file);
	}
}
/*---------------------------------------------------*/
void save_monitor_definition()
{
	FILE *file;
	file=fopen("MONITOR.DEF","w");
	if (NULL!=file) {
		(void)fprintf(file,"%4i\n",lastmon);
		(void)fprintf(file,"%4i %4i %4i %4i\n", hfreq_min,
			hfreq_max, vfreq_min, vfreq_max );
		fclose(file);
	}
}
/*---------------------------------------------------*/
void load_configuration_file()
{
FILE *file;
int f_id=0;
config_type *cnf;
void *adr;
int anz,conf_ok=0;

config_nr=0;
load_own_conf("DEFAULT.BCF");


file=fopen("STANDARD.BCF","r");
if (file==0) 
{
	alert(ERROR1);
	tree1[LOAD].ob_state|=DISABLED;return;
}
do
{
	cnf=&configuration[config_nr];
	cnf->clock=0;
	anz=fscanf(file,"%s %i %i %i %i %i %i %i %i %i %i %i %i %i %lf",
	&cnf->monitor,&cnf->xres,&cnf->yres,&cnf->interlace,&cnf->clock,
	&cnf->width,&cnf->leb,&cnf->rib,&cnf->hsync,
	&cnf->height,&cnf->upb,&cnf->lob,&cnf->vsync,&cnf->flags,
	&cnf->clock_Mhz);
	cnf->own=0;
if (monitor_type==0)
{	if (!strncmp(cnf->monitor,"SM",2)&&
		((VERSION==2)||((cnf->clock_Mhz==32))))
 		conf_ok=1;
}
else
{
/*#if VERSION==SOFT_VERSION
	if (cnf->clock!=2)
				conf_ok=1;
#endif*/
if ((monitor_type==2)&&strncmp(cnf->monitor,"TV",2))
{
	#if VERSION==SOFT_VERSION
		if ((cnf->clock!=2))
			conf_ok=1;
	#endif
	
	
	#if VERSION==XTAL_VERSION
		if ((cnf->clock!=2)||(((cnf->clock==2)&
		(!strncmp(cnf->monitor,"MS",2)))||(cnf->clock_Mhz==XTAL_FREQU)))
					 {if (cnf->clock==2) cnf->clock_Mhz=XTAL_FREQU;
					 	conf_ok=1;}
	#endif
	
	#if VERSION==PLL_VERSION
		conf_ok=1;
	#endif
}
else
{ /* TV */
	if (((monitor_type==1)||(monitor_type==3)) 
			&&(!strncmp(cnf->monitor,"TV",2))) 
		conf_ok=1;
}
}
if ((anz==15)&&(conf_ok)) config_nr++;
conf_ok=0;
} while(anz==15);

fclose(file);
}
/*---------------------------------------------------*/
void load_configuration()
{
FILE *file;
int f_id=0;
config_type *cnf;
char *boot_mem,*boot_mem1;
void *adr;
int anz,x,y,vx,vy;

load_configuration_file();

boot_mem=get_tabele();

if (boot_mem==0)
{
	f_id=(int)Fopen(boot_prg_name,FO_READ);
	if (f_id<0) return;
	boot_mem1=(char *)malloc(4096);
	if (boot_mem1==0) {Fclose(f_id);return;}
	(void)Fread(f_id,4096L,boot_mem1);
	Fclose(f_id);
	boot_mem=boot_mem1+32;
}	
	h2_v3=*(int*)(boot_mem+2614+166-32);

	for(anz=0;anz<=4;anz++)
	{
	if (*(int*)(boot_mem+anz*256)!=2)
	{
		vx=	1+*(int*)(boot_mem+64+anz*256);
		vy= 1+*(int*)(boot_mem+66+anz*256);
		x=  *(int*)(boot_mem+64+anz*256+0x1c);
		y=  *(int*)(boot_mem+64+anz*256+0x1e);
		if (anz==pl_idx)
		{
			cur_col_res[0]=vx;cur_col_res[1]=vy;
			cur_col_res[2]=x;cur_col_res[3]=y;
		}
		if (anz!=pl_idx)
			sprintf(tree1[COL2RES+anz].ob_spec.tedinfo->te_ptext,
			"%i*%i",vx,vy);
		else
			if (sledgehammer==-1)
				sprintf(tree1[COL2RES+anz].ob_spec.tedinfo->te_ptext,
				"%i*%i",vx,vy);
			else
				sprintf(tree1[COL2RES+anz].ob_spec.tedinfo->te_ptext,
				"%i*%i",1+sledgehammer,vy);
		if (*(int*)(boot_mem+64+anz*256+0x1a))
			sprintf(tree1[COL2VR+anz].ob_spec.tedinfo->te_ptext,
			"(%i*%i)",x,y);
		else
		{
			sprintf(tree1[COL2VR+anz].ob_spec.tedinfo->te_ptext,
			"         ");	
		}
	}
	else
			sprintf(tree1[COL2VR+anz].ob_spec.tedinfo->te_ptext,
			"         ");
	}
	for(anz=0;anz<=4;anz++)
		{
		if (*(int*)(boot_mem+anz*256+5*256)!=2)
			{
			vx=	1+*(int*)(boot_mem+64+anz*256+5*256);
			vy= 1+*(int*)(boot_mem+66+anz*256+5*256);
			x=  *(int*)(boot_mem+64+anz*256+5*256+0x1c);
			y=  *(int*)(boot_mem+64+anz*256+5*256+0x1e);
			if (anz==pl_idx)
			{
				cur_col_res[4]=vx;cur_col_res[5]=vy;
				cur_col_res[6]=x;cur_col_res[7]=y;
			}
			sprintf(tree1[CO2RES2+anz].ob_spec.tedinfo->te_ptext,
			"%i*%i",vx,vy);
			if (*(int*)(boot_mem+64+anz*256+0x1a+5*256))
				sprintf(tree1[COL2VR2+anz].ob_spec.tedinfo->te_ptext,
				"(%i*%i)",x,y);
			else
			sprintf(tree1[COL2VR2+anz].ob_spec.tedinfo->te_ptext,
			"         ");
			}
			else
			sprintf(tree1[COL2VR2+anz].ob_spec.tedinfo->te_ptext,
			"         ");
		}
		
	if (*(int*)(boot_mem+2608-32))
		{
		long satime;
		satime=*(long*)(boot_mem+2604-32);
		tree1[SAVER].ob_state|=SELECTED;
		sprintf(tree1[SAVTIME].ob_spec.tedinfo->te_ptext,
				"%03li",satime/2000L);
		}
#if (!DEMO_VERSION)
	ffc_on=0;
	if (42==*(int*)(boot_mem+2614+170-32))
		ffc_on=SELECTED;
#endif

		
if (f_id!=0)
	{
	Fclose(f_id);
	free(boot_mem);
	}
}
/*---------------------------------------------------*/

/*---------------------------------------------------*/	
void clip_slider_x(OBJECT *tree,int parent, int child, int value)
{
tree[child].ob_x=max(min(value,tree[parent].ob_width-tree[child].ob_width),0);
}
/*---------------------------------------------------*/	
void calc_paras(int ww,int hh, int xres, int yres)
{
hfrequ=(clock_tab[clock]*1e6/(double)(ww))/
	(1+((monitor_type==1)||(monitor_type==3)));
vfrequ=hfrequ/(double)(hh);
if ((clock==2)&&(vfrequ<40.0)) 
		{alert(ERROR10);undo(0);return;}
}
/*---------------------------------------------------*/	
void insert_number(OBJECT *tree,int child,char *form, int number)
{
sprintf(tmp,form,number);
		strcpy(tree[child].ob_spec.tedinfo->te_ptext,tmp);
}
/*---------------------------------------------------*/	
void insert_float(OBJECT *tree,int child,char *form, double number)
{
sprintf(tmp,form,number);
		strcpy(tree[child].ob_spec.tedinfo->te_ptext,tmp);
}
/*---------------------------------------------------*/	
int get_color()
{
int n;
#if DEMO_VERSION
return 2;
#else
for (n=COL2;n<=COL32,(tree1[n].ob_state&1)!=1;n++);
return (n-COL2);
#endif
}
/*---------------------------------------------------*/	 
void format_configs(int pos)
{
double hf=0,vf=0;
int n,m;
char interl,cf[6];
for(n=0;n<8;n++)
{
	if ((pos+n)<(max(1,config_nr)))
	{
	hf=configuration[pos+n].clock_Mhz*1e6/configuration[pos+n].width;
	vf=hf/configuration[pos+n].height;
	if ((monitor_type==1)||(monitor_type==3)) {hf/=2;vf/=2;}
	if (configuration[pos+n].interlace) 
		interl='i'; 
	else 
		interl=' ';
	for (m=0;m<5;m++) 
		if (configuration[pos+n].flags&(1<<m)) 
			cf[m]=8; /* H�ckchen */
		else 
			cf[m]=32;
	cf[5]=((configuration[pos+n].own||(configuration[pos+n].flags&128))?127:' ');
	if (hf>40000) *(configuration[pos+n].monitor+2)='2';
	sprintf(tree3[CNFTY1+n].ob_spec.tedinfo->te_ptext,
	"%4i*%4i%c  %3.0f    %3.1f %12s  %c %c  %c  %c  %c  %c",
	configuration[pos+n].xres/(1+((configuration[pos+n].flags==16)&&(monitor_type==2))),
	configuration[pos+n].yres,interl,vf,hf/1000.0,
	configuration[pos+n].monitor,cf[0],cf[1],cf[2],cf[3],cf[4],cf[5]); 	
	}
	else
		strcpy(tree3[CNFTY1+n].ob_spec.tedinfo->te_ptext,"");
}
tree3[CNFTYPE].ob_y=tree3[CNFSLIDE].ob_height*pos/(max(1,config_nr));
tree3[CNFTYPE].ob_height=
min(8*tree3[CNFSLIDE].ob_height/(max(1,config_nr)),
tree3[CNFSLIDE].ob_height);
}
/*---------------------------------------------------*/	
void change_rsc()
{
		clip_slider_x(tree1,XSLIDER,XRES,trxres(xres));
		insert_number(tree1,XRES,"%i",xres/(tc_sel+1));
		
		clip_slider_x(tree1,YSLIDER,YRES,tryres(yres));
		insert_number(tree1,YRES,"%i",yres*(interlace+1));
		
		clip_slider_x(tree1,FSSLIDER,FS,y_off);
		clip_slider_x(tree1,FESLIDER,FE,hh-y_off-yres);
		clip_slider_x(tree1,VSLIDER,VSYNC,2*vsync);
		clip_slider_x(tree1,CLOCKSLI,CLOCKS,
		(int)((clock_Mhz-1.0)*4.0-128.0));
		insert_number(tree1,FS,"%i",y_off);
		insert_number(tree1,FE,"%i",hh-y_off-yres);
		insert_number(tree1,VSYNC,"%i",vsync);
		
		clip_slider_x(tree1,LSSLIDER,LS,u1/2);
		clip_slider_x(tree1,LESLIDER,LE,u2/2);
		clip_slider_x(tree1,HSLIDER,HSYNC,(hsync)>>2);		
		insert_number(tree1,LS,"%i",u1);	
		insert_number(tree1,LE,"%i",u2);
		insert_number(tree1,HSYNC,"%i",hsync);
		
		insert_number(tree1,WIDTH,"W: %i",ww);
		insert_number(tree1,HEIGHT,"H: %i",hh);
		
		insert_float(tree1,HFREQU,"H: %.1fkHz",hfrequ/1000);
		insert_float(tree1,VFREQU,"V: %.1fHz",vfrequ);
		if (((hfrequ/100)>hfreq_max) ||
			((hfrequ/100)<hfreq_min)) {
			volatile bfobspec spec;
			volatile long *s;
			s=(long*)&spec;
			*s=(tree1[HFREQU].ob_spec.tedinfo->te_color);
			spec.textcol=RED;
			tree1[HFREQU].ob_spec.tedinfo->te_color=*s;
		} else {
			volatile bfobspec spec;
			volatile long *s;
			s=(long*)&spec;
			*s=(tree1[HFREQU].ob_spec.tedinfo->te_color);
			spec.textcol=BLACK;
			tree1[HFREQU].ob_spec.tedinfo->te_color=*s;
		}
		if (((vfrequ)>vfreq_max) ||
			((hfrequ)<vfreq_min)) {
			volatile bfobspec spec;
			volatile long *s;
			s=(long*)&spec;
			*s=(tree1[VFREQU].ob_spec.tedinfo->te_color);
			spec.textcol=RED;
			tree1[VFREQU].ob_spec.tedinfo->te_color=*s;
		} else {
			volatile bfobspec spec;
			volatile long *s;
			s=(long*)&spec;
			*s=(tree1[VFREQU].ob_spec.tedinfo->te_color);
			spec.textcol=BLACK;
			tree1[VFREQU].ob_spec.tedinfo->te_color=*s;
		}
#if 0
		if (((hfrequ/100)>hfreq_max) ||
			((hfrequ/100)<hfreq_min))
			tree1[HFREQU].ob_spec.tedinfo->te_color=1;
		else
			tree1[HFREQU].ob_spec.tedinfo->te_color=0;
		if (((vfrequ)>vfreq_max) ||
			((hfrequ)<vfreq_min))
			tree1[VFREQU].ob_spec.tedinfo->te_color=1;
		else
			tree1[VFREQU].ob_spec.tedinfo->te_color=0;
#endif
#if !FRANCAISE
		if (interlace) 
			insert_number(tree1,INTERLAC,"INTERLACE",0);
		else
			insert_number(tree1,INTERLAC,"NONINTERL",0);
#else
		if (interlace) 
			insert_number(tree1,INTERLAC,"ENTRELAC\220",0);
		else
			insert_number(tree1,INTERLAC,"NONENTREL",0);
#endif
		if (clock!=2)			
			insert_number(tree1,CLOCK,"CLK %iMHz",clock_tab[clock]);
		else
			insert_number(tree1,CLOCK,"EXT CLK",0);
		
		insert_float(tree1,CLOCKS,"%.1lf",clock_Mhz);
#if VERSION==PLL_VERSION
		if (clock!=2)
			tree1[CLOCKS].ob_state|=DISABLED;
		else
			tree1[CLOCKS].ob_state&=~DISABLED;
#endif
}
/*---------------------------------------------------*/	
/* how=0: kein Schreiben in Register, 1: Normal, 
	   2:Register,aber kein draw*/

void update(int how) 
{
calc_paras(ww,hh,xres,yres);
		change_rsc();
		set_vcdata(pl_idx,1);
if (how) 	{
		schreib_regs(vcreg,ccreg,xcreg,mcreg);
#if VERSION==PLL_VERSION
		if (tree1[MONM2].ob_state&SELECTED)
		{
			old_ww=ww;old_hh=hh;
			c_frequ=pll_calc(clock_Mhz,old_ww,old_hh);
			l_delta=200;
			pll_lc=0;
			own_vbl_count=255-10;
		}
#endif
		}
if (how<2){		
/*R		objc_draw(tree1,ROOTSCR,10,cx,cy,cw,ch);*/
		objc_draw(tree1,HFREQU,10,cx,cy,cw,ch);
		objc_draw(tree1,VFREQU,10,cx,cy,cw,ch);
		objc_draw(tree1,FESLIDER,10,cx,cy,cw,ch);
		objc_draw(tree1,LESLIDER,10,cx,cy,cw,ch);
		objc_draw(tree1,WIDTH,10,cx,cy,cw,ch);
		objc_draw(tree1,HEIGHT,10,cx,cy,cw,ch);
		}
}
/*---------------------------------------------------*/	
/* undo berechnet nebenbei aus den HW-Registern alle Parameter */
void undo(int how)
{
int u1u2;
ww=800;hh=524;x_off=80;y_off=32;
xres=640; yres=480;clock=1;interlace=0;tc=0;vsync=2;

interlace=(ccreg1[1]&2)>>1;
if (interlace<0) interlace=0;
if (interlace>0) interlace=1;

if ((monitor_type==2)&&(ccreg1[1]&4)) tc=1; else tc=0;

if (ccreg1[0]&4) clock=0; else clock=1;
if ((mcreg1[0]&0x100)&&(clock==1))
#if DEMO_VERSION
			{
			long adr;
			adr=(char*)get_tabele();
			if ((adr!=0)&&(sledgehammer!=-1)) *(int*)(adr+pl_idx*256+0x40)=sledgehammer;
			alert(ERROR18);quit_all();
			}
#else
	#if VERSION==SOFT_VERSION
		if ((monitor_type==2))
			{
			long adr;
			adr=(char*)get_tabele();
			if ((adr!=0)&&(sledgehammer!=-1)) *(int*)(adr+pl_idx*256+0x40)=sledgehammer;
			alert(ERROR18);quit_all();
			}
			
		clock_Mhz=32.0;	/* F�r Genlock */
		clock_tab[2]=32.0;
		if ((monitor_type==1)||(monitor_type==3))
			clock=2;
	#else 
		clock=2;
	#endif
		{
			FXParblk *fx_cookie;
			if (0!=(fx_cookie=get_cookie((long)'BPFX'))) {
				int spd_c;
				char s[20];
				
				spd_c=fx_cookie->rspeeder_fn();
				fx_systemclock=(spd_c>>11)&3;
				if (1==fx_systemclock)
					systemclockindex=0;
				else if (2==fx_systemclock)
					systemclockindex=3;
				else if (3==fx_systemclock)
					systemclockindex=5;
				clock_tab[1]=systemclocklist[systemclockindex];
			}
			else
				fx_systemclock=0;
		}
#endif
if (monitor_type==2)
{
hh=(vcreg1[0])/2;
y_off=(vcreg1[3])/2;
yres=(vcreg1[4]-vcreg1[3])/2;
vsync=(vcreg1[0]-vcreg1[5]+interlace)/2;


	if (pl_idx!=1)
	{
	/* mono,16,256,TC */
/*	xres=xcreg1[0]*4-152;*/
	xres=mcreg1[3]*coltable[pl_idx];
	ww=xcreg1[0]*4+8;
	hsync=xcreg1[5];
	u1u2=4*xcreg1[0]-xres*(tc+1)+8;
	u1=42+2*xcreg1[3]-2*((u1u2-160)/4+x88[pl_idx]+((xres*(1+tc)-640)>>2));
	u2=u1u2-u1;
	
	if (!(ccreg1[0]&0x100))
		{
		u1=u1+64;u2=u2-64;
		}
	}
#if !DEMO_VERSION
	else
	{/* 4 */
	/*xres=xcreg1[0]*32-96;*/
	xres=mcreg1[3]*coltable[pl_idx];
	ww=xcreg1[0]*32+64;
	hsync=xcreg1[5]*8;
	u2=-(long)32L*(long)xcreg1[4]+(long)32L*(long)x8a[pl_idx]+(long)xres-(long)490;
	u1=-(xres+32L*(long)x88[pl_idx]-32L*(long)xcreg1[3]-810+u2)>>1;
	
	}
}
else
if (monitor_type==0)
	{ /* SM124 */
	hh=(vcreg1[0])/2;
	y_off=(vcreg1[3])/2;
	yres=(vcreg1[4]-vcreg1[3])/2;
	vsync=(vcreg1[0]-vcreg1[5]+interlace)/2;
	

	xres=(2+xcreg1[0])*32-256;
	ww=(2+xcreg1[0])*32;
	hsync=ww-xcreg1[5]*32;
	u1=128;
	u2=128;
	}
else
	{/* TV */
		hh=(vcreg1[0])/2;
		y_off=(vcreg1[3])/2;
		yres=(vcreg1[4]-vcreg1[3])/2;
		vsync=(vcreg1[0]-vcreg1[5]+interlace)/2;
	
		if (pl_idx!=1)
		{
		/* TV mono,16,256,TC */
	/*	xres=xcreg1[0]*4-152;*/
		xres=mcreg1[3]*coltable[pl_idx];
		ww=xcreg1[0]*2+4;
		hsync=xcreg1[5]/2;
		u1=xcreg1[3]+160-x88_tv[pl_idx]-((xres-640)>>2);
		u2=ww-xres-u1;
/*		u1u2=4*xcreg1[0]-xres*(tc+1)+8;
		u1=42+2*xcreg1[3]-2*((u1u2-160)/4+x88[pl_idx]+((xres*(1+tc)-640)>>2));
		u2=u1u2-u1;
*/		
		if (!(ccreg1[0]&0x100))
			{
			u1=u1+64;u2=u2-64;
			}
		}

		else
		{/* TV 4 */
		/*xres=xcreg1[0]*32-96;*/
		xres=mcreg1[3]*coltable[pl_idx];
		ww=xcreg1[0]*16+32;
		hsync=xcreg1[5]*4;
		u1=4*(x8a_tv[pl_idx]-xcreg1[4])+160;
		u2=ww-xres-u1;
		}
#endif
	}
if ((monitor_type==2)&&(pl_idx==4)) xres*=2;
x_start=xres;

/*printf("%i %i %i %i ",vdi_x,vdi_y,xres,yres);*/
/* DIESER TEIL MUSS FUER TV_MODI UMGESCHRIEBEN WERDEN! */

if (	((monitor_type!=1)&&(monitor_type!=3)
		&&((xres!=((vdi_x+1)*((1+(pl_idx==4)))))
		||((yres*(interlace+1))!=(vdi_y+1))))
	||
		(((monitor_type==1)||(monitor_type==3))
		&&((xres!=(vdi_x+1))
		&&((yres*(interlace+1))!=(vdi_y+1))))
	)
/* ^^^^^^jetzt ist es umgeschrieben und es schaut einfach toll aus... */
{
	tree1[VSCREEN].ob_state|=SELECTED;
	sprintf(tree1[VSRES].ob_spec.tedinfo->te_ptext,"%04i%04i",vdi_x+1,vdi_y+1);
	vscreen_flag=1;
}
else
{
		tree1[VSCREEN].ob_state&=~SELECTED;
		vscreen_flag=0;
}



insert_number(tree1,CLOCK,"CLK %iMHz",clock_tab[clock]);
if (how) return;
allow_hsync=(!Kbshift(-1)&3);
schreib_regs(vcreg1,ccreg1,xcreg1,mcreg1);
allow_hsync=0;
if (tree1[MONM2].ob_state&SELECTED)
{
#if VERSION==PLL_VERSION
	if ((old_clock-clock_Mhz)>6.0) 
	{reset_wert=reset_formel(clock_Mhz);
	Supexec((long)super_reset_pll);
	}
	clock_tab[2]=old_clock;
	clock_Mhz=old_clock;
	old_ww=ww;old_hh=hh;
#endif
undo_flag=1;
set_clock(clock);
undo_flag=0;
}
else
{	clock_tab[2]=old_clock;
	clock_Mhz=old_clock;
	old_ww=ww;old_hh=hh;
	insert_number(tree1,CLOCK,"CLK %iMHz",clock_tab[clock]);
}

update(0); /* Regs  nicht mehr setzen */
/*R objc_draw(tree1,ROOTSCR,10,cx,cy,cw,ch);*/
objc_draw(tree1,CLOCK,10,cx,cy,cw,ch);
objc_draw(tree1,INTERLAC,10,cx,cy,cw,ch);
objc_draw(tree1,RESOLUT,10,cx,cy,cw,ch);
objc_draw(tree1,VPARA,10,cx,cy,cw,ch);
objc_draw(tree1,HPARA,10,cx,cy,cw,ch);
objc_draw(tree1,CLKPARA,10,cx,cy,cw,ch);
objc_draw(tree1,VSRES,10,cx,cy,cw,ch);
objc_draw(tree1,UNDO,10,cx,cy,cw,ch);

}
/*---------------------------------------------------*/	
int get_clock(void)
{
long old_stack;
int vr,vr1;
int clo=1;

	old_stack=Super(0L);
	vr=*(int*)0xff82c0L;vr1=*(int*)0xff820aL;
	if ((vr&4)&&(!(vr1&0x100)))
		clo=0;
	else
		if (!(vr&4)&&(!(vr1&0x100)))
		clo=1;
	else
	if (!(vr&4)&&((vr1&0x100)))
		clo=2;
	Super((void*)old_stack);
	return clo;
}
/*---------------------------------------------------*/	
void change_res(int parent,int type, int res1, int res2,int value)
{

	tree1[type].ob_x=res1;tree1[type].ob_y=res2;
	/*R tree1[SCREEN].ob_width=res1;*/
	sprintf(tmp,"%i",value);
	strcpy(tree1[type].ob_spec.tedinfo->te_ptext,tmp);
		
	update(1);

	if (get_clock()!=clock)
		set_clock(clock);

	objc_draw(tree1,parent,10,cx,cy,cw,ch);
}
/*---------------------------------------------------*/	
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++++++ HARDWARE-SPEZIFISCHES ZEUGS +++++++++++*/
void super_reset_pll()
{/* muss im supervisor sein */

if (!(tree1[MONM2].ob_state&SELECTED)) return;
if(*(unsigned char*)0xff9215!=255) /* sonst ist keine R�ckkopplung da*/
	paddle_adjust(paddle_formel(clock_Mhz),2);
else
 	reset_pll(0,0,reset_wert);
}
/*---------------------------------------------------*/	
void xreset_pll()
{
 	reset_pll(0,0,reset_wert);
}
/*---------------------------------------------------*/	
void curse_adjust()
{
		if(*(unsigned char*)0xff9215!=255) 
			paddle_adjust(paddle_formel(clock_Mhz),2);
}
/*---------------------------------------------------*/	
void identify_monitor()
{
	int n;
	monitor_type=mon_type();
	if (monitor_type==0) {
		/* SM124 */
		hfreq_min=350;
		hfreq_max=361;
		vfreq_min=65;
		vfreq_max=75;
		tree1[MONTV].ob_state=tree1[MONVG].ob_state=tree1[MONSVGA].ob_state=
		tree1[MONM2].ob_state=tree1[SAVER].ob_state=DISABLED;
		tree1[MONSM].ob_state=SELECTED;
	
		for(n=COL4;n<=COL32;n++) tree1[n].ob_state=DISABLED;
		clock_tab[2]=32.25;
		clock_Mhz=32.25;
		old_clock=32.25;
		tree1[LE].ob_state|=DISABLED;
		tree1[LS].ob_state|=DISABLED;
		lastmon=MONSM;
	} else if ((monitor_type==1)||(monitor_type==3)){
		 /* TV */
		hfreq_min=154;
		hfreq_max=158;
		vfreq_min=48;
		vfreq_max=65;
		tree1[MONVG].ob_state=tree1[MONSVGA].ob_state=
		tree1[MONM2].ob_state=tree1[MONSM].ob_state=DISABLED;
		tree1[MONTV].ob_state&=~DISABLED;
		tree1[MONTV].ob_state|=SELECTED;
		tree1[CLOCK].ob_state|=DISABLED;
		lastmon=MONTV;
		/*form_alert(1,"[3][TV-Modi are not|supported!][  OK  ]");
		exit(0);*/
	} else {
		/* VGA */
		hfreq_min=304;
		hfreq_max=317;
		vfreq_min=55;
		vfreq_max=75;
		lastmon=MONVG;
		load_monitor_definition();
	}
}
/*---------------------------------------------------*/	
void read_regs()
{
long old_stack;
int *vr;
int n;
if (TEST) return;
old_stack=Super(0L);
vr=(int*)0xff82a2L;
for (n=0;n<6;n++) {vcreg[n]=*vr++;vcreg1[n]=vcreg[n];}

vr=(int*)0xff8282L;
for (n=0;n<6;n++) {xcreg[n]=*vr++;xcreg1[n]=xcreg[n];}

vr=(int*)0xff820aL;
mcreg[0]=* /*(char *)*/ vr;mcreg1[0]=mcreg[0]; /* ext. clock */
/*            ^ warum nur???? Das ist FALSCH! */
/* Ist ja gut, ich hab's kapiert ... */
vr=(int*)0xff820eL;
mcreg[2]=*vr++;mcreg1[2]=mcreg[2];
mcreg[3]=*vr  ;mcreg1[3]=mcreg[3];

vr=(int*)0xff82c0L;
ccreg[0]=*vr++;ccreg1[0]=ccreg[0];
ccreg[1]=*vr;  ccreg1[1]=ccreg[1];
Super((void *) old_stack);
}
/*---------------------------------------------------*/
void schreib_regs(vcregtype vreg,vcregtype creg,
					vcregtype xreg,vcregtype mreg)
{
long old_stack;
int *vr;
int n;
	if (TEST) return;
	
	if ((!allow_hsync)&&((monitor_type==0)||(!(tree1[MONM2].ob_state)&SELECTED))) 
		return;
	
	old_stack=Super(0L);
	vr=(int*)0xff82a2L;
	if (!((monitor_type==0)||(!(tree1[MONM2].ob_state)&SELECTED)))
	{
		for (n=0;n<6;n++) { *vr++=vreg[n];}
		
		vr=(int*)0xff8282L;
		for (n=0;n<6;n++) { *vr++=xreg[n];}
		
		if (!vscreen_flag)
		{
		 vr=(int*)0xff820eL;
		*vr++=mreg[2];
		*vr=mreg[3];
		}
		
		vr=(int*)0xff82c0L;
		*vr=creg[0];
		vr=(int*)0xff82c2L;
		*vr=creg[1];
	}
	else
	{
		vr=(int*)0xff828cL;
		*vr=xreg[5];
	}
	Super((void *) old_stack); 
}
/*---------------------------------------------------*/	
void set_clock(int wert)
{
long old_stack;
int *vr,*vr1;
int vtf;
int jarre;
if (TEST) return;

if ((monitor_type==0)||(!(tree1[MONM2].ob_state)&SELECTED)) return;

old_stack=Super(0L);
vr=(int*)0xff82c0L;vr1=(int*)0xff820aL;
if (wert==0) 
	{
	mcreg[0]=0;*vr1=0;
	if (undo_flag)
		jarre=ccreg1[0];
	else
		jarre=0x186|(ccreg[0]&0x100);
	*vr=jarre;
	ccreg[0]=jarre;
	}
else if (wert==1) 
	{
	mcreg[0]=0;*vr1=0;
	if (undo_flag)
		jarre=ccreg1[0];
	else
		jarre=0x182|(ccreg[0]&0x100);
	*vr=jarre;ccreg[0]=jarre;
	}
else {
	if (undo_flag)
		jarre=ccreg1[0];
	else
		jarre=0x82|(ccreg[0]&0x100);
	ccreg[0]=jarre;
	mcreg[0]=0x100;*vr1=0x100;
	}
	
#if VERSION==PLL_VERSION

old_cm=clock_Mhz;
vtf=pll_calc(clock_Mhz,ww,hh);
/*if ((clock==2)) { Super((void *) old_stack);
						alert(ERROR10);undo(0);
						return;}*/
						
c_frequ=vtf;
pll_on=(clock==2);pll_mode=0;pll_lc_flag=0;
pll_lc=0;own_vbl_count=255-10;l_delta=1800;
tree1[CLOCKS].ob_state=DISABLED*(clock!=2);

#endif

Super((void *) old_stack); 
#if VERSION==PLL_VERSION
if (clock!=2) return;
graf_mouse(BUSYBEE,0L);
old_stack=Super(0L);
#if 1
while(l_delta>=(5+7*(clock_Mhz<36.0)))
{
	printf("\033H %x   ",(*(int*)(0xff9214))&255);
}
/*pll_on=0;*/
#endif
l_delta=100;
Super((void *) old_stack);  
graf_mouse(ARROW,0L);
#endif
}
/*---------------------------------------------------*/	
/*     Hier werden die Werte f�r die HW berechnet    */
/* cholera=Farben, cholerb=1 bei online-config       */
void set_vcdata(int cholera,int cholerb)
{
/*int cholera;*/
int tc=0;
int add1=0,add2=0;
int wunderflag=0;

vcreg[0]=hh*2+1-interlace;
vcreg[1]=y_off*2+yres*2+1;
vcreg[2]=y_off*2+1;
vcreg[3]=y_off*2+1-interlace;
vcreg[4]=vcreg[0]-2*(hh-yres-y_off);
vcreg[5]=hh*2-2*vsync+1;

/*cholera=get_color();*/
#if DEMO_VERSION
cholera=2;
#endif

if (monitor_type==2)
{
	if (cholera!=1)
	{
#if !DEMO_VERSION&(VERSION!=SOFT_VERSION)	
		if ((cholera<3)&&(clock==2)) 
			add1=32;
		else
#endif
			add1=0;
		xcreg[0]=((xres+u1+u2-8)>>2);
		xcreg[1]=0x8d-(u1+u2-160)/4+((xres-640)>>2)+(u1-42)/2+0*(((u1+u2)%4)==2);
		xcreg[2]=u1>>1;
		xcreg[3]=((u1+u2-160)>>2)+x88[cholera]+((xres-640)>>2)+(u1-42)/2-add1;

		xcreg[4]=-((u1+u2-160)>>2)+x8a[cholera]+
					((xres-640)>>2)+((u1-42)>>1)+add1/4
#if VERSION!=SOFT_VERSION
					-8*(clock==2)*(cholera==0)
#endif
					+8*(clock!=2)*(cholera==2) + 32*(cholera==0)*(clock!=2);
		xcreg[5]=hsync;
	}
#if !DEMO_VERSION	
	else
	{	/* 4 Farben */
		xcreg[0]=(xres)/32+(u1+u2)/32-2;
		/*xcreg[1]=0x12+((xres-640)/32)+1-(u2-118)/64;*/
		xcreg[1]=0x12-(u1+u2-160)/32+((xres-640)/32)+1+(u1-42)/32;
		
		xcreg[2]=1+1+(u1-42)/32;
		
		xcreg[3]=(u1+u2-160)/32+(u1-42)/32
				+x88[cholera]+((xres-640)/32)+1;
						
		xcreg[4]=-(u1+u2-160)/32+(u1-42)/32+x8a[cholera]+((xres-640)/32)+1;
		xcreg[5]=hsync/8;
	}
#endif
}
#if !DEMO_VERSION 
else
if (monitor_type==0)
	{
		xcreg[0]=(ww)/32-2;
		xcreg[1]=xcreg[2]=0;
		xcreg[3]=0x20f+((xres-640)/32);
		xcreg[4]=0x0c+((xres-640)/32);
		xcreg[5]=(ww-hsync)/32;
	}
else	/* nun der TV-Teil */
{/*�, was schreiben wir hier denn rein?*/
	vcreg[1]=max(vcreg[1],vcreg[5]-3); /* kein Cinemascope */
	vcreg[2]=min(vcreg[2],31);
		
	if (cholera!=1)
	{
		xcreg[0]=(ww/2)-2;
		xcreg[1]=409; /* Erfahrung */
		xcreg[2]=80; /* dito */
		xcreg[3]= ((u1-160))+x88_tv[cholera]+((xres-640)>>2);
		xcreg[4]=+x8a_tv[cholera]+
				((xres-640))-((160-u1)/3)+128*(cholera==0);					
/*		printf("\033H%i ",xcreg[4]);
		scanf("%i\n",&xcreg[4]);*/
		xcreg[5]=hsync*2;
	
	}
	else
	{ /* 4F/TV */
		xcreg[0]=(ww/16)-2;
		xcreg[1]=0x34;
		xcreg[2]=8;
		xcreg[3]=x88_tv[cholera]+(u1-160)/32;
		xcreg[4]=x8a_tv[cholera]+(160-u1)/4;
		xcreg[5]=hsync/4+1;
	}

}
#endif

mcreg[0]=0x100*(clock==2); /* Ext. Clock Fehler...*/

if (cholerb==1)
{/* bei der online-config. */
	if ((x_start-xres)>32)
	{
		mcreg[2]=(x_start-xres)/(coltable[cholera]*(1+(cholera==4)));
		mcreg[3]=xres/(coltable[cholera]*(1+(cholera==4)));
		/* verhindert Flackern beim kleiner drehen*/
	} 
	else
	{
	mcreg[2]=0;
	mcreg[3]=x_start/(coltable[cholera]*(1+((monitor_type==2)&&(cholera==4)))); 	
	}

}
else
{
	mcreg[2]=0;
	mcreg[3]=xres/(coltable[cholera]); /* ff8210 */
	/*           ^ war mal >> */
	/* Zeilenbreite in WORDS */
}

#if !DEMO_VERSION
if (((cholerb==1)&&(pl_idx==4))||((cholerb==0)&&(cholera==4))) tc=1;
	#if VERSION==SOFT_VERSION
		wunderflag=1;
	#else 
		if (monitor_type==0)
			wunderflag=0;
		else
			wunderflag=(cholera==1)||(cholera==3)||(cholera==4)||(clock<2);
	#endif
ccreg[0]=0x100*wunderflag+ 0x82 +4*(clock==0);
#else
		tc=0;ccreg[0]=0x182+4*(clock==0);
#endif

if ((monitor_type!=1)&&(monitor_type!=3))
	ccreg[1]=8*(1-tc)+2*interlace+4*tc;
else
	ccreg[1]=4+2*interlace;
/*regaus;*/
}
/*---------------------------------------------------*/
int check_monitor()
{
#if 0
if (monitor_type==2)
{
if ((tree1[MONVG].ob_state)&SELECTED)
	{
	if ((fabs(hfrequ-31050.0)>650.0)||(fabs(vfrequ-60.0)>3.0))
	{
	putc(7,stderr);
	alert(ERROR3);
	return 1;
	}
	}
if ((tree1[MONSVGA].ob_state)&SELECTED)
	{
	if	(((fabs(hfrequ-31050.0)>650.0)||(fabs(vfrequ-60.0)>3.0))
		&&((fabs(hfrequ-35400.0)>400.0)||(fabs(vfrequ-57.0)>3.0)))
	{
	putc(7,stderr);
	alert(ERROR7);
	return 1;
	}
	}
}
else
if (monitor_type==0)
{
if ((fabs(hfrequ-35550.0)>650.0)||(fabs(vfrequ-70.0)>7.0))
	{
	putc(7,stderr);
	alert(ERROR3);
	return 1;
	}
}
#endif
	if (((hfrequ/100.0)<hfreq_min) ||
		((hfrequ/100.0)>hfreq_max) ||
		((vfrequ)<vfreq_min) ||
		((vfrequ)>vfreq_max))
	{
		putc(7, stderr );
		alert(ERROR7);
		return 1;
	}
return 0;
}

/*---------------------------------------------------*/
/* �berpr�ft, ob Takt f�r die eingestellte Farbe �berhaupt geht*/
int check_clock(int fehlernr,int col)
{
int clk;
if (clock<2) return 1; /* 25/32 gehen immer*/
clk=clock_Mhz;
if (((col==4)||(col==3))&&(clk>46)) {alert(fehlernr);return 0;}
return 1;
}
/*---------------------------------------------------*/
/* save l�dt blowboot.prg entweder aus AUTO oder dem akt. Pfad
ganz ein, ver�ndert die entsprechenden Variablen und sichert
es in AUTO wieder ab */
int save()
{
long file1;
int file;
long oldstack;
int blubber[128];
int vxres=0,vyres=0;
int n, cholera,vtf,res_bank,z62db;
char *bootprg;
long bootlen;
char *adr;
char *param;


if (check_monitor()) return 0;
if (!check_clock(ERROR14,get_color())) return 0;
if (!(bootprg=(char*)malloc(16384)))
{
	alert(ERROR6);
	return 0;
}
file1=Fopen(boot_prg_name,0);
if (file1<0)
{
file1=Fopen("BLOWBOOT.DAT",0);
if (file1<0)
	{
	alert(ERROR2);
	return 0;
	}
}
file=(int)file1;
bootlen=Fread(file,16384,bootprg);
Fclose(file);

adr=(char*)get_tabele();
if (adr!=0)
	memcpy(bootprg+32,adr,2*5*256);


#if !DEMO_VERSION
cholera=get_color();
set_vcdata(cholera,0);
pix_doub=(cholera==4?1:0)&&(monitor_type==2); 
#else
cholera=2;
set_vcdata(2,0);
pix_doub=0;
#endif
res_bank=tree1[SECRES].ob_state&SELECTED;

for (n=0;n<6;n++) blubber[(0xa2>>1)+n]=vcreg[n];
for (n=0;n<6;n++) blubber[(0x82>>1)+n]=xcreg[n];

blubber[0xc0>>1]=ccreg[0];
blubber[0xc2>>1]=ccreg[1];
blubber[0]=0;	/* an */
blubber[1]=0x1971;

blubber[5]=mcreg[0];			/* ext syn */


blubber[7]=mcreg[2];			/* line_offset*/
blubber[8]=mcreg[3]/(pix_doub+1);/* line length */

if (tree1[VSCREEN].ob_state&SELECTED)
{
	if (*tree1[VSRES].ob_spec.tedinfo->te_ptext==' ')
		*tree1[VSRES].ob_spec.tedinfo->te_ptext='0';
	sscanf(tree1[VSRES].ob_spec.tedinfo->te_ptext,"%4i%i",&vxres,&vyres);
	if ((vxres<(xres/(pix_doub+1)))||(vyres<(yres*(interlace+1)))) {alert(ERROR11);free(bootprg);return 0;}
	vxres=(vxres>>5)<<5;vyres=(vyres>>4)<<4;
#if DEMO_VERSION
	if (((vxres)>768)||((vyres)>768)) {alert(ERROR13);free(bootprg);return 0;}
#endif
	blubber[7]=(vxres-xres/(pix_doub+1))/coltable[cholera];
	if (blubber[7]>511) {alert(ERROR13);free(bootprg);return 0;}
	blubber[45]=1; 
	blubber[46]=xres/(pix_doub+1);
	blubber[47]=yres*(interlace+1);
	blubber[48]=cholera;
}
else
{
	vxres=xres/(pix_doub+1);
	vyres=yres*(interlace+1);
	blubber[45]=0;
}

blubber[32]=vxres-1; 		     /* LINEA xres */
blubber[33]=(vyres)-1; 		/* LINEA yres */
#if !DEMO_VERSION
blubber[34]=2*vxres/coltable[cholera]; /* LINEA bytes/line  */ 
#else
blubber[34]=vxres/2; /* LINEA bytes/line  */ 
#endif
*((long*)&blubber[35])=(long)((long)blubber[34]*(long)(vyres));
/* ^ Bildschirmspeicherl�nge */

if (*((long*)&blubber[35])>2000000L)
			if (alert(ERROR12)==2) {return 0;}
#if !DEMO_VERSION
blubber[37]=(2*(vxres/(1/*+(cholera==4)*/))/coltable[cholera])<<4; /* LINEA charzeilenbreite...  */
blubber[38]=16/coltable[cholera]; 						/* LINEA Planes */

#else
blubber[37]=vxres<<3; /* LINEA charzeilenbreite...  */
blubber[38]=4; 						/* LINEA Planes */
#endif

#if VERSION==PLL_VERSION
vtf=pll_calc(clock_Mhz,ww,hh);
blubber[39]=vtf;
blubber[40]=(clock==2);
blubber[41]=reset_formel(clock_Mhz)/PLL_DIV;
if (vtf==c_frequ)
	{oldstack=Super(0L);
	blubber[42]=((*(int*)0xff9214)&255)-7;
	Super(oldstack);
	}
else
	blubber[42]=paddle_formel(clock_Mhz);
blubber[43]=blubber[44]=0;
#endif
blubber[49]=monitor_type;
blubber[50]=0;

if (use_critical)
if (!(tree1[MONM2].ob_state&SELECTED)) {alert(ERROR15);}
	else
	if (hfrequ<30000.0)
		blubber[50]=1;
	
if (tree1[SAVER].ob_state&SELECTED)
{
	long time;
	sscanf(tree1[SAVTIME].ob_spec.tedinfo->te_ptext,"%li",&time);
	*(long*)&blubber[51]=time*2000L;
	blubber[53]=0x100*(time!=0);		/* Saver aktiv */
/*	*(int*)(param+8)=0;		Z�hler-Startwert */
}
else
{
	*(long*)&blubber[51]=300*200L; /* Default 5min */
	blubber[53]=0;		/* Saver inaktiv */
/*	*(int*)(param+8)=0;		 Z�hler-Startwert */
}

#if 1
if ((adr!=0)&&(sledgehammer!=-1))
	{
	*(int*)(adr+256*pl_idx+0x40)=sledgehammer;
	*(int*)(bootprg+32+256*pl_idx+0x40)=sledgehammer;
	}	
#endif

if (adr!=0)
	(void)memcpy(adr+256*cholera+5*256*res_bank,&blubber[0],256);


#if !DEMO_VERSION
	for(n=MONTV;n<=MONM2;n++)
		if (tree1[n].ob_state&SELECTED) break;
	z62db=n-MONTV;
	*(int*)(bootprg+2614+168)=z62db;
	if (MONM2==n)
	{
		if (ffc_on)
			*(int*)(bootprg+2614+170)=42;
		else
			*(int*)(bootprg+2614+170)=0;
	}
if (adr!=0)
	(void)memcpy(adr+2614+168-32,bootprg+2614+168,4);
#endif

(void)memcpy(bootprg+0x20+256*cholera+5*256*res_bank,&blubber[0],256);
/* ^^^ in den Speicher fuers Saven nach Auto... */
if (adr!=0) 
	(void)memcpy(bootprg+0x20+10*256,adr+10*256,6);
param=bootprg+0x20+10*128*2+4+6;
*(int*)(param)=monitor_type; 
*(int*)(bootprg+2614+166)=h2_v3;

#if 1
if ((adr!=0)&&(res_bank==0)&&(pl_idx==cholera))
	sledgehammer=*(int*)(adr+256*pl_idx+0x40);
if (adr!=0) *(int*)(adr+256*pl_idx+0x40)=0;
#endif

file1=Fcreate(boot_prg_name,0);
if (file1<0)
	{
	alert(ERROR5);
	free(bootprg);
	return 0;
	}
	file=(int)file1;
if (Fwrite(file,bootlen,bootprg)!=bootlen)
	{
	alert(ERROR5);
	free(bootprg);
	return 0;
	}
	
Fclose(file);
free(bootprg);
if (!res_bank)
sprintf(tree1[COL2RES+get_color()].ob_spec.tedinfo->te_ptext,
				"%i*%i",vxres,vyres);
else
sprintf(tree1[CO2RES2+get_color()].ob_spec.tedinfo->te_ptext,
				"%i*%i",vxres,vyres);
				
if (blubber[45]) 	/* Virtual screen on*/
	sprintf(tree1[(res_bank?COL2VR2:COL2VR)+get_color()].ob_spec.tedinfo->te_ptext,
				"(%i*%i)",blubber[46],blubber[47]);
else
	sprintf(tree1[(res_bank?COL2VR2:COL2VR)+get_color()].ob_spec.tedinfo->te_ptext,"        ");
	
				
objc_draw(tree1,COLSELEC,10,cx,cy,cw,ch);

return 1;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++*/
void main()
{
	int ex_ob,xtmp,ytmp;
	int actual_color;
	int act_TREE;
	int comefrom;
	int boot=0;
	int dummy;
	long old_stack,boot_mem;
	
	
	 
	init();
#if 1
	boot_mem=get_tabele();
	if (boot_mem!=0) {
		sledgehammer=*(int*)(boot_mem+256*pl_idx+0x40);
		*(int*)(boot_mem+256*pl_idx+0x40)=0;
	}
#endif
	
	read_regs();
	
	/*if (!(ccreg1[1]&2)) {alert(ERROR17);exit(0);}*/
	dummy=Vsetmode(-1);
	if ((dummy&0x10)&&(dummy&0x100)) {
		long adr;
		adr=(char*)get_tabele();
		if ((adr!=0)&&(sledgehammer!=-1))
			*(int*)(adr+pl_idx*256+0x40)=sledgehammer;
		alert(ERROR17);exit(0);
	}
	
	rsrc_gaddr(R_TREE,TREE1,&tree1);
	rsrc_gaddr(R_TREE,TREE2,&tree2);
	rsrc_gaddr(R_TREE,TREE3,&tree3);
	rsrc_gaddr(R_TREE,TREE4,&tree4);
	rsrc_gaddr(R_TREE,TREE5,&tree5);
	
	identify_monitor();
	load_configuration();
	act_TREE=TREE1;
	tree1[COL2+pl_idx].ob_state|=SELECTED;
	if (monitor_type==2)
		tc_sel=(pl_idx==4);
#if !DEMO_VERSION
	actual_color=pl_idx;
#else
	actual_color=8192;
	for(dummy=0;dummy<12;dummy++) {
		actual_color>>=1;
	}
#endif
		
	undo(1);
	
	if ((xres==cur_col_res[6]) &&
		(((1+interlace)*yres)==cur_col_res[7])&&
		((vdi_x+1)==cur_col_res[4])&&
		((vdi_y+1)==cur_col_res[5])) {
		tree1[FIRSTRES].ob_state=0;
		tree1[SECRES].ob_state|=SELECTED;
	} else {
		tree1[SECRES].ob_state=0;
		tree1[FIRSTRES].ob_state|=SELECTED;
	}
		
	
	#if VERSION==PLL_VERSION
	strcpy(tree2[INFOLI1].ob_spec.free_string,"Hard 2 VERSION V.final");
	if (!TEST) Supexec((long)(install_vbl));
	if ((old_c_frequ!=-1)&&(clock==2)) {
		/* PLL schon da und laufend...*/
		double clpll;
		int rclpll;
		old_c_frequ=old_c_frequ;
		clpll=7.0*38400.0/old_c_frequ*(double)(ww)*(double)(hh)/1e6;
		
		rclpll=4*max(33.0,min(clpll,56.0));
		clpll=rclpll/4.0;
		clock_tab[2]=clpll;
		clock_Mhz=clock_tab[2];
		old_clock=clock_Mhz;
		old_cm=old_clock;
		c_frequ=old_c_frequ;
		pll_on=1; /* ein */
	} else {
		old_cm=33.0;
		clock_tab[2]=old_cm;
		clock_Mhz=clock_tab[2];
		if (h2_v3==-1) {
			/* test auf H2 mit spannungsregler */
			reset_wert=10;
			Supexec((long)xreset_pll);
			old_stack=Super(0L);
			if (*(unsigned char*)0xff9215!=255) {
				if (*(unsigned char*)0xff9215>0x60)
					h2_v3=0;
				else {
					h2_v3=1;
				}	
			}
			Super(old_stack);
		
			reset_wert=200;
			tree1[MONM2].ob_state|=SELECTED;
			Supexec((long)super_reset_pll);
			reset_wert=reset_formel(33.0);
			Supexec((long)super_reset_pll);
		}
	
		tree1[MONM2].ob_state&=~SELECTED;
	}
	
#else
	tree1[CLOCKS].ob_state=DISABLED;
	/*tree1[CRITICAL].ob_state=DISABLED;*/
	old_clock=50.0;
#endif
#if VERSION==XTAL_VERSION
	strcpy(tree2[INFOLI1].ob_spec.free_string,"Hard 1 Version V.final");
#endif
#if DEMO_VERSION
	strcpy(tree2[INFOLI1].ob_spec.free_string,"DEMO VERSION");
#endif
	
	calc_paras(ww,hh,xres,yres);
	change_rsc();

#if DEMO_VERSION
	tree4[SUPER78].ob_state|=DISABLED;
	tree4[SUPER78].ob_state&=~SELECTED;
#endif

	for (n=MONTV; n<=MONM2; n++)
		if (tree1[n].ob_state&SELECTED)
			break;
	n-=MONTV;
	if (n<4) {
		tree4[SUPER78].ob_state|=DISABLED;
	}
	show_dialog(TREE1,0,1,-1);
	form_dial(FMD_START,cx,cy,cw,ch,cx,cy,cw,ch);
	graf_mouse(ARROW,0);
	while(1) {
		ex_ob=do_dialog(act_TREE,0);
		if (act_TREE==TREE1)
		{
			if (ex_ob==QUIT) 
				break;
			
			if ((SUUS==ex_ob)) {
				move_box(tree1,TREE1,mcx,mcy,mcw,mch);
			}
			
			
			if ((monitor_type!=0)&&(ex_ob>=COL2)&&(ex_ob<=COL32)) {
#if !DEMO_VERSION
				if ((ex_ob==COL32)&&(monitor_type==2)) 
					tc_sel=1;
				else 
					tc_sel=0;
				change_res(XSLIDER,XRES,trxres(xres),0,xres);
				actual_color=get_color();
#else
				tree1[ex_ob].ob_state=0;
				tree1[COL2].ob_state|=SELECTED;
				actual_color=2;
	#endif
			}
		
			xtmp=trxres(xres);ytmp=0;
			if (handle_slider(ex_ob,tree1,XSLIDER,XRES,
				XPLUS,XMINUS,0,0,1,8,0,0,1,&xtmp,&ytmp)) {
				xres=min(320+16*xtmp,1880);
				/*xres=(xres>>5)*32;*/
				if ((monitor_type==1)||(monitor_type==3)) {
					u1=160-((xres-640)>>2)-((xres-640)>>3);
					u2=224-((xres-640)>>1)-((xres-640)>>2)+((xres-640)>>3);
		/*			printf("\n\n\n%i %i",u1,u2); */
				}
				ww=xres+u1+u2;
				
				hsync=max(hsync,320-(u1+u2)+((xres-640)>>2));
				hsync=min(hsync,300-8-(((1200-(xres+u1+u2-15))>>4)<<2));
				
				update(2);
				
				objc_draw(tree1,HSLIDER,10,cx,cy,cw,ch);
				objc_draw(tree1,HPARA,10,cx,cy,cw,ch);
				
				change_res(XSLIDER,XRES,trxres(xres),0,xres);
				
			}
				
			xtmp=tryres(yres);ytmp=0;
			if (handle_slider(ex_ob,tree1,YSLIDER,YRES,
				YPLUS,YMINUS,0,0,1+(1-interlace),
				8,0,0,1,&xtmp,&ytmp)) {
				hh=min((hh-yres-y_off)+(y_off+200+8*xtmp),1152);
				yres=min(200+8*xtmp,hh-y_off);
				change_res(YSLIDER,YRES,tryres(yres),0,yres);
			}
				
			xtmp=y_off;ytmp=0;
			if (handle_slider(ex_ob,tree1,FSSLIDER,FS,
				FSPLUS,FSMINUS,0,0,1+4*(DEMO_VERSION),
				8+2*DEMO_VERSION,0,0,1,&xtmp,&ytmp)) {
				if (xtmp) {
					hh=hh-(y_off-(xtmp));
					y_off=xtmp;
				} 
				update(1);
				objc_draw(tree1,FSSLIDER,10,cx,cy,cw,ch);
				objc_draw(tree1,FESLIDER,10,cx,cy,cw,ch);
			}
				
			xtmp=(hh-y_off-yres);ytmp=0;
			if (handle_slider(ex_ob,tree1,FESLIDER,FE,
				FEPLUS,FEMINUS,0,0,1+4*DEMO_VERSION,
				8+2*DEMO_VERSION,0,0,1,&xtmp,&ytmp)) {
				hh=y_off+yres+xtmp;
				update(1);
				objc_draw(tree1,FESLIDER,10,cx,cy,cw,ch);
			}	
			xtmp=u1/2;ytmp=0;
			if (handle_slider(ex_ob,tree1,LSSLIDER,LS,
				LSPLUS,LSMINUS,0,0,1+4*DEMO_VERSION,8+2*DEMO_VERSION,0,0,1,&xtmp,&ytmp)) 
			{
				u1=(xtmp*2);
				/*hsync=max(hsync,((320-(u1+u2))-16+((xres-640)>>2)));*/
				ww=xres+u1+u2;
				update(1);
				objc_draw(tree1,LSSLIDER,10,cx,cy,cw,ch);
				objc_draw(tree1,HSLIDER,10,cx,cy,cw,ch);
		
			}		
				
			xtmp=u2/2;ytmp=0;
			if (handle_slider(ex_ob,tree1,LESLIDER,LE,
				LEPLUS,LEMINUS,0,0,1+4*DEMO_VERSION,8+2*DEMO_VERSION,0,0,1,&xtmp,&ytmp)) 
			{
				u2=(xtmp*2);
				ww=xres+u1+u2;
				/*hsync=max(hsync,((320-(u1+u2))-20+((xres-640)>>2)));*/
				update(1);
				objc_draw(tree1,LESLIDER,10,cx,cy,cw,ch);
				objc_draw(tree1,HSLIDER,10,cx,cy,cw,ch);
			}
		
			xtmp=vsync*2;ytmp=0;
			if (handle_slider(ex_ob,tree1,VSLIDER,VSYNC,
				VPLUS,VMINUS,0,0,2,8,0,0,1,&xtmp,&ytmp)) 
			{
				vsync=max(1,xtmp/2);
				update(2);
				objc_draw(tree1,VSLIDER,10,cx,cy,cw,ch);
			}
			
			xtmp=hsync>>(2/*-(tree1[MONTV].ob_state&SELECTED!=0)*/);
			ytmp=0;
			if (handle_slider(ex_ob,tree1,HSLIDER,HSYNC,
				HPLUS,HMINUS,0,0,1,8,0,0,1,&xtmp,&ytmp)) 
			{
				hsync=xtmp<<(2/*-(tree1[MONTV].ob_state&SELECTED!=0)*/);
				allow_hsync=(Kbshift(-1)&3);
				update(2);
				allow_hsync=0;
				objc_draw(tree1,HSLIDER,10,cx,cy,cw,ch);
			}
				
			xtmp=(int)(4.0*(clock_Mhz-32.0-1.0));
			ytmp=0;		
				
#if VERSION==PLL_VERSION
			if ((clock==2)&&(handle_slider(ex_ob,tree1,CLOCKSLI,CLOCKS,
				CLOCKPLU,CLOCKMIN,0,0,1,8,0,0,1,&xtmp,&ytmp))) {
				old_cm=clock_Mhz;
				clock_Mhz=xtmp/4.0+32.0+1.0;
				clock_Mhz=min(51.5,clock_Mhz);
		/*		
				reset_wert=reset_formel(clock_Mhz);
				Supexec((long)super_reset_pll);
				
				if (fabs(old_cm-clock_Mhz)>4.0)
						Supexec((long)curse_adjust);
		*/
				insert_float(tree1,CLOCKS,"%.1lf",clock_Mhz);
				sprintf(tmp,"CLK %.0lfMHz",clock_tab[clock]);
				strcpy(tree1[CLOCK].ob_spec.tedinfo->te_ptext,tmp);
				
				clock_tab[2]=clock_Mhz;
				update(2);
		
				set_clock(clock) ;
					
				objc_draw(tree1,CLOCKSLI,10,cx,cy,cw,ch);
		/*		objc_draw(tree1,CLOCK,10,cx,cy,cw,ch);	*/
				objc_draw(tree1,HFREQU,10,cx,cy,cw,ch);	
				objc_draw(tree1,VFREQU,10,cx,cy,cw,ch);	
				}
#endif		
			if (ex_ob==CLOCK) {
				if ((clock==1)&&(Kbshift(-1)&3)) {
					int mousex,mousey,dummy;
					OBJECT* my_menu;
					
					rsrc_gaddr(R_TREE,CLCKMENU,&my_menu);
					my_clk_menu.mn_tree=my_menu;
					graf_mkstate(&mousex,&mousey,&dummy,&dummy);
					menu_popup(&my_clk_menu,mousex+16,mousey,&my_clk_menu);
					if (my_clk_menu.mn_item>1) {
						systemclockindex=my_clk_menu.mn_item-2;
						clock_tab[1]=systemclocklist[systemclockindex];
					}
				} else {
#if (VERSION!=SOFT_VERSION)
					clock=(clock==1?2:(clock==2?0:1));
#else
					clock=(clock==1?0:1);
#endif
				}
#if (VERSION!=SOFT_VERSION)
				if ((clock==2)&&(boot==0)&&(u1==42)) {
					u1=52;
					boot=1;
				}
#endif
		
				set_clock(clock) ;
				clock_tab[2]=clock_Mhz;
				sprintf(tmp,"CLK %.0lfMHz",clock_tab[clock]);
				strcpy(tree1[CLOCK].ob_spec.tedinfo->te_ptext,tmp);
				update(1);
				objc_draw(tree1,CLOCK,10,cx,cy,cw,ch);	
				objc_draw(tree1,CLOCKSLI,10,cx,cy,cw,ch);
			} 
		
			if (ex_ob==INTERLAC) {
				interlace=1-interlace;
				if (interlace==0) 
					if ((yres&15)==8) {	
						yres+=8;
						hh+=8;
					}
				update(1);
				objc_draw(tree1,INTERLAC,10,cx,cy,cw,ch);
				objc_draw(tree1,YRES,10,cx,cy,cw,ch);
			}
			
			if (ex_ob==UNDO) {
				undo(0);
			}
			
			if (ex_ob==SAVE) 
			{
				graf_mouse(BUSYBEE,0);
				(void)save();
				graf_mouse(ARROW,0);
				objc_draw(tree1,SAVE,10,cx,cy,cw,ch);
			}
				
			if (ex_ob==SAVECONF) 
			{
				do_save_conf();
				objc_draw(tree1,SAVECONF,10,cx,cy,cw,ch);
			}
			
		} 						/*---------> END IF actree=tree1 */
	
		if (act_TREE==TREE3)					/* NUR BEI LOAD CONF-BOX */
		{
			int wwx,tmpos=config_pos;
	
			wwx=tree3[CNFTYPE].ob_height/8;
			xtmp=0;ytmp=config_pos*wwx;
			
			if ((SUUS2==ex_ob))
			{
				act_TREE=TREE1;
				move_box(tree1,TREE1,mcx,mcy,mcw,mch);
				act_TREE=TREE3;
				show_dialog(TREE3,0,0,TREE1);
			}
			
			if (handle_slider(ex_ob,tree3,CNFSLIDE,CNFTYPE,
				0,0,CNFPLUS,CNFMINUS,0,0,wwx,wwx*8-1,2,&xtmp,&ytmp)) 
			{
		
				config_pos=ytmp/wwx;
				config_pos=max(0,min(config_pos,config_nr-8));
				format_configs(config_pos);
				if (tmpos!=config_pos) 
				{
					objc_draw(tree3,LIST,10,cx,cy,cw,ch);
					objc_draw(tree3,CNFSLIDE,10,cx,cy,cw,ch); 
				}
			}	
			if (ex_ob==CNFCANCE)
			{
				act_TREE=TREE1;
				show_dialog(TREE1,0,0,-2);
			}
				
			if (ex_ob==REMOVE)
			{
				for(n=0;n<8;n++) if (tree3[CNFTY1+n].ob_state&SELECTED) break;
				config_sel=config_pos+n;
				if (config_sel<max(1,config_nr)) {
					if (remove_conf(config_sel)) {
						load_configuration_file();
						format_configs(config_pos);
						objc_draw(tree3,LIST,10,cx,cy,cw,ch);
						objc_draw(tree3,CNFSLIDE,10,cx,cy,cw,ch); 
					}
				}
				objc_draw(tree3,REMOVE,10,cx,cy,cw,ch);
			}	
				
			if (ex_ob==CNFOK)
			{
				int old_c;
			
				for(n=0;n<8;n++) if (tree3[CNFTY1+n].ob_state&SELECTED) break;
				config_sel=config_pos+n;
				if (config_sel<max(1,config_nr)) {
					old_c=clock;
					old_cm=clock_Mhz;
					clock=configuration[config_sel].clock;
					clock_Mhz=configuration[config_sel].clock_Mhz;
				
					if (check_clock(ERROR16,pl_idx)) {
						xres=configuration[config_sel].xres;
						yres=configuration[config_sel].yres;
						ww=configuration[config_sel].width;
						hh=configuration[config_sel].height;
						u1=configuration[config_sel].leb;
						u2=configuration[config_sel].rib;
						y_off=configuration[config_sel].upb;
						vsync=configuration[config_sel].vsync;
						hsync=configuration[config_sel].hsync;
						clock=configuration[config_sel].clock;
						clock_Mhz=configuration[config_sel].clock_Mhz;
						interlace=configuration[config_sel].interlace;
						if (interlace) yres/=2;
#if VERSION==PLL_VERSION
						clock_tab[2]=clock_Mhz;
#else
						clock_tab[2]=clock_Mhz=50.0;
#endif
						if (old_c==2)
							update(2);
#if VERSION==PLL_VERSION
						if ((clock==2)&&((fabs(clock_Mhz-old_cm)>5.0)||(old_c!=2))) 
						{
							pll_on=0;
							reset_wert=reset_formel(clock_Mhz);
							Supexec((long)super_reset_pll);
							evnt_timer(50,0);			
						}
		
						clock_tab[2]=clock_Mhz;
#endif
						insert_float(tree1,CLOCKS,"%.1lf",clock_Mhz);
						sprintf(tmp,"CLK %.0lfMHz",clock_tab[clock]);
						if (old_c!=2) update(2);
						set_clock(clock);
					} else {
						clock=old_c;
						clock_Mhz=old_cm;
					}
				}
				act_TREE=TREE1;
				show_dialog(TREE1,0,0,-2);
		
			}		
		}				/*------------> end if TREE3=LOADCONF */
		
		if (act_TREE==TREE4)
		{
			if (ex_ob==SUUS4)
			{
				act_TREE=TREE1;
				move_box(tree1,TREE1,mcx,mcy,mcw,mch);
				act_TREE=TREE4;
				show_dialog(TREE4,0,0,TREE1);
			}
			
			if (ex_ob==SELMONTY)
			{
				sprintf(tree5[VFREQMIN].ob_spec.tedinfo->te_ptext,
					"%03i",vfreq_min);
				sprintf(tree5[VFREQMAX].ob_spec.tedinfo->te_ptext,
					"%03i",vfreq_max);
				sprintf(tree5[HFREQMIN].ob_spec.tedinfo->te_ptext,
					"%03i",hfreq_min);
				sprintf(tree5[HFREQMAX].ob_spec.tedinfo->te_ptext,
					"%03i",hfreq_max);
				load_monitor_definition();
				act_TREE=TREE5;
				comefrom=TREE4;
				show_dialog(TREE5,0,0,TREE1);
			}
			
			if (ex_ob==OPTIOK)
			{
				ffc_on=tree4[SUPER78].ob_state&SELECTED;
				use_critical=tree4[CRITICAL].ob_state&SELECTED;
				act_TREE=TREE1;
				show_dialog(TREE1,0,0,-2);
			}
			
			if (ex_ob==OPTICANC)
			{
				act_TREE=TREE1;
				show_dialog(TREE1,0,0,-2);
			}
		}									/*------------> end if TREE4=OPTIONS */
		
		if (act_TREE==TREE1) {
			if (mon_type==2)
				switch(ex_ob) {
				case MONTV:
					hfreq_min=149;
					hfreq_max=151;
					vfreq_min=48;
					vfreq_max=65;
					break;
				case MONSM:
					hfreq_min=354;
					hfreq_max=358;
					vfreq_min=65;
					vfreq_max=75;
					break;
				case MONVG:
					hfreq_min=309;
					hfreq_max=313;
					vfreq_min=55;
					vfreq_max=75;
					lastmon=MONVG;
					change_rsc();
					show_dialog(TREE1,0,0,-2);
					break;
				case MONSVGA:
					if (lastmon!=MONSVGA) {
						hfreq_min=352;
						hfreq_max=358;
						vfreq_min=58;
						vfreq_max=62;
						lastmon=MONSVGA;
						load_monitor_definition();
					}
					act_TREE=TREE5;
					comefrom=TREE1;
					sprintf(tree5[VFREQMIN].ob_spec.tedinfo->te_ptext,
						"%03i",vfreq_min);
					sprintf(tree5[VFREQMAX].ob_spec.tedinfo->te_ptext,
						"%03i",vfreq_max);
					sprintf(tree5[HFREQMIN].ob_spec.tedinfo->te_ptext,
						"%03i",hfreq_min);
					sprintf(tree5[HFREQMAX].ob_spec.tedinfo->te_ptext,
						"%03i",hfreq_max);
					show_dialog(TREE5,0,0,TREE1);
					break;
				case MONM2:
					if (lastmon!=MONM2) {
						hfreq_min=300;
						hfreq_max=500;
						vfreq_min=58;
						vfreq_max=88;
						lastmon=MONM2;
						load_monitor_definition();
					}
					lastmon=MONM2;
					act_TREE=TREE5;
					comefrom=TREE1;
					sprintf(tree5[VFREQMIN].ob_spec.tedinfo->te_ptext,
						"%03i",vfreq_min);
					sprintf(tree5[VFREQMAX].ob_spec.tedinfo->te_ptext,
						"%03i",vfreq_max);
					sprintf(tree5[HFREQMIN].ob_spec.tedinfo->te_ptext,
						"%03i",hfreq_min);
					sprintf(tree5[HFREQMAX].ob_spec.tedinfo->te_ptext,
						"%03i",hfreq_max);
					show_dialog(TREE5,0,0,TREE1);
					break;
				}
		}
	
		if (act_TREE==TREE5)
		{
			if (ex_ob==SUUS5)
			{
				act_TREE=TREE1;
				move_box(tree1,TREE1,mcx,mcy,mcw,mch);
				act_TREE=TREE5;
				show_dialog(TREE5,0,0,TREE1);
			}
			
			if (ex_ob==MONTYOK || ex_ob==MONTYSAV)
			{
				if (strlen(tree5[HFREQMIN].ob_spec.tedinfo->te_ptext)==2)
					strcat(tree5[HFREQMIN].ob_spec.tedinfo->te_ptext,"0");
					
				if (strlen(tree5[HFREQMAX].ob_spec.tedinfo->te_ptext)==2)
					strcat(tree5[HFREQMAX].ob_spec.tedinfo->te_ptext,"0");
					
				sscanf(tree5[VFREQMIN].ob_spec.tedinfo->te_ptext,
					"%3i",&vfreq_min);
				sscanf(tree5[VFREQMAX].ob_spec.tedinfo->te_ptext,
					"%3i",&vfreq_max);
				sscanf(tree5[HFREQMIN].ob_spec.tedinfo->te_ptext,
					"%3i",&hfreq_min);
				sscanf(tree5[HFREQMAX].ob_spec.tedinfo->te_ptext,
					"%3i",&hfreq_max);
			}
			
			if (ex_ob==MONTYOK)
			{
			
#if 0
				printf("\033H%4i%4i\n", vfreq_min, vfreq_max);
				printf("%4i%4i\n", hfreq_min, hfreq_max);
#endif
				change_rsc();
				act_TREE=comefrom;
				show_dialog(comefrom,0,0,-2);
			}
			
			if (ex_ob==MONTYSAV)
			{
#if 0
				printf("\033H%4i%4i\n", vfreq_min, vfreq_max);
				printf("%4i%4i\n", hfreq_min, hfreq_max);
#endif
				save_monitor_definition();
				change_rsc();
				act_TREE=comefrom;
				show_dialog(comefrom,0,0,-2);
			}

			if (ex_ob==MONTYCAN)
			{
				act_TREE=comefrom;
				show_dialog(comefrom,0,0,-2);
			}
		}									/*------------> end if TREE4=OPTIONS */
		
		if ((ex_ob==INFORM)&&(act_TREE==TREE1))
		{
			act_TREE=TREE2; 
			show_dialog(TREE2,0,0,TREE1);
		}
		
		if ((ex_ob==OPTIONS)&&(act_TREE==TREE1))	/* mu� nach dem if (act_TREE=TREE1) stehen! */
		{
				
			tree4[SUPER78].ob_state=ffc_on*SELECTED;
			
			if (tree1[MONM2].ob_state&SELECTED)
				tree4[SUPER78].ob_state&=~DISABLED;
			else
				tree4[SUPER78].ob_state|=DISABLED;
				
			tree4[CRITICAL].ob_state=use_critical*SELECTED;
			act_TREE=TREE4;
			(void)graf_growbox(tree1[OPTIONS].ob_x,tree1[OPTIONS].ob_y,
			tree1[OPTIONS].ob_width,tree1[OPTIONS].ob_height,
			tree4[0].ob_x,tree4[0].ob_y,
			tree4[0].ob_width,tree4[0].ob_height);
			show_dialog(TREE4,0,0,TREE1);
		}
				
		if ((ex_ob==LOAD)&&(act_TREE==TREE1))	/* mu� nach dem if (act_TREE=TREE1) stehen! */
		{
			act_TREE=TREE3;
			format_configs(config_pos);
/*			(void)graf_growbox(tree1[LOAD].ob_x,tree1[LOAD].ob_y,
			tree1[LOAD].ob_width,tree1[LOAD].ob_height,
			tree3[0].ob_x,tree3[0].ob_y,
			tree3[0].ob_width,tree3[0].ob_height);*/
			show_dialog(TREE3,0,0,TREE1);
		}
			
		if ((ex_ob==INFOOK)&&(act_TREE==TREE2)) {
			act_TREE=TREE1;
			show_dialog(TREE1,0,0,-2);
		}
	} /* end while(1) */
	undo(0);
		
#if VERSION==PLL_VERSION
	if (!TEST)	Supexec ((long)de_install_vbl);
#endif

	clean_up();
	{
	long adr;
		adr=(char*)get_tabele();
		if ((adr!=0)&&(sledgehammer!=-1)) 
			*(int*)(adr+pl_idx*256+0x40)=sledgehammer;
	}
	quit_all();
} 