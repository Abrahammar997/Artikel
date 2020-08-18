#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <fstream>

using namespace std;

class Node
{
public:
    //Definimos un booliano para ver si ya fue visitada, este nos ayudara a medir la front speed
    bool vista = 0;
    // Definimos el nodo con celulas tipo S
    int ncellS = 0; //No. de celulas
    //Indicamos que canales están ocupados con un 1, y cuales no con un 0.
    int leftS = 0;  
    int upS = 0;
    int rightS = 0; 
    int downS = 0;

    // Definimos el nodo con celulas tipo T
    int ncellT = 0; //No. de celulas
    //Indicamos que canales están ocupados con un 1, y cuales no con un 0.
    int leftT = 0;
    int upT = 0;
    int rightT = 0;
    int downT = 0;

    Node()
    {
        //pass
    }
    
    void R(int chS, int chT, double nu, int alpha, double pT, double d)
    {
        //Si tenemos celulas se dan los escenarios de reproduccion
        if((ncellS+ncellT)>0)
        {
            vista = 1; //Aca marcamos sí alguna vez tuvo una visita
            double r;
            r = ((double) rand() / (RAND_MAX));
    
            double P1,P2,P3;
            //Factores en la proba:
            double bS = (double) chS;
            double bT = (double) chT;
            double NS = (double) ncellS;
            double NT = (double) ncellT;
            //Reproduccion en celulas S
            P1 = (double) (1/(NS + NT))*nu*(NS/(NS + NT))*((bS-NS)/bS)*pT*pow(1-NT/(NS + NT),alpha);
            //Reproduccion en celulas T
            P2 = (double) (1/(NS + NT))*nu*(NS/(NS + NT))*((bT-NT)/chT)*((bT-NT+1)/(bT -1))*(1-pT*pow(1-NT/(NS + NT),alpha)); 
            //Muerte de celulas T
            P3 = (double) (1/(NS + NT))*(NT/(NS + NT))*d;

            if (r < P1) ++ncellS;
            else if (r < (P1+P2)) ++ncellT;
            else if (r < (P1+P2+P3)) --ncellT;

            
        }
    }
        
    
    
    void O(int chS, int chT)
    {
    //Code for random
    double p;
    
    int spaces,bullets,bag;
    
    //Code for S cells---------------------------
    
    //Decide which r going to velocity channels(bag) or to rest channels
    spaces = chS;
    bullets = ncellS;
    bag = 0;
    
    while ((bag != 4)&&(bullets != 0))
    {
        p = ((double) rand() / (RAND_MAX));
        if (p < ((double) (4-bag)/spaces)) ++bag;
        --spaces;
        --bullets;
    }
    
    // Decide which velocity channels are going to be taken
    leftS=0;
    rightS=0;
    upS=0;
    downS=0;
    
    p = ((double) rand() / (RAND_MAX)); 
    if(p < (double)bag/4 )  leftS = 1;    
    p = ((double) rand() / (RAND_MAX));    
    if(p < (double)(bag-leftS)/3 )  upS = 1;    
    p = ((double) rand() / (RAND_MAX));    
    if(p < (double)(bag-leftS-upS)/2 )  rightS = 1;    
    p = ((double) rand() / (RAND_MAX));    
    if(p < (double)(bag-leftS-upS-rightS) )  downS = 1;

    
    //Code for T cells---------------------------
    
    //Decide which r going to velocity channels(bag) or to rest channels
    spaces = chT;
    bullets = ncellT;
    bag = 0;
    
    while ((bag != 4)&&(bullets != 0))
    {
        p = ((double) rand() / (RAND_MAX));
        if (p < ((double) (4-bag)/spaces)) ++bag;
        --spaces;
        --bullets;
    }

    // Decide which velocity channels are going to be taken
    leftT=0;
    rightT=0;
    upT=0;
    downT=0;
    
    p = ((double) rand() / (RAND_MAX));    
    if(p < (double)bag/4 )  leftT = 1;    
    p = ((double) rand() / (RAND_MAX));    
    if(p < (double)(bag-leftT)/3 )  upT = 1;    
    p = ((double) rand() / (RAND_MAX));    
    if(p < (double)(bag-leftT-upT)/2 )  rightT = 1;    
    p = ((double) rand() / (RAND_MAX));    
    if(p < (double)(bag-leftT-upT-rightT) )  downT = 1;
    }
    
    
    void P(Node nL, Node nU, Node nR, Node nD)
    {
    //La migracion se caracteriza por flujo entrante menos saliente
        int InS = nL.rightS +  nU.downS + nR.leftS + nD.upS;
        int OutS = rightS +  downS + leftS + upS;
        ncellS += InS - OutS;
    
        int InT = nL.rightT +  nU.downT + nR.leftT + nD.upT;
        int OutT = rightT +  downT + leftT + upT;
        ncellT += InT - OutT;
    }
    
