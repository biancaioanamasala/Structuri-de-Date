#include "intersectii.h"

// functie ajutatoare
void afiseaza_lista_intervale(ListaIntervale2D *lista) {
    int i;
    for (i = 0; i < lista->dimensiune; i++) {
        Interval2D *interv = lista->intervale[i];
        printf("{punct stanga x: %d, punct stanga y: %d, punct dreapta x: %d, punct dreapta y: %d}\n",
                    interv->punct_stanga->x, interv->punct_stanga->y,
                    interv->punct_dreapta->x, interv->punct_dreapta->y);
    }
}


ListaIntervale2D* citeste_intrare(char *nume_fisier_intrare) {
    // TODO citeste intervalele din fisierul dat ca parametru
    // si le salveaza in lista de intervale
    FILE * fis = fopen(nume_fisier_intrare, "rt");
    int numar_intervale;
    int x1, x2, y1, y2;
    fscanf(fis, "%d", & numar_intervale);
    ListaIntervale2D * lista = (ListaIntervale2D * ) malloc(sizeof(ListaIntervale2D));
    lista -> dimensiune = numar_intervale;
    lista -> intervale = (Interval2D ** ) malloc(numar_intervale * sizeof(Interval2D *));

    for (int i = 0; i < numar_intervale; i++) {
      fscanf(fis, "%d %d %d %d", & x1, & y1, & x2, & y2);
      lista -> intervale[i] = malloc(sizeof(Interval2D));
      lista -> intervale[i] -> punct_stanga = malloc(sizeof(Punct2D));
      lista -> intervale[i] -> punct_dreapta = malloc(sizeof(Punct2D));
      lista -> intervale[i] -> punct_stanga -> x = x1;
      lista -> intervale[i] -> punct_stanga -> y = y1;
      lista -> intervale[i] -> punct_dreapta -> x = x2;
      lista -> intervale[i] -> punct_dreapta -> y = y2;
    }
    return lista;
}

// ! Functie pentru actualizarea in arbore
// ! O veti da ca parametru cand initializati arborele
void actualizare_cu_delta(Nod *nod, int v2) {
    nod->info += v2;
}

// ! Functie pentru combinarea raspunsurilor in arbore
// ! O veti da ca parametru cand initializati arborele
int suma_raspunsurilor(int r1, int r2) {
    return r1 + r2;
}

// functie ajutatoare folosita in calculul numarului de intersectii
int cmp_function(const void *a, const void *b) {
    int xa = ((Event *)a) -> abscisa;
    int xb = ((Event *)b) -> abscisa;
    int tipa = ((Event *)a) -> tip;
    int tipb = ((Event *)b) -> tip;
    if(xa == xb) {
        tipa = ((Event *)a) -> tip;
        tipb = ((Event *)b) -> tip;
        return tipa - tipb;
    }
    return xa - xb;
}

int calculeaza_numar_intersectii(ListaIntervale2D *lista) {
    // TODO calculati numarul de intersectii folosind arbori de intervale conform enuntului
    // Hint: initializarea arborelui: 
    // ArboreDeIntervale *arbore = construieste_arbore(0, y_max, 0, actualizare_cu_delta, suma_raspunsurilor);
    int y_max = 0, i;
    int raspuns = 0;
    int x1, x2, y1, y2;
    Event *evenim = malloc(2 * lista->dimensiune * sizeof(Event));
    int cnt = 0;
    for(i = 0; i < lista -> dimensiune; i++) {
        x1 = lista -> intervale[i] -> punct_stanga -> x;
        y1 = lista -> intervale[i] -> punct_stanga -> y;
        x2 = lista -> intervale[i] -> punct_dreapta -> x;
        y2 = lista -> intervale[i] -> punct_dreapta -> y;
        if(y1 > y_max) {
            y_max = y1;
        }
        if(y2 > y_max) {
            y_max = y2;
        }
        if(x1 == x2) {          //segment vertical
            evenim[cnt].tip = 1;
            evenim[cnt].abscisa = x1;
            evenim[cnt].ordonata1 = y1;
            evenim[cnt].ordonata2 = y2;
            cnt++;
        } else {              //segment orizontal
            evenim[cnt].tip = 0;
            evenim[cnt].abscisa = x1;
            evenim[cnt].ordonata1 = y1;
            evenim[cnt].ordonata2 = y1;
            evenim[cnt + 1].tip = 2;
            evenim[cnt + 1].abscisa = x2;
            evenim[cnt + 1].ordonata1 = y2;
            evenim[cnt + 1].ordonata2 = y2;
            cnt += 2;

        }
    }
    qsort(evenim, cnt, sizeof(Event), cmp_function);
    ArboreDeIntervale *arbore = construieste_arbore(0, y_max, 0, actualizare_cu_delta, suma_raspunsurilor);
    Interval vertical;
    for (i = 0; i < cnt; i++) {  //parcurg lista sortata
        x1 = evenim[i].abscisa;
        y1 = evenim[i].ordonata1;
        y2 = evenim[i].ordonata2;
        if(evenim[i].tip == 1) { // segment vertical
            if(y1 < y2) {
                vertical.capat_stanga = y1;
                vertical.capat_dreapta = y2;
            } else {
                vertical.capat_stanga = y2;
                vertical.capat_dreapta = y1;
            }
            raspuns += interogare_interval_in_arbore(arbore, &vertical);
        }
        if(evenim[i].tip == 0) { // incepe un nou segment
            vertical.capat_stanga = y1;
            vertical.capat_dreapta = y2;
            actualizare_interval_in_arbore(arbore, &vertical, 1);
        }
        if(evenim[i].tip == 2) {
            vertical.capat_stanga = y1;
            vertical.capat_dreapta = y2;
            actualizare_interval_in_arbore(arbore, &vertical, -1);
        } 
    }
    free(evenim);
    return raspuns;
}


int calculeaza_numar_intersectii_trivial(ListaIntervale2D *lista) {
    // TODO: solutia triviala, verific fiecare segment cu fiecare segment daca se intersecteaza 
    int raspuns = 0, i, j;
    int x1, x2, x3, x4, y1, y2, y3, y4;
    for(i = 0; i < lista -> dimensiune; i++) {
        x1 = lista -> intervale[i] -> punct_stanga -> x;
        y1 = lista -> intervale[i] -> punct_stanga -> y;
        x2 = lista -> intervale[i] -> punct_dreapta -> x;
        y2 = lista -> intervale[i] -> punct_dreapta -> y;
        if(x1 != x2) {
            for(j = 0; j < lista -> dimensiune; j++) {
                x3 = lista -> intervale[j] -> punct_stanga -> x;
                y3 = lista -> intervale[j] -> punct_stanga -> y;
                x4 = lista -> intervale[j] -> punct_dreapta -> x;
                y4 = lista -> intervale[j] -> punct_dreapta -> y;
                if(x3 == x4 && x1 <= x3 && x3 <= x2 && ((y3 <= y1 && y4 >= y2) || (y3 >= y1 && y4 <= y1))) {
                    raspuns++;
                }
            }
        }
    }
    return raspuns;
}
