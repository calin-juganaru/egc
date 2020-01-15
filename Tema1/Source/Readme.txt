@Tema1.h:
        În acest antet sunt definite constantele folosite în joc (dimensiunile păsării, pereților,
înalțimea săriturii, poziția inițială, etc.), clasa în care este implementat jocul și funcțiile
pentru translatare, scalare și rotire.

    Constante:
        MAX_WALLS = numărul de pereți randați în scenă;
        MAX_COLORS = numărul de culori posibile ale pereților;
        PRECISION = precizia cercurilor/elipselor;
        INITIAL_X/Y = coordonatele din care pornește pasărea;
        JUMP_HEIGHT = înălțimea unei sărituri;
        WALL_DISTANCE = distanța dintre doi pereți consecutivi;
        WALL_WIDTH = lățimea unui perete;
        WALL_SCALE = rație folosită la translatarea pereților;
        BIRD_WIDTH = lățimea păsării;
        BIRD_HEIGHT = înălțimea păsării;
        MAX_GAP = lărgimea maximă a golului din pereți;
        MIN_GAP = lărgimea minimă;
        SLIDE_HEIGHT = înalțimea cu care glisează pereții;
        SLIDE_RATIO = SLIDE_HEIGHT / WALL_WIDTH;
        SLIDING_FREQ = frecvența cu care apar în joc pereți glisanți;
        FLY_SPEED_INC = incrementarea vitezei de deplasare;
        FALL_SPEED_INC = incrementarea vitezei de cădere;
        FALL_ANGLE_INC = incrementarea unghiului de rotire;
        MAX_WING_POS = numărul de poziții posibile are aripii.


@Meshes.h:
    create_square(): creează mesh-ul pentru un pătrat
    create_sun(): creează mesh-ul pentru un soare galben, având punctul din centru roșu, pentru un
                    efect în dégradé al culorilor
    create_cloud(): creează mesh-ul pentru un norișor alb
    create_bird(): creează mesh-ul pentru o pasăre, cu o anumită poziție a aripii

@tema1.cpp:
        Init(): creează mesh-urile pentru MAX_COLORS tipuri de pereți, fiecare având o culoare
                generată aleator, un soare, un nor, și MAX_WING_POS păsări, fiecare cu o altă
                poziție a aripii (pentru a comuta între acestea și a crea animația zborului);
                creează și prima tranșă de pereți, inițial, ei vor umple scena, apoi, pe măsură ce
                primul va dispărea în stânga, va apărea unul nou în dreapta, fără să fie generați
                alți pereți, care să nu apară în scenă.
        generate_wall(): întoarce mesh-ul peretelui cu culoarea dată de numărul primit ca parametru
        random_scale(): returnează o rație folosită pentru a scala pereții; inițial, un perete este
                        un pătrat, dar acesta va fi scalat pe ordonată, devenind un dreptunghi,
                        deci rația este înălțimea / lățimea peretelui de jos (cel de sus se bazează
                        pe înălțimea celui de jos + lărgimea golului +/- glisarea)
        OnKeyPress(): tasta SPACE pentru a sări, R pentru repornirea jocului (implicit a progresului,
                        vitezei și scorului), iar P pentru pauză sau revenirea din pauză
        Update():
            - incrementează viteza de cădere, viteza de deplasare, unghiul de rotație, poziția
            aripii; decrementează poziția pe axa ordonată; afișează scorul, care crește cu 1
            la fiecare trecere printr-o gaură
            - verifică dacă pasărea intră în coliziune cu pereții din apropiere (coordonatele sale
            sunt sau nu între coordonatele unui perete, care este un dreptunghi)
            - randează mesh-urile pentru pasăre, nor, soare și pereți. Fiecare pereche de pereți
            este definită prin colțul stânga-jos al peretelui de jos, înalțimea sa și un flag care
            să specifice dacă sunt pereți glisanți sau nu. Toate celelalte mărimi, cum ar fi cele
            care definesc peretele de sus, sunt deduse din acestea și din constante: lățimea, de
            exemplu, este constantă, peretele de sus începe de unde se termină cel de jos +
            înalțimea golului, iar când glisează, pereții se aproprie cu o valoare constantă.
            Pentru a nu randa pereți ce nu (mai) există în scenă am folosit o coadă cu număr
            constant de elemente, și, când primul iese din cadru va fi scos de la începutul cozii
            și va fi adăugat unul nou la sfârșitul ei. Funcția random_scale() verifică dacă rația
            generată (care va fi folosită pentru crearea unei perechi de pereți) permite golului
            și distanței dintre pereți să fie suficient de mare (MIN_GAP) astfel încât pasărea
            să încapă.