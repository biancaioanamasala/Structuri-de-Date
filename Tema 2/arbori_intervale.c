#include "arbori_intervale.h"

void creare_nod(Nod* nod, int capat_stanga, int capat_dreapta) {
    nod -> info = 0;
    nod -> interval = (Interval *)malloc(sizeof(Interval));
    nod -> copil_dreapta = NULL;
    nod -> copil_stanga = NULL;
    nod -> interval -> capat_stanga = capat_stanga;
    nod -> interval -> capat_dreapta = capat_dreapta;
    int mijloc;
    if( capat_stanga < capat_dreapta )
    {
        mijloc = ( capat_stanga + capat_dreapta ) / 2;
        nod -> copil_stanga = (Nod *)malloc(sizeof(Nod));
        nod -> copil_dreapta = (Nod *)malloc(sizeof(Nod));
        creare_nod( nod -> copil_stanga , capat_stanga , mijloc );
        creare_nod( nod -> copil_dreapta , mijloc + 1 , capat_dreapta );
    }
}
ArboreDeIntervale *construieste_arbore(int capat_stanga, int capat_dreapta,
        int valoare_predifinita_raspuns_copil,
        void (*f_actualizare)(Nod *nod, int v_actualizare),
        int (*f_combinare_raspunsuri_copii)(int raspuns_stanga, int raspuns_dreapta)) {
    // TODO implementati functia de constructie a arborelui prezentata in enunt
    // TODO initializati campurile unui ArboreDeIntervale*
    ArboreDeIntervale* arbore=(ArboreDeIntervale *)malloc(sizeof(ArboreDeIntervale));\
    arbore -> radacina = (Nod *)malloc(sizeof(Nod));
    arbore -> dimensiune = capat_dreapta - capat_stanga + 1;
    arbore -> valoare_predifinita_raspuns_copil = valoare_predifinita_raspuns_copil;
    arbore -> f_actualizare = f_actualizare;
    arbore -> f_combinare_raspunsuri_copii = f_combinare_raspunsuri_copii;
    creare_nod(arbore -> radacina, capat_stanga , capat_dreapta );
    return arbore;
}


void actualizare_interval_pentru_nod(ArboreDeIntervale *arbore, Nod *nod,
                                        Interval *interval, int v_actualizare) {
    // TODO implementati functia de actualizare pentru un nod
    // Hint: pentru a actualiza un nod cand este cazul folositi arbore->f_actualizare(nod, v_actualizare);
    int a = interval -> capat_stanga;
    int b = interval -> capat_dreapta;
    if( a <= nod -> interval -> capat_stanga && nod -> interval -> capat_dreapta <= b)
    {
        arbore -> f_actualizare( nod , v_actualizare );
    }
    else
    {
        int mijloc = ( nod -> interval -> capat_stanga + nod -> interval -> capat_dreapta ) / 2;
        if(a <= mijloc)
        {
            actualizare_interval_pentru_nod( arbore , nod->copil_stanga , interval , v_actualizare);
        }
        if( mijloc < b )
        {
            actualizare_interval_pentru_nod( arbore , nod-> copil_dreapta , interval , v_actualizare );
        }
        arbore -> f_actualizare( nod , v_actualizare );
    }
} 


// Functia este deja implementata, se cheama functia de mai sus cu radacina arborelui
void actualizare_interval_in_arbore(ArboreDeIntervale *arbore, 
                                        Interval *interval, int v_actualizare) {
    actualizare_interval_pentru_nod(arbore, arbore->radacina, interval, v_actualizare);
}


int interogare_interval_pentru_nod(ArboreDeIntervale *arbore, Nod *nod, Interval *interval) {
    // TODO implementati functia de interogare pentru un nod prezentata in enunt
    // Hint: cand vreti sa calculati combinarea raspunsurilor folositi
    // arbore->f_combinare_raspunsuri_copii(raspuns_copil_stanga, raspuns_copil_drepta)
    int a = interval -> capat_stanga;
    int b = interval -> capat_dreapta;
    int mijloc, val_s = 0 , val_d = 0;
    if( a <= nod -> interval -> capat_stanga && nod -> interval -> capat_dreapta <= b )
    {
        return nod -> info;
    }
    else
    {
        
        mijloc = ( nod -> interval -> capat_stanga + nod -> interval -> capat_dreapta ) / 2 ;
        if( a <= mijloc )
        {
            val_s = interogare_interval_pentru_nod( arbore , nod -> copil_stanga , interval );
        }
        if( mijloc < b )
        {
            val_d = interogare_interval_pentru_nod( arbore , nod -> copil_dreapta , interval );
        }
        return arbore -> f_combinare_raspunsuri_copii( val_s , val_d );
    }
}


// Functia este deja implementata, se cheama functia de mai sus cu radacina arborelui
int interogare_interval_in_arbore(ArboreDeIntervale *arbore, Interval *interval) {
    return interogare_interval_pentru_nod(arbore, arbore->radacina, interval);
}


// ----- DOAR pentru bonus si DOAR daca considerati ca e necesara ----- //
void seteaza_info_in_nod_la_valoare_capat_dreapta(Nod* nod) {
    // TODO cred ca e destul de clar ce vrea sa faca functia asta
}


void seteaza_info_in_arbore_la_valoare_capat_dreapta(ArboreDeIntervale* arbore) {
    seteaza_info_in_nod_la_valoare_capat_dreapta(arbore->radacina);
}