    void Pbound( Node nU, Node nR, Node nD)
    {
        int InS =  nU.downS + nR.leftS + nD.upS;
        int OutS = downS + leftS + upS;
        ncellS += InS - OutS;
    
        int InT = nU.downT + nR.leftT + nD.upT;
        int OutT = downT + leftT + upT;
        ncellT += InT - OutT;
    }
    
    void POR(Node nL, Node nU, Node nR, Node nD, int chS, int chT, double nu, int alpha, double pT, double d)
    {
        //La transicion de tiempo se caracteriza por la aplicacion POR
        R(chS, chT, nu, alpha, pT, d);
        O(chS, chT);
        P(nL, nU, nR, nD);
    }
    
    void PboundOR(Node nU, Node nR, Node nD,  int chS, int chT, double nu, int alpha, double pT, double d)
    {
        R(chS, chT, nu, alpha, pT, d);
        O(chS, chT);
        Pbound(nU, nR, nD);
    }
    
    void inicializar(int chS)
    {
        //Pone de 0 a Capacidad maxima-1 de celulas aleatoriamente
        ncellS = rand() % chS;
    }

    int density()
    {
        //Devuelve el no. de celulas totales
        return (ncellS + ncellT);
    }
    int densityS()
    {
        //Devuelve el no. de celulas CSC
        return ncellS;
    }

    int densityT()
    {
        //Devuelve el no. de celulas DCC
        return ncellT;
    }

    bool preferS()
    {
        if (ncellS >= ncellT)
        {
            return 1;
        }
        else
        {
            return 0;
        }
        
    }
  
};


class LGCA
{
public:
    vector<vector<Node> > Lattice;
    int Time = 0;
    int height;
    int width;
    int N;

    //Node parameters
    int chS;
    int chT;
    double nu;
    int alpha;
    double pT;
    double d;
    
    LGCA(int alto, int largo, int bS, int bT, double v, int a, double p, double death)
    {
        height = alto;
        width = largo;
        chS = bS;
        chT = bT;
        N = bS +bT;
        nu = v;
        alpha = a;
        pT = p;
        d = death;

        for (int i = 0; i < alto; i++)
        {
            vector<Node> temp;
            for (int j = 0; j < largo; j++)
            {
                temp.push_back(Node());
            }
            Lattice.push_back(temp);
        }
        
    }

    void renombrar(int alto, int largo, int bS, int bT, double v, int a, double p, double death)
    {
        Time = 0;
        height = alto;
        width = largo;
        chS = bS;
        chT = bT;
        N = bS +bT;
        nu = v;
        alpha = a;
        pT = p;
        d = death;
        
        vector<vector<Node> > auxLatice;
        for (int i = 0; i < alto; i++)
        {
            vector<Node> temp;
            for (int j = 0; j < largo; j++)
            {
                temp.push_back(Node());
            }
            auxLatice.push_back(temp);
        }
        Lattice = auxLatice;
    }
    
    void putstrip()
    {
    for(int i=0; i<height; i++)
    {
        for(int j=0; j < 4; j++)
        {
            Lattice[i][j].inicializar(chS);
        }
    }
    }
    
