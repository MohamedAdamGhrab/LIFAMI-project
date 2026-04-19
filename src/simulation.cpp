#include <Grapic.h>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace grapic;
using namespace std;

const int WIN_W=1100;
const int WIN_H=700;
const int PANEL_W=280;
const int CELL=15;
const int DIMW=(WIN_W-PANEL_W)/CELL;
const int DIMH=WIN_H/CELL;
const int vide=0;
const int herbe=1;
const int brule=2;
const int feu=3;
const int cendre=4;
const int Femelle=0;
const int Male=1;
const int MAX_LAPINS=400;
const int MAX_LOUPS=100;
const int MAX_VACHES=50;
const int Energie_depart_lapin=60;
const int Energie_depart_loup=80;
const int Energie_depart_vache=90;
const int Energie_max=120;
const int Energie_manger_herbe=40;
const int Energie_manger_lapin=60;
const int Energie_manger_vache=100;
const int Age_max_lapin=3000;
const int Age_max_loup=2000;
const int Age_max_vache=4000;
const int NB_lapins_depart=30;
const int NB_loups_depart=15;
const int NB_vaches_depart=8;
const int HIST=200;
const int proba_Herbe_pousse=5;
const int proba_repro_lapin=1;
const int proba_repro_loup=2;
const int proba_repro_vache=1;
const int Delai_repro_lapin=80;
const int Delai_repro_loup=120;
const int Delai_repro_vaches=200;
const int Densite_max_lapin=3;
const int Rayon_densite=3;
const int ECH_niv2_lapin=1;
const int ECH_niv3_lapin=3;
const int ECH_niv4_lapin=6;
const int Repas_niv2_loup=3;
const int Repas_niv3_loup=6;
const int SAIN=0;
const int INFECTE=1;
const int RETABLI=2;
const int DUREE_Infection=200;
const int DUREE_Immunite=400;
const int Proba_contagion=8;
const int Proba_infection_ini=1;
const int MAX_FERMIERS=5;
const int PRINTEMPS=0;
const int ETE=1;
const int AUTOMNE=2;
const int HIVER=3;
const int Meteorite_rayon_cratere=4;
const int Meteorite_rayon_onde=8;
const int Meteorite_duree_impact=30;
const int Meteorite_duree_onde=50;
const int Meteorite_duree_brule=500;
const int MAX_METEORITES=3;
const int proba_propagation_feu=15;
const int duree_feu=20;
const int duree_cendre=300;

struct Animal{
    int x,y;
    int px_avant,py_avant;
    int dir_x,dir_y;
    int genre;
    int energie;
    int age;
    int niveau;
    int nb_echappements;
    int nb_repas;
    int frames_depuis_repro;
    bool dans_vision_loup;
    bool vivant;
    bool en_meute;
    int etat;
    int timer_maladie;
};

struct Fermier{
    int x,y;
    int rayon;
    bool actif;
};

struct Meteorite{
    int cx,cy;
    int timer;
    bool actif;
};

struct Monde{
    int grille[DIMW][DIMH];
    int timer_grille[DIMW][DIMH];
    Animal lapins[MAX_LAPINS];
    Animal loups[MAX_LOUPS];
    Animal vaches[MAX_VACHES];
    int nb_lapins;
    int nb_loups;
    int nb_vaches;
    int frame;
    int hist_lapins[HIST];
    int hist_loups[HIST];
    int hist_idx;
    int nb_lapins_niv[5];
    int nb_loups_niv[5];
    Plot courbe;
    float temps_courbe;
    float lv_N;
    float lv_P;
    int saison;
    Fermier fermiers[MAX_FERMIERS];
    int nb_fermiers;
    int nb_infectes;
    int vitesse;
    int nb_infectes_lapins;
    int nb_infectes_loups;
    int nb_infectes_vaches;
    Meteorite meteorites[MAX_METEORITES];
    int nb_meteorites;
    bool incendie_actif;
};

int hasard(int max){
    return rand()%max;
}

bool chance(int p){
    return hasard(100)<p;
}

int valabs(int a){
    if(a<0){
        return -a;
    }
    return a;
}

