#pragma once

using namespace std;

class Dna {
public:
    
    vector<float> dna;
    int num;
    int generation;
    
    int mateMode;
    int boundsMode;
    
    int poolelements;
    
    
    Dna();
    ~Dna(){}
    
    // setup
    void setup(int num) ;
    void setup(vector<float>* data);
    void setup(Dna & d);
    void setup(Dna & d, float dev);
    
    Dna * setNum(int n);
    int size();
    int getNum();
    void incGeneration();
    
    // set
    Dna * setDna(Dna * d);
    Dna * setDna( vector<float>& data);
    Dna * setGene(int n, float val);
    //Dna * set( int n,  float val);
    Dna * setRandomDNA();
    Dna * setMateMode(int mateMode);
    Dna * setBoundsMode(int boundsMode);
    
    // get
    vector<float>* getDna();
    float get(int n);
    float getGene(int n);
    int getMateMode();
    int getBoundsMode();
    
    
    // mutate
    Dna * mutate(float prob);
    Dna * mutate(float prob, float amount);
    Dna * mutateGene(int gene, float dev) ;
    
    // mate
    Dna * mate(Dna & dnaparent);
    Dna * mate(Dna & dnaparent, float param);
    
    // difference
    float difference(Dna & dnatarget) ;
    vector<float> differenceDNA(Dna & dnatarget) ;
    float differenceGene(int gene, Dna & dnatarget) ;
    float fitness(Dna & dnatarget) ;
    
    // data
    void print() ;
    void printDna() ;
    string toString();
    
protected:
    void bound();
    
    Dna * crossover1(Dna & dnaparent);
    Dna * crossover2(Dna & dnaparent, float prob);
    Dna * crossover3(Dna & dnaparent, float percent);
    Dna * crossover4(Dna & dnaparent);
    
};