    bool isbig()
    {
        for(int i=0; i<height; i++)
        {
            if((bool)(Lattice[i][width-5].ncellS + Lattice[i][width-5].ncellT))
            {
            return 0;
            }
        }
        return 1;    
    }
    
    void makebig()
    {
        width += 100;
        for(int j=0; j<height; j++)
        {
            for(int k = 0; k<100; k++)
            {
                Lattice[j].push_back(Node());
            }
        }
    } 
    
    void step()
    {
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width-2); j++)
        {
            if(j==0)
            {    
                if(i==0) 
                    Lattice[i][j].PboundOR(Lattice[height-1][j], Lattice[i][j+1], Lattice[i+1][j], chS, chT, nu, alpha, pT, d);
                else if(i== (height-1)) 
                    Lattice[i][j].PboundOR(Lattice[i-1][j],Lattice[i][j+1],Lattice[0][j], chS, chT, nu, alpha, pT, d);
                else 
                    Lattice[i][j].PboundOR(Lattice[i-1][j],Lattice[i][j+1],Lattice[i-1][j], chS, chT, nu, alpha, pT, d);
            }
            else
            {
                if(i==0) 
                    Lattice[i][j].POR(Lattice[i][j-1], Lattice[height-1][j], Lattice[i][j+1], Lattice[i+1][j], chS, chT, nu, alpha, pT, d);
                else if(i== (height-1)) 
                    Lattice[i][j].POR(Lattice[i][j-1],Lattice[i-1][j],Lattice[i][j+1],Lattice[0][j], chS, chT, nu, alpha, pT, d);
                else 
                    Lattice[i][j].POR(Lattice[i][j-1],Lattice[i-1][j],Lattice[i][j+1],Lattice[i+1][j], chS, chT, nu, alpha, pT, d);
            }
        } 
    }
    }
    
    void walk(int t)
    {
        for(int k=0; k<t; k++)
        {
            if(!(isbig())) makebig();
            Time++;
            step();
        }
    }

    vector<int> bulk_info()
    {
        vector<int> front = {width,0,0,0,Time};
        // Acá encontramos el bulk size.
        for(int y = 0; y < height; y++)
        {
            int x=0;
            bool flag = 1;
            while(flag)
            {
                flag = ((bool) Lattice[y][x].density());
                x++;   
            }
            if (front[1] <= x) front[1] = x;
            if (front[0] >= x) front[0] = x;
        }

        //recoleccion de no de celulas tipo T y tipo S
        for(int y = 0; y < height; y++)
        {
            for(int x = front[0]; x < front[1]; x++)
            {
                front[2] += Lattice[y][x].densityS();
                front[3] += Lattice[y][x].densityT();
            }
        }

        return front;
    }

    void retrato()
    {
        //string stringT = to_string(Time);
        string filename = "zev.ppm";
        ofstream img (filename);
        img << "P3" <<endl;
        img << width << " "<<height<<endl;
        img << "255" << endl;

        for(int y = 0; y < height; y++)
        {
            for(int x=0; x < width; x++)
            {
                int b,r,g;
                b = 255 - (Lattice[y][x].densityS()*255)/chS;
                r = 255 - (Lattice[y][x].densityT()*255)/chT;
                g = 255;
                img << r << " " << g << " " << b << " " << endl;
                
            }
        }

    }
    
};

int main()
{

    //Inicializamos la aleatoridad
    srand(time(NULL));

    //Este es el código base

    //Try me
    LGCA Ejem = LGCA(100, 50, 4, 4, 0.5, 0, 0.9, 0.1);
    Ejem.putstrip();
    Ejem.walk(10000);
    Ejem.retrato();

    //Independent variable: bS & bT
    //  values:             (linspace 3 al 0) dividir en cuartos (12 en total)
    //Parameter conf:
    //Time: 15000
    //Alto: 1000
    //*a: 3
    //*p: 0.7
    //*d: 0.05

    //Response variable: Dominate mode & Front position
}