int dist_carre(int x1,int y1,int x2,int y2){
    return (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
}

void ajouter_lapin(Monde &m,int x,int y){
    if(m.nb_lapins>=MAX_LAPINS){
        return;
    }
    Animal &a=m.lapins[m.nb_lapins];
    a.x=x;
    a.y=y;
    a.px_avant=x;
    a.py_avant=y;
    a.dir_x=1;
    a.dir_y=0;
    a.genre=hasard(2);
    a.energie=Energie_depart_lapin;
    a.age=0;
    a.niveau=1;
    a.nb_echappements=0;
    a.nb_repas=0;
    a.frames_depuis_repro=0;
    a.dans_vision_loup=false;
    a.vivant=true;
    a.en_meute=false;
    a.etat=SAIN;
    a.timer_maladie=0;
    m.nb_lapins++;
}

void ajouter_loup(Monde &m,int x,int y){
    if(m.nb_loups>=MAX_LOUPS){
        return;
    }
    Animal &a=m.loups[m.nb_loups];
    a.x=x;
    a.y=y;
    a.px_avant=x;
    a.py_avant=y;
    a.dir_x=1;
    a.dir_y=0;
    a.genre=hasard(2);
    a.energie=Energie_depart_loup;
    a.age=0;
    a.niveau=1;
    a.nb_echappements=0;
    a.nb_repas=0;
    a.frames_depuis_repro=0;
    a.dans_vision_loup=false;
    a.vivant=true;
    a.en_meute=false;
    a.etat=SAIN;
    a.timer_maladie=0;
    m.nb_loups++;
}

void ajouter_vache(Monde &m,int x,int y){
    if(m.nb_vaches>=MAX_VACHES){
        return;
    }
    Animal &a=m.vaches[m.nb_vaches];
    a.x=x;
    a.y=y;
    a.px_avant=x;
    a.py_avant=y;
    a.dir_x=1;
    a.dir_y=0;
    a.genre=hasard(2);
    a.energie=Energie_depart_vache;
    a.age=0;
    a.niveau=1;
    a.nb_echappements=0;
    a.nb_repas=0;
    a.frames_depuis_repro=0;
    a.dans_vision_loup=false;
    a.vivant=true;
    a.en_meute=false;
    a.etat=SAIN;
    a.timer_maladie=0;
    m.nb_vaches++;
}

void init(Monde &m){
    srand(time(NULL));
    m.nb_lapins=0;
    m.nb_loups=0;
    m.nb_vaches=0;
    m.frame=0;
    m.hist_idx=0;
    m.temps_courbe=0.0f;
    m.saison=PRINTEMPS;
    m.nb_infectes=0;
    m.vitesse=3;
    m.nb_infectes_lapins=0;
    m.nb_infectes_loups=0;
    m.nb_infectes_vaches=0;
    m.incendie_actif=false;
    m.lv_N=(float)NB_lapins_depart;
    m.lv_P=(float)NB_loups_depart;
    m.nb_fermiers=0;
    for(int i=0;i<MAX_FERMIERS;i++){
        m.fermiers[i].x=0;
        m.fermiers[i].y=0;
        m.fermiers[i].rayon=4;
        m.fermiers[i].actif=false;
    }
    m.nb_meteorites=0;
    for(int i=0;i<MAX_METEORITES;i++){
        m.meteorites[i].actif=false;
        m.meteorites[i].timer=0;
    }
    plot_setSize(m.courbe,-1);
    for(int i=0;i<HIST;i++){
        m.hist_lapins[i]=0;
        m.hist_loups[i]=0;
    }
    for(int i=0;i<5;i++){
        m.nb_lapins_niv[i]=0;
        m.nb_loups_niv[i]=0;
    }
    for(int x=0;x<DIMW;x++){
        for(int y=0;y<DIMH;y++){
            if(chance(65)){
                m.grille[x][y]=herbe;
            }else{
                m.grille[x][y]=vide;
            }
            m.timer_grille[x][y]=0;
        }
    }
    for(int i=0;i<NB_lapins_depart;i++){
        ajouter_lapin(m,hasard(DIMW),hasard(DIMH));
    }
    for(int i=0;i<NB_loups_depart;i++){
        ajouter_loup(m,hasard(DIMW),hasard(DIMH));
    }
    for(int i=0;i<NB_vaches_depart;i++){
        ajouter_vache(m,hasard(DIMW),hasard(DIMH));
    }
}

int taux_herbe(int saison){
    if(saison==ETE){
        return 8;
    }
    if(saison==AUTOMNE){
        return 3;
    }
    if(saison==HIVER){
        return 0;
    }
    return proba_Herbe_pousse;
}

void pousser_herbe(Monde &m){
    int taux=taux_herbe(m.saison);
    for(int x=0;x<DIMW;x++){
        for(int y=0;y<DIMH;y++){
            if(m.grille[x][y]==vide && chance(taux)){
                m.grille[x][y]=herbe;
            }
            if(m.grille[x][y]==brule){
                m.timer_grille[x][y]++;
                if(m.timer_grille[x][y]>Meteorite_duree_brule){
                    m.grille[x][y]=vide;
                    m.timer_grille[x][y]=0;
                }
            }
            if(m.grille[x][y]==cendre){
                m.timer_grille[x][y]++;
                if(m.timer_grille[x][y]>duree_cendre){
                    m.grille[x][y]=vide;
                    m.timer_grille[x][y]=0;
                }
            }
        }
    }
}

bool loup_voit_case(Animal &loup,int x,int y,int saison){
    int rayon;
    if(loup.niveau>=2){
        rayon=2;
    }else{
        rayon=1;
    }
    if(saison==HIVER){
        rayon++;
    }
    return valabs(loup.x-x)<=rayon && valabs(loup.y-y)<=rayon;
}

bool lapin_vu_par_loup(Monde &m,int i){
    for(int j=0;j<m.nb_loups;j++){
        if(m.loups[j].vivant && loup_voit_case(m.loups[j],m.lapins[i].x,m.lapins[i].y,m.saison)){
            return true;
        }
    }
    return false;
}

void direction_vers_herbe(Monde &m,int x,int y,int rayon,int &dx,int &dy){
    dx=0;
    dy=0;
    int meilleure=999;
    for(int cx=-rayon;cx<=rayon;cx++){
        for(int cy=-rayon;cy<=rayon;cy++){
            int nx=x+cx;
            int ny=y+cy;
            if(nx>=0 && nx<DIMW && ny>=0 && ny<DIMH && m.grille[nx][ny]==herbe){
                int dist=valabs(cx)+valabs(cy);
                if(dist<meilleure || (dist==meilleure && chance(50))){
                    meilleure=dist;
                    dx=cx;
                    dy=cy;
                }
            }
        }
    }
    if(dx>1) dx=1;
    if(dx<-1) dx=-1;
    if(dy>1) dy=1;
    if(dy<-1) dy=-1;
}

void direction_fuite_aleatoire(Monde &m,int x,int y,int &dx,int &dy){
    dx=0;
    dy=0;
    int tentatives=0;
    while(tentatives<8){
        int tdx=hasard(3)-1;
        int tdy=hasard(3)-1;
        if(tdx==0 && tdy==0){
            tentatives++;
            continue;
        }
        int nx=x+tdx;
        int ny=y+tdy;
        if(nx<0 || nx>=DIMW || ny<0 || ny>=DIMH){
            tentatives++;
            continue;
        }
        bool loup_dans_direction=false;
        for(int i=0;i<m.nb_loups;i++){
            if(!m.loups[i].vivant){
                continue;
            }
            int d=valabs(m.loups[i].x-nx)+valabs(m.loups[i].y-ny);
            if(d<=1){
                loup_dans_direction=true;
                break;
            }
        }
        if(!loup_dans_direction){
            dx=tdx;
            dy=tdy;
            return;
        }
        tentatives++;
    }
    dx=hasard(3)-1;
    dy=hasard(3)-1;
}

void direction_fuite_intelligente(Monde &m,int x,int y,int &dx,int &dy){
    dx=0;
    dy=0;
    int meilleure=-1;
    for(int cx=-1;cx<=1;cx++){
        for(int cy=-1;cy<=1;cy++){
            if(cx==0 && cy==0){
                continue;
            }
            int nx=x+cx;
            int ny=y+cy;
            if(nx<0 || nx>=DIMW || ny<0 || ny>=DIMH){
                continue;
            }
            int dist_min=9999;
            for(int i=0;i<m.nb_loups;i++){
                if(!m.loups[i].vivant){
                    continue;
                }
                int d=valabs(m.loups[i].x-nx)+valabs(m.loups[i].y-ny);
                if(d<dist_min){
                    dist_min=d;
                }
            }
            if(dist_min>meilleure || (dist_min==meilleure && chance(50))){
                meilleure=dist_min;
                dx=cx;
                dy=cy;
            }
        }
    }
}

void direction_vers_lapin(Monde &m,int x,int y,int rayon,int &dx,int &dy){
    dx=0;
    dy=0;
    int meilleure=999;
    for(int i=0;i<m.nb_lapins;i++){
        if(!m.lapins[i].vivant){
            continue;
        }
        int cx=m.lapins[i].x-x;
        int cy=m.lapins[i].y-y;
        int dist=valabs(cx)+valabs(cy);
        if(dist<=rayon){
            if(dist<meilleure || (dist==meilleure && chance(50))){
                meilleure=dist;
                dx=cx;
                dy=cy;
            }
        }
    }
    if(dx>1) dx=1;
    if(dx<-1) dx=-1;
    if(dy>1) dy=1;
    if(dy<-1) dy=-1;
}

void direction_vers_vache(Monde &m,int x,int y,int rayon,int &dx,int &dy){
    dx=0;
    dy=0;
    int meilleure=999;
    for(int i=0;i<m.nb_vaches;i++){
        if(!m.vaches[i].vivant){
            continue;
        }
        int cx=m.vaches[i].x-x;
        int cy=m.vaches[i].y-y;
        int dist=valabs(cx)+valabs(cy);
        if(dist<=rayon){
            if(dist<meilleure || (dist==meilleure && chance(50))){
                meilleure=dist;
                dx=cx;
                dy=cy;
            }
        }
    }
    if(dx>1) dx=1;
    if(dx<-1) dx=-1;
    if(dy>1) dy=1;
    if(dy<-1) dy=-1;
}

void direction_anticipee(Monde &m,int x,int y,int &dx,int &dy){
    dx=0;
    dy=0;
    int meilleure=999;
    for(int i=0;i<m.nb_lapins;i++){
        if(!m.lapins[i].vivant){
            continue;
        }
        int dist=valabs(m.lapins[i].x-x)+valabs(m.lapins[i].y-y);
        if(dist<=4 && dist<meilleure){
            meilleure=dist;
            int vx=m.lapins[i].x-m.lapins[i].px_avant;
            int vy=m.lapins[i].y-m.lapins[i].py_avant;
            int cx=(m.lapins[i].x+vx)-x;
            int cy=(m.lapins[i].y+vy)-y;
            if(cx>0){
                dx=1;
            }else if(cx<0){
                dx=-1;
            }else{
                dx=0;
            }
            if(cy>0){
                dy=1;
            }else if(cy<0){
                dy=-1;
            }else{
                dy=0;
            }
        }
    }
    if(dx==0 && dy==0){
        direction_vers_lapin(m,x,y,4,dx,dy);
    }
}

int compter_lapins_zone(Monde &m,int x,int y,int rayon){
    int n=0;
    for(int i=0;i<m.nb_lapins;i++){
        if(m.lapins[i].vivant &&
           valabs(m.lapins[i].x-x)<=rayon &&
           valabs(m.lapins[i].y-y)<=rayon){
            n++;
        }
    }
    return n;
}

void case_voisine_libre(Monde &m,int x,int y,int &nx,int &ny){
    nx=x;
    ny=y;
    for(int cx=-1;cx<=1;cx++){
        for(int cy=-1;cy<=1;cy++){
            if(cx==0 && cy==0){
                continue;
            }
            int tx=x+cx;
            int ty=y+cy;
            if(tx>=0 && tx<DIMW && ty>=0 && ty<DIMH){
                bool occupe=false;
                for(int k=0;k<m.nb_lapins;k++){
                    if(m.lapins[k].vivant && m.lapins[k].x==tx && m.lapins[k].y==ty){
                        occupe=true;
                    }
                }
                if(!occupe){
                    nx=tx;
                    ny=ty;
                    return;
                }
            }
        }
    }
}

void monter_niveau_lapin(Animal &a){
    if(a.niveau==1 && a.nb_echappements>=ECH_niv2_lapin){
        a.niveau=2;
    }
    if(a.niveau==2 && a.nb_echappements>=ECH_niv3_lapin){
        a.niveau=3;
    }
    if(a.niveau==3 && a.nb_echappements>=ECH_niv4_lapin){
        a.niveau=4;
    }
}

void monter_niveau_loup(Monde &m,int i){
    Animal &a=m.loups[i];
    if(a.niveau==1 && a.nb_repas>=Repas_niv2_loup){
        a.niveau=2;
    }
    if(a.niveau==2 && a.nb_repas>=Repas_niv3_loup){
        a.niveau=3;
    }
    if(a.niveau==3 && !a.en_meute){
        int voisins=0;
        for(int j=0;j<m.nb_loups;j++){
            if(j==i || !m.loups[j].vivant){
                continue;
            }
            if(m.loups[j].niveau>=3){
                int dist=valabs(m.loups[j].x-a.x)+valabs(m.loups[j].y-a.y);
                if(dist<=3){
                    voisins++;
                }
            }
        }
        if(voisins>=2){
            a.niveau=4;
            a.en_meute=true;
        }
    }
}

void verifier_meutes(Monde &m){
    for(int i=0;i<m.nb_loups;i++){
        if(!m.loups[i].vivant || !m.loups[i].en_meute){
            continue;
        }
        int voisins=0;
        for(int j=0;j<m.nb_loups;j++){
            if(j==i || !m.loups[j].vivant){
                continue;
            }
            if(m.loups[j].niveau>=3){
                int dist=valabs(m.loups[j].x-m.loups[i].x)+valabs(m.loups[j].y-m.loups[i].y);
                if(dist<=3){
                    voisins++;
                }
            }
        }
        if(voisins<2){
            m.loups[i].niveau=3;
            m.loups[i].en_meute=false;
        }
    }
}

bool dans_rayon_fermier(Monde &m,int x,int y){
    for(int i=0;i<m.nb_fermiers;i++){
        if(!m.fermiers[i].actif){
            continue;
        }
        int dist=valabs(m.fermiers[i].x-x)+valabs(m.fermiers[i].y-y);
        if(dist<=m.fermiers[i].rayon){
            return true;
        }
    }
    return false;
}

void direction_fuite_fermier(Monde &m,int x,int y,int &dx,int &dy){
    dx=0;
    dy=0;
    int dist_min=9999;
    int fx=x;
    int fy=y;
    for(int i=0;i<m.nb_fermiers;i++){
        if(!m.fermiers[i].actif){
            continue;
        }
        int dist=valabs(m.fermiers[i].x-x)+valabs(m.fermiers[i].y-y);
        if(dist<dist_min){
            dist_min=dist;
            fx=m.fermiers[i].x;
            fy=m.fermiers[i].y;
        }
    }
    if(x<fx){
        dx=-1;
    }else if(x>fx){
        dx=1;
    }
    if(y<fy){
        dy=-1;
    }else if(y>fy){
        dy=1;
    }
    if(dx==0 && dy==0){
        dx=hasard(3)-1;
        dy=hasard(3)-1;
    }
}

void lancer_meteorite(Monde &m,int cx,int cy){
    if(m.nb_meteorites>=MAX_METEORITES){
        return;
    }
    Meteorite &met=m.meteorites[m.nb_meteorites];
    met.cx=cx;
    met.cy=cy;
    met.timer=0;
    met.actif=true;
    m.nb_meteorites++;
}

void repousser_animal(Animal &a,int cx,int cy,int force){
    int dx=a.x-cx;
    int dy=a.y-cy;
    if(dx>0){
        dx=force;
    }else if(dx<0){
        dx=-force;
    }
    if(dy>0){
        dy=force;
    }else if(dy<0){
        dy=-force;
    }
    if(dx==0 && dy==0){
        dx=(rand()%3-1)*force;
        dy=(rand()%3-1)*force;
    }
    int nx=a.x+dx;
    int ny=a.y+dy;
    if(nx<0) nx=0;
    if(nx>=DIMW) nx=DIMW-1;
    if(ny<0) ny=0;
    if(ny>=DIMH) ny=DIMH-1;
    a.x=nx;
    a.y=ny;
    a.energie-=15;
}

void update_meteorites(Monde &m){
    for(int k=0;k<m.nb_meteorites;k++){
        Meteorite &met=m.meteorites[k];
        if(!met.actif){
            continue;
        }
        met.timer++;
        if(met.timer==1){
            for(int x=0;x<DIMW;x++){
                for(int y=0;y<DIMH;y++){
                    int dc=dist_carre(x,y,met.cx,met.cy);
                    if(dc<=Meteorite_rayon_cratere*Meteorite_rayon_cratere){
                        m.grille[x][y]=brule;
                        m.timer_grille[x][y]=0;
                    }
                }
            }
            for(int i=0;i<m.nb_lapins;i++){
                if(!m.lapins[i].vivant){
                    continue;
                }
                if(dist_carre(m.lapins[i].x,m.lapins[i].y,met.cx,met.cy)<=Meteorite_rayon_cratere*Meteorite_rayon_cratere){
                    m.lapins[i].vivant=false;
                }
            }
            for(int i=0;i<m.nb_loups;i++){
                if(!m.loups[i].vivant){
                    continue;
                }
                if(dist_carre(m.loups[i].x,m.loups[i].y,met.cx,met.cy)<=Meteorite_rayon_cratere*Meteorite_rayon_cratere){
                    m.loups[i].vivant=false;
                }
            }
            for(int i=0;i<m.nb_vaches;i++){
                if(!m.vaches[i].vivant){
                    continue;
                }
                if(dist_carre(m.vaches[i].x,m.vaches[i].y,met.cx,met.cy)<=Meteorite_rayon_cratere*Meteorite_rayon_cratere){
                    m.vaches[i].vivant=false;
                }
            }
        }
        if(met.timer>=2 && met.timer<=Meteorite_duree_onde){
            int rayon_actuel=Meteorite_rayon_cratere+(met.timer*(Meteorite_rayon_onde-Meteorite_rayon_cratere))/Meteorite_duree_onde;
            int r2=rayon_actuel*rayon_actuel;
            for(int i=0;i<m.nb_lapins;i++){
                if(!m.lapins[i].vivant){
                    continue;
                }
                int dc=dist_carre(m.lapins[i].x,m.lapins[i].y,met.cx,met.cy);
                if(dc<=r2 && dc>Meteorite_rayon_cratere*Meteorite_rayon_cratere){
                    repousser_animal(m.lapins[i],met.cx,met.cy,2);
                }
            }
            for(int i=0;i<m.nb_loups;i++){
                if(!m.loups[i].vivant){
                    continue;
                }
                int dc=dist_carre(m.loups[i].x,m.loups[i].y,met.cx,met.cy);
                if(dc<=r2 && dc>Meteorite_rayon_cratere*Meteorite_rayon_cratere){
                    repousser_animal(m.loups[i],met.cx,met.cy,2);
                }
            }
            for(int i=0;i<m.nb_vaches;i++){
                if(!m.vaches[i].vivant){
                    continue;
                }
                int dc=dist_carre(m.vaches[i].x,m.vaches[i].y,met.cx,met.cy);
                if(dc<=r2 && dc>Meteorite_rayon_cratere*Meteorite_rayon_cratere){
                    repousser_animal(m.vaches[i],met.cx,met.cy,2);
                }
            }
            for(int x=0;x<DIMW;x++){
                for(int y=0;y<DIMH;y++){
                    int dc=dist_carre(x,y,met.cx,met.cy);
                    if(dc<=r2 && m.grille[x][y]==herbe && chance(30)){
                        m.grille[x][y]=vide;
                    }
                }
            }
        }
        if(met.timer>Meteorite_duree_impact+Meteorite_duree_onde){
            met.actif=false;
        }
    }
}

void update_incendie(Monde &m){
    int nouvelle_grille[DIMW][DIMH];
    for(int x=0;x<DIMW;x++){
        for(int y=0;y<DIMH;y++){
            nouvelle_grille[x][y]=m.grille[x][y];
        }
    }
    bool feu_present=false;
    for(int x=0;x<DIMW;x++){
        for(int y=0;y<DIMH;y++){
            if(m.grille[x][y]==feu){
                feu_present=true;
                m.timer_grille[x][y]++;
                for(int i=0;i<m.nb_lapins;i++){
                    if(m.lapins[i].vivant && m.lapins[i].x==x && m.lapins[i].y==y){
                        m.lapins[i].vivant=false;
                    }
                }
                for(int i=0;i<m.nb_loups;i++){
                    if(m.loups[i].vivant && m.loups[i].x==x && m.loups[i].y==y){
                        m.loups[i].vivant=false;
                    }
                }
                for(int i=0;i<m.nb_vaches;i++){
                    if(m.vaches[i].vivant && m.vaches[i].x==x && m.vaches[i].y==y){
                        m.vaches[i].vivant=false;
                    }
                }
                if(m.timer_grille[x][y]>duree_feu){
                    nouvelle_grille[x][y]=cendre;
                    m.timer_grille[x][y]=0;
                    continue;
                }
                for(int dx=-1;dx<=1;dx++){
                    for(int dy=-1;dy<=1;dy++){
                        if(dx==0 && dy==0){
                            continue;
                        }
                        int nx=x+dx;
                        int ny=y+dy;
                        if(nx>=0 && nx<DIMW && ny>=0 && ny<DIMH){
                            if(m.grille[nx][ny]==herbe && chance(proba_propagation_feu)){
                                nouvelle_grille[nx][ny]=feu;
                            }
                        }
                    }
                }
                if(m.saison==ETE){
                    for(int dx=-1;dx<=1;dx++){
                        for(int dy=-1;dy<=1;dy++){
                            if(dx==0 && dy==0){
                                continue;
                            }
                            int nx=x+dx;
                            int ny=y+dy;
                            if(nx>=0 && nx<DIMW && ny>=0 && ny<DIMH){
                                if(m.grille[nx][ny]==herbe && chance(proba_propagation_feu)){
                                    nouvelle_grille[nx][ny]=feu;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    for(int x=0;x<DIMW;x++){
        for(int y=0;y<DIMH;y++){
            m.grille[x][y]=nouvelle_grille[x][y];
        }
    }
    m.incendie_actif=feu_present;
}

void demarrer_incendie(Monde &m,int gx,int gy){
    if(gx>=0 && gx<DIMW && gy>=0 && gy<DIMH){
        for(int dx=-1;dx<=1;dx++){
            for(int dy=-1;dy<=1;dy++){
                int nx=gx+dx;
                int ny=gy+dy;
                if(nx>=0 && nx<DIMW && ny>=0 && ny<DIMH){
                    if(m.grille[nx][ny]==herbe || m.grille[nx][ny]==vide){
                        m.grille[nx][ny]=feu;
                        m.timer_grille[nx][ny]=0;
                    }
                }
            }
        }
        m.incendie_actif=true;
    }
}

bool pres_du_feu(Monde &m,int x,int y){
    for(int dx=-2;dx<=2;dx++){
        for(int dy=-2;dy<=2;dy++){
            int nx=x+dx;
            int ny=y+dy;
            if(nx>=0 && nx<DIMW && ny>=0 && ny<DIMH){
                if(m.grille[nx][ny]==feu){
                    return true;
                }
            }
        }
    }
    return false;
}

void direction_fuite_feu(Monde &m,int x,int y,int &dx,int &dy){
    dx=0;
    dy=0;
    int somme_x=0;
    int somme_y=0;
    int nb=0;
    for(int cx=-4;cx<=4;cx++){
        for(int cy=-4;cy<=4;cy++){
            int nx=x+cx;
            int ny=y+cy;
            if(nx>=0 && nx<DIMW && ny>=0 && ny<DIMH){
                if(m.grille[nx][ny]==feu){
                    somme_x+=nx;
                    somme_y+=ny;
                    nb++;
                }
            }
        }
    }
    if(nb>0){
        int centre_x=somme_x/nb;
        int centre_y=somme_y/nb;
        if(x<centre_x){
            dx=-1;
        }else if(x>centre_x){
            dx=1;
        }
        if(y<centre_y){
            dy=-1;
        }else if(y>centre_y){
            dy=1;
        }
    }
    if(dx==0 && dy==0){
        dx=hasard(3)-1;
        dy=hasard(3)-1;
    }
}

void deplacer_lapin(Monde &m,int i){
    Animal &a=m.lapins[i];
    int dx=0;
    int dy=0;
    bool vu_maintenant=lapin_vu_par_loup(m,i);
    if(a.dans_vision_loup && !vu_maintenant){
        a.nb_echappements++;
        monter_niveau_lapin(a);
    }
    a.dans_vision_loup=vu_maintenant;
    if(pres_du_feu(m,a.x,a.y)){
        direction_fuite_feu(m,a.x,a.y,dx,dy);
    }else if(a.niveau==1){
        direction_vers_herbe(m,a.x,a.y,6,dx,dy);
        if(dx==0 && dy==0){
            dx=hasard(3)-1;
            dy=hasard(3)-1;
        }
    }else if(a.niveau==2){
        if(vu_maintenant){
            direction_fuite_aleatoire(m,a.x,a.y,dx,dy);
        }else{
            direction_vers_herbe(m,a.x,a.y,6,dx,dy);
            if(dx==0 && dy==0){
                dx=hasard(3)-1;
                dy=hasard(3)-1;
            }
        }
    }else{
        if(vu_maintenant){
            direction_fuite_intelligente(m,a.x,a.y,dx,dy);
        }else{
            direction_vers_herbe(m,a.x,a.y,6,dx,dy);
            if(dx==0 && dy==0){
                dx=hasard(3)-1;
                dy=hasard(3)-1;
            }
        }
    }
    if(m.frame%3==0){
        a.px_avant=a.x;
        a.py_avant=a.y;
    }
    if(dx!=0 || dy!=0){
        a.dir_x=dx;
        a.dir_y=dy;
    }
    int nx=a.x+dx;
    int ny=a.y+dy;
    if(nx>=0 && nx<DIMW && ny>=0 && ny<DIMH){
        a.x=nx;
        a.y=ny;
    }
}

void deplacer_loup(Monde &m,int i){
    Animal &a=m.loups[i];
    int dx=0;
    int dy=0;
    if(pres_du_feu(m,a.x,a.y)){
        direction_fuite_feu(m,a.x,a.y,dx,dy);
    }else if(dans_rayon_fermier(m,a.x,a.y)){
        direction_fuite_fermier(m,a.x,a.y,dx,dy);
        a.energie-=2;
    }else if(a.niveau==1){
        direction_vers_lapin(m,a.x,a.y,3,dx,dy);
        if(dx==0 && dy==0){
            direction_vers_vache(m,a.x,a.y,3,dx,dy);
        }
        if(dx==0 && dy==0){
            dx=hasard(3)-1;
            dy=hasard(3)-1;
        }
    }else if(a.niveau==2){
        direction_anticipee(m,a.x,a.y,dx,dy);
        if(dx==0 && dy==0){
            direction_vers_vache(m,a.x,a.y,4,dx,dy);
        }
        if(dx==0 && dy==0){
            dx=hasard(3)-1;
            dy=hasard(3)-1;
        }
    }else if(a.niveau==3){
        direction_vers_lapin(m,a.x,a.y,2,dx,dy);
        if(dx==0 && dy==0){
            int meilleure=999;
            for(int j=0;j<m.nb_loups;j++){
                if(j==i || !m.loups[j].vivant){
                    continue;
                }
                if(m.loups[j].niveau!=3){
                    continue;
                }
                int dist=valabs(m.loups[j].x-a.x)+valabs(m.loups[j].y-a.y);
                if(dist<meilleure && dist>1){
                    meilleure=dist;
                    int cx=m.loups[j].x-a.x;
                    int cy=m.loups[j].y-a.y;
                    dx=cx>0 ? 1 : (cx<0 ? -1 : 0);
                    dy=cy>0 ? 1 : (cy<0 ? -1 : 0);
                }
            }
        }
        if(dx==0 && dy==0){
            direction_vers_vache(m,a.x,a.y,3,dx,dy);
        }
        if(dx==0 && dy==0){
            dx=hasard(3)-1;
            dy=hasard(3)-1;
        }
    }else{
        direction_vers_lapin(m,a.x,a.y,6,dx,dy);
        if(dx==0 && dy==0){
            direction_vers_vache(m,a.x,a.y,6,dx,dy);
        }
        if(dx==0 && dy==0){
            dx=hasard(3)-1;
            dy=hasard(3)-1;
        }
    }
    if(m.frame%3==0){
        a.px_avant=a.x;
        a.py_avant=a.y;
    }
    if(dx!=0 || dy!=0){
        a.dir_x=dx;
        a.dir_y=dy;
    }
    int nx=a.x+dx;
    int ny=a.y+dy;
    if(nx>=0 && nx<DIMW && ny>=0 && ny<DIMH){
        a.x=nx;
        a.y=ny;
    }
}

void deplacer_vache(Monde &m,int i){
    Animal &a=m.vaches[i];
    int dx=0;
    int dy=0;
    if(pres_du_feu(m,a.x,a.y)){
        direction_fuite_feu(m,a.x,a.y,dx,dy);
    }else{
        if(m.frame%2!=0){
            return;
        }
        direction_vers_herbe(m,a.x,a.y,4,dx,dy);
        if(dx==0 && dy==0){
            dx=hasard(3)-1;
            dy=hasard(3)-1;
        }
    }
    if(dx!=0 || dy!=0){
        a.dir_x=dx;
        a.dir_y=dy;
    }
    int nx=a.x+dx;
    int ny=a.y+dy;
    if(nx>=0 && nx<DIMW && ny>=0 && ny<DIMH){
        a.x=nx;
        a.y=ny;
    }
}

void manger_herbe(Monde &m,int i){
    int x=m.lapins[i].x;
    int y=m.lapins[i].y;
    if(m.grille[x][y]==herbe){
        m.grille[x][y]=vide;
        m.lapins[i].energie+=Energie_manger_herbe;
        if(m.lapins[i].energie>Energie_max){
            m.lapins[i].energie=Energie_max;
        }
    }
}

void manger_herbe_vache(Monde &m,int i){
    int x=m.vaches[i].x;
    int y=m.vaches[i].y;
    if(m.grille[x][y]==herbe){
        m.grille[x][y]=vide;
        m.vaches[i].energie+=Energie_manger_herbe;
        if(m.vaches[i].energie>Energie_max){
            m.vaches[i].energie=Energie_max;
        }
    }
}

void manger_lapin(Monde &m,int i){
    Animal &loup=m.loups[i];
    int nb_manges=0;
    int max_repas=(loup.niveau>=3)?2:1;
    bool a_mange=false;
    for(int j=0;j<m.nb_lapins && nb_manges<max_repas;j++){
        if(!m.lapins[j].vivant){
            continue;
        }
        int dist=valabs(loup.x-m.lapins[j].x)+valabs(loup.y-m.lapins[j].y);
        if(dist>1){
            continue;
        }
        if(dans_rayon_fermier(m,m.lapins[j].x,m.lapins[j].y)){
            continue;
        }
        int niv_lapin=m.lapins[j].niveau;
        bool peut=false;
        if(loup.niveau==4 && loup.en_meute){
            peut=true;
        }else if(niv_lapin<=loup.niveau){
            peut=true;
        }else if(loup.niveau==3 && niv_lapin<=2){
            peut=true;
        }
        if(peut){
            if(m.lapins[j].etat==INFECTE && loup.etat==SAIN){
                loup.etat=INFECTE;
                loup.timer_maladie=0;
            }
            m.lapins[j].vivant=false;
            loup.energie+=Energie_manger_lapin;
            if(loup.energie>Energie_max){
                loup.energie=Energie_max;
            }
            loup.nb_repas++;
            monter_niveau_loup(m,i);
            nb_manges++;
            a_mange=true;
        }
    }
    for(int j=0;j<m.nb_vaches && nb_manges<max_repas;j++){
        if(!m.vaches[j].vivant){
            continue;
        }
        int dist=valabs(loup.x-m.vaches[j].x)+valabs(loup.y-m.vaches[j].y);
        if(dist>1){
            continue;
        }
        if(dans_rayon_fermier(m,m.vaches[j].x,m.vaches[j].y)){
            continue;
        }
        if(m.vaches[j].etat==INFECTE && loup.etat==SAIN){
            loup.etat=INFECTE;
            loup.timer_maladie=0;
        }
        m.vaches[j].vivant=false;
        loup.energie+=Energie_manger_vache;
        if(loup.energie>Energie_max){
            loup.energie=Energie_max;
        }
        loup.nb_repas++;
        nb_manges++;
        a_mange=true;
    }
    if(a_mange){
        m.loups[i].px_avant=m.loups[i].x;
        m.loups[i].py_avant=m.loups[i].y;
    }
}

void vieillir_lapin(Monde &m,int i){
    m.lapins[i].energie--;
    m.lapins[i].age++;
    m.lapins[i].frames_depuis_repro++;
    if(m.saison==HIVER){
        m.lapins[i].energie--;
    }
    if(m.lapins[i].etat==INFECTE){
        m.lapins[i].energie--;
    }
    if(m.lapins[i].energie<=0 || m.lapins[i].age>Age_max_lapin){
        m.lapins[i].vivant=false;
    }
}

void vieillir_loup(Monde &m,int i){
    m.loups[i].age++;
    m.loups[i].frames_depuis_repro++;
    if(m.frame%2==0){
        m.loups[i].energie--;
    }
    if(m.saison==ETE && m.frame%3==0){
        m.loups[i].energie--;
    }
    if(m.loups[i].etat==INFECTE){
        m.loups[i].energie--;
    }
    if(m.loups[i].energie<=0 || m.loups[i].age>Age_max_loup){
        m.loups[i].vivant=false;
    }
}

void vieillir_vache(Monde &m,int i){
    m.vaches[i].energie--;
    m.vaches[i].age++;
    m.vaches[i].frames_depuis_repro++;
    if(m.saison==HIVER){
        m.vaches[i].energie--;
    }
    if(m.vaches[i].etat==INFECTE){
        m.vaches[i].energie--;
    }
    if(m.vaches[i].energie<=0 || m.vaches[i].age>Age_max_vache){
        m.vaches[i].vivant=false;
    }
}

void reproduire_lapins(Monde &m){
    int delai=Delai_repro_lapin;
    if(m.saison==ETE){
        delai+=20;
    }
    if(m.saison==AUTOMNE){
        delai-=20;
    }
    if(delai<20){
        delai=20;
    }
    for(int i=0;i<m.nb_lapins;i++){
        if(!m.lapins[i].vivant){
            continue;
        }
        if(m.lapins[i].genre!=Femelle){
            continue;
        }
        if(m.lapins[i].energie<50){
            continue;
        }
        if(m.lapins[i].frames_depuis_repro<delai){
            continue;
        }
        if(compter_lapins_zone(m,m.lapins[i].x,m.lapins[i].y,Rayon_densite)>=Densite_max_lapin){
            continue;
        }
        for(int j=0;j<m.nb_lapins;j++){
            if(!m.lapins[j].vivant){
                continue;
            }
            if(m.lapins[j].genre!=Male){
                continue;
            }
            if(m.lapins[j].energie<50){
                continue;
            }
            int dist=valabs(m.lapins[i].x-m.lapins[j].x)+valabs(m.lapins[i].y-m.lapins[j].y);
            if(dist<=2 && chance(proba_repro_lapin)){
                int nx,ny;
                case_voisine_libre(m,m.lapins[i].x,m.lapins[i].y,nx,ny);
                ajouter_lapin(m,nx,ny);
                m.lapins[i].energie-=20;
                m.lapins[i].frames_depuis_repro=0;
                break;
            }
        }
    }
}

void reproduire_loups(Monde &m){
    for(int i=0;i<m.nb_loups;i++){
        if(!m.loups[i].vivant){
            continue;
        }
        if(m.loups[i].genre!=Femelle){
            continue;
        }
        if(m.loups[i].energie<75){
            continue;
        }
        if(m.loups[i].frames_depuis_repro<Delai_repro_loup){
            continue;
        }
        for(int j=0;j<m.nb_loups;j++){
            if(!m.loups[j].vivant){
                continue;
            }
            if(m.loups[j].genre!=Male){
                continue;
            }
            if(m.loups[j].energie<75){
                continue;
            }
            int dist=valabs(m.loups[i].x-m.loups[j].x)+valabs(m.loups[i].y-m.loups[j].y);
            if(dist<=2 && chance(proba_repro_loup)){
                int nx,ny;
                case_voisine_libre(m,m.loups[i].x,m.loups[i].y,nx,ny);
                ajouter_loup(m,nx,ny);
                m.loups[i].energie-=30;
                m.loups[i].frames_depuis_repro=0;
                break;
            }
        }
    }
}

void reproduire_vaches(Monde &m){
    for(int i=0;i<m.nb_vaches;i++){
        if(!m.vaches[i].vivant){
            continue;
        }
        if(m.vaches[i].genre!=Femelle){
            continue;
        }
        if(m.vaches[i].energie<70){
            continue;
        }
        if(m.vaches[i].frames_depuis_repro<Delai_repro_vaches){
            continue;
        }
        for(int j=0;j<m.nb_vaches;j++){
            if(!m.vaches[j].vivant){
                continue;
            }
            if(m.vaches[j].genre!=Male){
                continue;
            }
            if(m.vaches[j].energie<70){
                continue;
            }
            int dist=valabs(m.vaches[i].x-m.vaches[j].x)+valabs(m.vaches[i].y-m.vaches[j].y);
            if(dist<=2 && chance(proba_repro_vache)){
                int nx,ny;
                case_voisine_libre(m,m.vaches[i].x,m.vaches[i].y,nx,ny);
                ajouter_vache(m,nx,ny);
                m.vaches[i].energie-=25;
                m.vaches[i].frames_depuis_repro=0;
                break;
            }
        }
    }
}

void compacter_lapins(Monde &m){
    int w=0;
    for(int i=0;i<m.nb_lapins;i++){
        if(m.lapins[i].vivant){
            m.lapins[w++]=m.lapins[i];
        }
    }
    m.nb_lapins=w;
}

void compacter_loups(Monde &m){
    int w=0;
    for(int i=0;i<m.nb_loups;i++){
        if(m.loups[i].vivant){
            m.loups[w++]=m.loups[i];
        }
    }
    m.nb_loups=w;
}

void compacter_vaches(Monde &m){
    int w=0;
    for(int i=0;i<m.nb_vaches;i++){
        if(m.vaches[i].vivant){
            m.vaches[w++]=m.vaches[i];
        }
    }
    m.nb_vaches=w;
}

void propager_maladie(Monde &m){
    for(int i=0;i<m.nb_lapins;i++){
        if(m.lapins[i].vivant && m.lapins[i].etat==SAIN && chance(Proba_infection_ini)){
            if(m.nb_lapins>20){
                m.lapins[i].etat=INFECTE;
                m.lapins[i].timer_maladie=0;
            }
        }
    }
    for(int i=0;i<m.nb_lapins;i++){
        if(!m.lapins[i].vivant || m.lapins[i].etat!=INFECTE){
            continue;
        }
        m.lapins[i].timer_maladie++;
        if(m.lapins[i].timer_maladie>DUREE_Infection){
            m.lapins[i].etat=RETABLI;
            m.lapins[i].timer_maladie=0;
            continue;
        }
        for(int j=0;j<m.nb_lapins;j++){
            if(i==j || !m.lapins[j].vivant){
                continue;
            }
            if(m.lapins[j].etat!=SAIN){
                continue;
            }
            int dist=valabs(m.lapins[i].x-m.lapins[j].x)+valabs(m.lapins[i].y-m.lapins[j].y);
            if(dist<=2 && chance(Proba_contagion)){
                m.lapins[j].etat=INFECTE;
            }
        }
    }
    for(int i=0;i<m.nb_loups;i++){
        if(!m.loups[i].vivant || m.loups[i].etat!=INFECTE){
            continue;
        }
        m.loups[i].timer_maladie++;
        if(m.loups[i].timer_maladie>DUREE_Infection){
            m.loups[i].etat=RETABLI;
            m.loups[i].timer_maladie=0;
            continue;
        }
        for(int j=0;j<m.nb_loups;j++){
            if(i==j || !m.loups[j].vivant){
                continue;
            }
            if(m.loups[j].etat!=SAIN){
                continue;
            }
            int dist=valabs(m.loups[i].x-m.loups[j].x)+valabs(m.loups[i].y-m.loups[j].y);
            if(dist<=2 && chance(Proba_contagion)){
                m.loups[j].etat=INFECTE;
            }
        }
    }
    for(int i=0;i<m.nb_vaches;i++){
        if(!m.vaches[i].vivant || m.vaches[i].etat!=INFECTE){
            continue;
        }
        m.vaches[i].timer_maladie++;
        if(m.vaches[i].timer_maladie>DUREE_Infection){
            m.vaches[i].etat=RETABLI;
            m.vaches[i].timer_maladie=0;
            continue;
        }
    }
    for(int i=0;i<m.nb_lapins;i++){
        if(m.lapins[i].etat==RETABLI){
            m.lapins[i].timer_maladie++;
            if(m.lapins[i].timer_maladie>DUREE_Immunite){
                m.lapins[i].etat=SAIN;
            }
        }
    }
    for(int i=0;i<m.nb_loups;i++){
        if(m.loups[i].etat==RETABLI){
            m.loups[i].timer_maladie++;
            if(m.loups[i].timer_maladie>DUREE_Immunite){
                m.loups[i].etat=SAIN;
            }
        }
    }
    for(int i=0;i<m.nb_vaches;i++){
        if(m.vaches[i].etat==RETABLI){
            m.vaches[i].timer_maladie++;
            if(m.vaches[i].timer_maladie>DUREE_Immunite){
                m.vaches[i].etat=SAIN;
            }
        }
    }
}

void maj_niveaux(Monde &m){
    for(int i=0;i<5;i++){
        m.nb_lapins_niv[i]=0;
        m.nb_loups_niv[i]=0;
    }
    for(int i=0;i<m.nb_lapins;i++){
        if(m.lapins[i].vivant){
            m.nb_lapins_niv[m.lapins[i].niveau]++;
        }
    }
    for(int i=0;i<m.nb_loups;i++){
        if(m.loups[i].vivant){
            m.nb_loups_niv[m.loups[i].niveau]++;
        }
    }
}

void maj_statistiques(Monde &m){
    int infectes=0;
    int infectes_lapins=0;
    int infectes_loups=0;
    int infectes_vaches=0;
    for(int i=0;i<m.nb_lapins;i++){
        if(m.lapins[i].vivant && m.lapins[i].etat==INFECTE){
            infectes++;
            infectes_lapins++;
        }
    }
    for(int i=0;i<m.nb_loups;i++){
        if(m.loups[i].vivant && m.loups[i].etat==INFECTE){
            infectes++;
            infectes_loups++;
        }
    }
    for(int i=0;i<m.nb_vaches;i++){
        if(m.vaches[i].vivant && m.vaches[i].etat==INFECTE){
            infectes++;
            infectes_vaches++;
        }
    }
    m.nb_infectes=infectes;
    m.nb_infectes_lapins=infectes_lapins;
    m.nb_infectes_loups=infectes_loups;
    m.nb_infectes_vaches=infectes_vaches;
}

void maj_historique(Monde &m){
    if(m.frame%10==0){
        m.hist_lapins[m.hist_idx%HIST]=m.nb_lapins;
        m.hist_loups[m.hist_idx%HIST]=m.nb_loups;
        m.hist_idx++;
        m.temps_courbe+=0.1f;
        plot_add(m.courbe,m.temps_courbe,(float)m.nb_lapins,0);
        plot_add(m.courbe,m.temps_courbe,(float)m.nb_loups,1);
        float alpha=0.02f;
        float beta=0.005f;
        float delta=0.002f;
        float gamma=0.03f;
        float dt=0.5f;
        float dN=alpha*m.lv_N-beta*m.lv_N*m.lv_P;
        float dP=delta*m.lv_N*m.lv_P-gamma*m.lv_P;
        m.lv_N+=dN*dt;
        m.lv_P+=dP*dt;
        if(m.lv_N<0){
            m.lv_N=0;
        }
        if(m.lv_P<0){
            m.lv_P=0;
        }
        plot_add(m.courbe,m.temps_courbe,m.lv_N,2);
        plot_add(m.courbe,m.temps_courbe,m.lv_P,3);
        plot_add(m.courbe,m.temps_courbe,(float)m.nb_infectes,4);
    }
}

void update(Monde &m){
    pousser_herbe(m);
    for(int i=0;i<m.nb_lapins;i++){
        if(m.lapins[i].vivant){
            vieillir_lapin(m,i);
            deplacer_lapin(m,i);
            manger_herbe(m,i);
        }
    }
    reproduire_lapins(m);
    for(int i=0;i<m.nb_loups;i++){
        if(m.loups[i].vivant){
            vieillir_loup(m,i);
            deplacer_loup(m,i);
            manger_lapin(m,i);
        }
    }
    reproduire_loups(m);
    verifier_meutes(m);
    for(int i=0;i<m.nb_vaches;i++){
        if(m.vaches[i].vivant){
            vieillir_vache(m,i);
            deplacer_vache(m,i);
            manger_herbe_vache(m,i);
        }
    }
    reproduire_vaches(m);
    update_meteorites(m);
    update_incendie(m);
    propager_maladie(m);
    compacter_lapins(m);
    compacter_loups(m);
    compacter_vaches(m);
    maj_niveaux(m);
    maj_statistiques(m);
    maj_historique(m);
    m.frame++;
}

void dessiner_grille(Monde &m){
    for(int x=0;x<DIMW;x++){
        for(int y=0;y<DIMH;y++){
            int etat=m.grille[x][y];
            if(etat==herbe){
                if(m.saison==PRINTEMPS){
                    color(120,200,80);
                }else if(m.saison==ETE){
                    color(80,180,40);
                }else if(m.saison==AUTOMNE){
                    color(180,160,60);
                }else{
                    color(200,210,220);
                }
            }else if(etat==brule){
                color(60,30,10);
            }else if(etat==feu){
                if(m.frame%4<2){
                    color(255,100,0);
                }else{
                    color(255,200,0);
                }
            }else if(etat==cendre){
                color(100,100,90);
            }else{
                if(m.saison==HIVER){
                    color(230,235,240);
                }else{
                    color(240,235,220);
                }
            }
            rectangleFill(x*CELL,y*CELL,x*CELL+CELL-1,y*CELL+CELL-1);
        }
    }
}

void dessiner_meteorites(Monde &m){
    for(int k=0;k<m.nb_meteorites;k++){
        Meteorite &met=m.meteorites[k];
        if(!met.actif){
            continue;
        }
        int px=met.cx*CELL+CELL/2;
        int py=met.cy*CELL+CELL/2;
        if(met.timer<=Meteorite_duree_impact){
            int rayon_anim=(met.timer*Meteorite_rayon_cratere*CELL)/Meteorite_duree_impact;
            if(rayon_anim<3){
                rayon_anim=3;
            }
            color(255,150,0);
            circleFill(px,py,rayon_anim+4);
            color(255,50,0);
            circleFill(px,py,rayon_anim);
            if(met.timer<10){
                color(255,255,200);
                circleFill(px,py,rayon_anim/2);
            }
        }
        if(met.timer>Meteorite_duree_impact && met.timer<=Meteorite_duree_impact+Meteorite_duree_onde){
            int t=met.timer-Meteorite_duree_impact;
            int rayon_onde=Meteorite_rayon_cratere*CELL+(t*(Meteorite_rayon_onde-Meteorite_rayon_cratere)*CELL)/Meteorite_duree_onde;
            color(255,255,255);
            circle(px,py,rayon_onde);
            if(rayon_onde>2){
                circle(px,py,rayon_onde-2);
            }
        }
    }
}

void dessiner_animal(int gx,int gy,int dx,int dy,int taille,int r,int g,int b){
    color(r,g,b);
    int px=gx*CELL+CELL/2;
    int py=gy*CELL+CELL/2;
    circleFill(px,py,taille);
    color(255,255,255);
    line(px,py,px+dx*taille,py+dy*taille);
}

void dessiner_maladie(int gx,int gy,int etat){
    int px=gx*CELL+CELL/2;
    int py=gy*CELL+CELL/2;
    if(etat==INFECTE){
        color(255,255,0);
        circle(px,py,8);
    }else if(etat==RETABLI){
        color(0,255,200);
        circle(px,py,8);
    }
}

void couleur_lapin(int niveau,int &r,int &g,int &b){
    if(niveau==1){
        r=80;g=180;b=255;
    }else if(niveau==2){
        r=50;g=220;b=200;
    }else if(niveau==3){
        r=30;g=255;b=120;
    }else{
        r=200;g=255;b=50;
    }
}

void couleur_loup(int niveau,bool meute,int &r,int &g,int &b){
    if(niveau==1){
        r=220;g=50;b=50;
    }else if(niveau==2){
        r=180;g=80;b=80;
    }else if(niveau==3){
        r=150;g=150;b=150;
    }else if(meute){
        r=50;g=50;b=50;
    }else{
        r=160;g=120;b=120;
    }
}

void dessiner_lapins(Monde &m){
    for(int i=0;i<m.nb_lapins;i++){
        if(!m.lapins[i].vivant){
            continue;
        }
        int dx=m.lapins[i].dir_x;
        int dy=m.lapins[i].dir_y;
        if(dx==0 && dy==0){
            dx=1;
        }
        int r,g,b;
        couleur_lapin(m.lapins[i].niveau,r,g,b);
        dessiner_animal(m.lapins[i].x,m.lapins[i].y,dx,dy,4,r,g,b);
        dessiner_maladie(m.lapins[i].x,m.lapins[i].y,m.lapins[i].etat);
    }
}

void dessiner_loups(Monde &m){
    for(int i=0;i<m.nb_loups;i++){
        if(!m.loups[i].vivant){
            continue;
        }
        int dx=m.loups[i].dir_x;
        int dy=m.loups[i].dir_y;
        if(dx==0 && dy==0){
            dx=1;
        }
        int r,g,b;
        couleur_loup(m.loups[i].niveau,m.loups[i].en_meute,r,g,b);
        dessiner_animal(m.loups[i].x,m.loups[i].y,dx,dy,6,r,g,b);
        dessiner_maladie(m.loups[i].x,m.loups[i].y,m.loups[i].etat);
    }
}

void dessiner_vaches(Monde &m){
    for(int i=0;i<m.nb_vaches;i++){
        if(!m.vaches[i].vivant){
            continue;
        }
        int dx=m.vaches[i].dir_x;
        int dy=m.vaches[i].dir_y;
        if(dx==0 && dy==0){
            dx=1;
        }
        dessiner_animal(m.vaches[i].x,m.vaches[i].y,dx,dy,5,160,90,10);
        dessiner_maladie(m.vaches[i].x,m.vaches[i].y,m.vaches[i].etat);
    }
}

void dessiner_fermier(Monde &m){
    for(int i=0;i<m.nb_fermiers;i++){
        if(!m.fermiers[i].actif){
            continue;
        }
        int px=m.fermiers[i].x*CELL+CELL/2;
        int py=m.fermiers[i].y*CELL+CELL/2;
        color(100,100,255);
        circle(px,py,m.fermiers[i].rayon*CELL);
        color(255,200,50);
        circleFill(px,py,7);
        color(0,0,0);
        circleFill(px,py-2,2);
    }
}

bool bouton(int x1,int y1,int x2,int y2,const char texte[],bool actif,int mx,int my,bool clic){
    if(actif){
        color(80,150,80);
    }else{
        color(60,60,70);
    }
    rectangleFill(x1,y1,x2,y2);
    color(255,255,255);
    rectangle(x1,y1,x2,y2);
    print(x1+6,y1+5,texte);
    if(clic && mx>=x1 && mx<=x2 && my>=y1 && my<=y2){
        return true;
    }
    return false;
}

void dessiner_panel(Monde &m,int mx,int my,bool clic){
    int px=WIN_W-PANEL_W;
    color(0,0,0);
    rectangleFill(px,0,WIN_W,WIN_H);
    plot_draw(m.courbe,px+8,8,WIN_W-8,180,true);
    int ly=185;
    color(80,180,255);
    print(px+12,ly,"-- Lapins sim.");
    color(220,50,50);
    print(px+145,ly,"-- Loups sim.");
    ly-=14;
    color(80,255,180);
    print(px+12,ly,"-- Lapins th.");
    color(255,150,50);
    print(px+145,ly,"-- Loups th.");
    ly-=14;
    color(255,255,0);
    print(px+12,ly,"-- Infectes");
    int y=WIN_H-20;
    int pas=16;
    color(150,150,150);
    print(px+12,y,"Lapins :");
    print(px+90,y,m.nb_lapins);
    y-=pas;
    print(px+12,y,"Loups  :");
    print(px+90,y,m.nb_loups);
    y-=pas;
    print(px+12,y,"Vaches :");
    print(px+90,y,m.nb_vaches);
    y-=pas;
    print(px+12,y,"Cycle  :");
    print(px+90,y,m.frame);
    y-=pas;
    color(200,200,100);
    if(m.saison==PRINTEMPS){
        print(px+12,y,"Saison : Printemps");
    }else if(m.saison==ETE){
        print(px+12,y,"Saison : Ete");
    }else if(m.saison==AUTOMNE){
        print(px+12,y,"Saison : Automne");
    }else{
        print(px+12,y,"Saison : Hiver");
    }
    y-=pas;
    color(255,127,0);
    print(px+12,y,"Infectes total :");
    print(px+110,y,m.nb_infectes);
    y-=pas;
    color(255,255,0);
    print(px+12,y,"Inf lap :");
    print(px+110,y,m.nb_infectes_lapins);
    y-=pas;
    print(px+12,y,"Inf lou :");
    print(px+110,y,m.nb_infectes_loups);
    y-=pas;
    print(px+12,y,"Inf vac :");
    print(px+110,y,m.nb_infectes_vaches);
    y-=pas;
    color(255,0,50);
    print(px+12,y,"Vitesse :");
    print(px+110,y,m.vitesse);
    y-=pas+4;
    int r,g,b;
    color(150,150,150);
    print(px+12,y,"Lapins par niveau :");
    y-=pas;
    for(int n=1;n<=4;n++){
        couleur_lapin(n,r,g,b);
        color(r,g,b);
        print(px+12,y,"Niv");
        print(px+42,y,n);
        print(px+55,y,":");
        print(px+65,y,m.nb_lapins_niv[n]);
        y-=pas;
    }
    color(240,50,50);
    print(px+12,y,"Loups par niveau :");
    y-=pas;
    for(int n=1;n<=4;n++){
        couleur_loup(n,n==4,r,g,b);
        color(r,g,b);
        print(px+12,y,"Niv");
        print(px+42,y,n);
        print(px+55,y,":");
        print(px+65,y,m.nb_loups_niv[n]);
        y-=pas;
    }
    y-=4;
    color(200,200,200);
    print(px+12,y,"Saisons :");
    y-=pas+2;
    int bw=60;
    int bh=18;
    if(bouton(px+12,y,px+12+bw,y+bh,"Printemps",m.saison==PRINTEMPS,mx,my,clic)){
        m.saison=PRINTEMPS;
    }
    if(bouton(px+12+bw+4,y,px+12+2*bw+4,y+bh,"Ete",m.saison==ETE,mx,my,clic)){
        m.saison=ETE;
    }
    y-=bh+4;
    if(bouton(px+12,y,px+12+bw,y+bh,"Autonome",m.saison==AUTOMNE,mx,my,clic)){
        m.saison=AUTOMNE;
    }
    if(bouton(px+12+bw+4,y,px+12+2*bw+4,y+bh,"Hiver",m.saison==HIVER,mx,my,clic)){
        m.saison=HIVER;
    }
    y-=bh+6;
    color(70,70,90);
    print(px+12,y,"ESPACE : pause");
    y-=pas;
    print(px+12,y,"R : reset");
    y-=pas;
    print(px+12,y,"F : fermier");
    y-=pas;
    print(px+12,y,"M : meteorite");
    y-=pas;
    print(px+12,y,"I : incendie");
    y-=pas;
    print(px+12,y,"+/- : vitesse");
    y-=pas;
    print(px+12,y,"ECHAP : quitter");
}

void draw(Monde &m,int mx,int my,bool clic){
    dessiner_grille(m);
    dessiner_lapins(m);
    dessiner_loups(m);
    dessiner_vaches(m);
    dessiner_fermier(m);
    dessiner_meteorites(m);
    dessiner_panel(m,mx,my,clic);
}

void gerer_fermier(Monde &m,int mx,int my){
    if(mx<(WIN_W-PANEL_W) && mx>=0 && my>=0 && my<WIN_H){
        int gx=mx/CELL;
        int gy=my/CELL;
        if(gx>=0 && gx<DIMW && gy>=0 && gy<DIMH){
            if(m.nb_fermiers<MAX_FERMIERS){
                m.fermiers[m.nb_fermiers].x=gx;
                m.fermiers[m.nb_fermiers].y=gy;
                m.fermiers[m.nb_fermiers].rayon=4;
                m.fermiers[m.nb_fermiers].actif=true;
                m.nb_fermiers++;
            }
        }
    }
}

void gerer_meteorite(Monde &m,int mx,int my){
    if(mx<(WIN_W-PANEL_W) && mx>=0 && my>=0 && my<WIN_H){
        int gx=mx/CELL;
        int gy=my/CELL;
        if(gx>=0 && gx<DIMW && gy>=0 && gy<DIMH){
            lancer_meteorite(m,gx,gy);
        }
    }
}

void gerer_incendie(Monde &m,int mx,int my){
    if(mx<(WIN_W-PANEL_W) && mx>=0 && my>=0 && my<WIN_H){
        int gx=mx/CELL;
        int gy=my/CELL;
        if(gx>=0 && gx<DIMW && gy>=0 && gy<DIMH){
            demarrer_incendie(m,gx,gy);
        }
    }
}

int main(int,char**){
    Monde m;
    bool stop=false;
    bool pause=false;
    int compteur=0;
    bool espace_avant=false;
    bool f_avant=false;
    bool m_avant=false;
    bool i_avant=false;
    bool plus_avant=false;
    bool moins_avant=false;
    int mode_clic=0;
    winInit("Simulation de Proies/Predateurs de Ghrab Mohamed Adam",WIN_W,WIN_H);
    init(m);
    while(!stop){
        winClear();
        bool espace_maintenant=isKeyPressed(SDLK_SPACE);
        if(espace_maintenant && !espace_avant){
            pause=!pause;
        }
        espace_avant=espace_maintenant;
        if(isKeyPressed(SDLK_r)){
            init(m);
            mode_clic=0;
        }
        bool f_maintenant=isKeyPressed(SDLK_f);
        if(f_maintenant && !f_avant){
            if(mode_clic==1){
                mode_clic=0;
            }else{
                mode_clic=1;
            }
        }
        f_avant=f_maintenant;
        bool m_maintenant=isKeyPressed(SDLK_m);
        if(m_maintenant && !m_avant){
            if(mode_clic==2){
                mode_clic=0;
            }else{
                mode_clic=2;
            }
        }
        m_avant=m_maintenant;
        bool i_maintenant=isKeyPressed(SDLK_i);
        if(i_maintenant && !i_avant){
            if(mode_clic==3){
                mode_clic=0;
            }else{
                mode_clic=3;
            }
        }
        i_avant=i_maintenant;
        bool plus_maintenant=isKeyPressed(SDLK_KP_PLUS) || isKeyPressed(SDLK_EQUALS);
        if(plus_maintenant && !plus_avant){
            m.vitesse++;
            if(m.vitesse>10){
                m.vitesse=10;
            }
        }
        plus_avant=plus_maintenant;
        bool moins_maintenant=isKeyPressed(SDLK_KP_MINUS) || isKeyPressed(SDLK_6);
        if(moins_maintenant && !moins_avant){
            m.vitesse--;
            if(m.vitesse<1){
                m.vitesse=1;
            }
        }
        moins_avant=moins_maintenant;
        int mx,my;
        mousePos(mx,my);
        bool clic=isMousePressed(SDL_BUTTON_LEFT);
        if(clic && mode_clic==1){
            gerer_fermier(m,mx,my);
            mode_clic=0;
        }else if(clic && mode_clic==2){
            gerer_meteorite(m,mx,my);
            mode_clic=0;
        }else if(clic && mode_clic==3){
            gerer_incendie(m,mx,my);
            mode_clic=0;
        }
        compteur++;
        if(!pause && compteur>=(11-m.vitesse)){
            update(m);
            compteur=0;
        }
        draw(m,mx,my,clic);
        if(mode_clic==1){
            color(255,80,0);
            print(10,WIN_H-30,"cliquez n'importe ou pour placer un FERMIER ");
        }else if(mode_clic==2){
            color(255,80,0);
            print(10,WIN_H-30,"cliquez n'importe ou pour lancer une METEORITE ");
        }else if(mode_clic==3){
            color(255,80,0);
            print(10,WIN_H-30,"cliquez n'importe ou pour demarrer un INCENDIE ");
        }
        stop=winDisplay();
    }

    winQuit();
    return 0;
}
