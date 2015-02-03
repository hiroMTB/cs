#include "Dna.h"
#include "cinder/Rand.h"
#include "cinder/Utilities.h"

using namespace ci;

Dna::Dna(){
    mateMode = 0;
    boundsMode = 2;
    poolelements = 0;
}

// setup

void Dna::setup(int num) {
    setNum(num);
}

void Dna::setup(vector<float>* data) {
    setNum(data->size());
    for (int i = 0; i < data->size(); i++) {
        dna[i] = data->at(i);
    }
}

void Dna::setup(Dna & d) {
    setNum(d.num);
    for (int i = 0; i < d.dna.size(); i++) {
        dna[i] = d.dna[i];
    }
}

void Dna::setup(Dna & d, float dev) {
    setNum(d.num);
    for (int i = 0; i < d.dna.size(); i++) {
        dna[i] = d.dna[i]+ randFloat(-dev, dev);
    }
}


// ////////

/**
 * set num genes
 *
 * @param n
 * @return
 */
Dna * Dna::setNum(int n) {
    num = n;
    dna.assign(num, 0);
    return setRandomDNA();
}

/**
 * @return num
 */
int Dna::size() {
    return num;
}
int Dna::getNum() {
    return num;
}

/**
 * @param dna
 * @return
 */
Dna * Dna::setDna(Dna * d) {
    return setDna(d->dna);
}

/**
 * @param data
 * @return
 */
Dna * Dna::setDna( vector<float>& data) {
    if (data.size() != dna.size())
        setNum(data.size());
    
    for (int i = 0; i < num; i++) {
        dna[i] = data[i];
    }
    
    return this;
}

/**
 * @return dna float[]
 */
vector<float>* Dna::getDna() {
    return &dna;
}

float Dna::get(int n) {
    return dna[n];
}
float Dna::getGene(int n) {
    return dna[n];
}

Dna * Dna::setGene(int n, float val) {
    dna[n] = val;
    return this;
}
//Dna * Dna::set( int n,  float val) {
//    dna[n] = val;
//    return this;
//}

Dna * Dna::setRandomDNA() {
    for (int i = 0; i < dna.size(); i++) {
        dna[i] = randFloat(1);
    }
    return this;
}

void Dna::incGeneration(){
    generation++;
}

Dna * Dna::mutate(float prob) {
    incGeneration();
    for (int i = 0; i < num; i++) {
        if (randFloat(1) < prob)
            dna[i] = randFloat(1.0000001f);
    }
    bound();
    return this;
}

Dna * Dna::mutate(float prob, float amount) {
    incGeneration();
    // for (int i = 0; i < num; i++) {
    // float rnd = random(1f);
    // if (rnd > a && rnd < b)
    // dna[i] = random(1.0000001f);
    // }
    // return this;
    for (int i = 0; i < num; i++) {
        if (randFloat(1) < prob)
            dna[i] += randFloat(-amount, amount);
    }
    bound();
    return this;
}

// mate
Dna * Dna::mate(Dna & dnaparent) {
    return mate(dnaparent, 0.5f);
}

// mate
Dna * Dna::mate(Dna & dnaparent, float param) {
    incGeneration();
    switch (mateMode) {
        case 0:
            return crossover1(dnaparent);
        case 1:
            return crossover2(dnaparent, param);
        case 2:
            return crossover3(dnaparent, param);
        case 3:
            return crossover4(dnaparent);
    }
    return this;
}

Dna * Dna::crossover1(Dna & dnaparent) {
    int pt = (int) randFloat(dna.size());
    for (int i = 0; i < num; i++) {
        if (i < pt)
            continue;
        else
            dna[i] = dnaparent.dna[i];
    }
    bound();
    return this;
}

Dna * Dna::crossover2(Dna & dnaparent, float prob) {
    for (int i = 0; i < num; i++) {
        bool useOtherGene = randFloat(1) > prob;
        if (useOtherGene)
            dna[i] = dnaparent.dna[i];
    }
    bound();
    return this;
}

Dna * Dna::crossover3(Dna & dnaparent, float percent) {
    float per0 = 1.0f - percent;
    float per1 = percent;
    for (int i = 0; i < num; i++) {
        dna[i] = per0 * dna[i] + per1 * dnaparent.dna[i];
    }
    bound();
    return this;
}

Dna * Dna::crossover4(Dna & dnaparent) {
    for (int i = 0; i < num; i++) {
        dna[i] = randFloat(dna[i], dnaparent.dna[i]);
    }
    bound();
    return this;
}


void Dna::bound(){
    
    if(boundsMode == 0) {
        return;
    }
    
    if (boundsMode == 1) {
        for(int i=0; i<dna.size();i++) {
            if (dna[i] > 1)
                dna[i] = 1;
            if (dna[i] < 0)
                dna[i] = 0;
        }
    }
    if (boundsMode == 2) {
        for(int i=0; i<dna.size();i++) {
            while (dna[i] > 1)
                dna[i] -= 1;
            while (dna[i] < 0)
                dna[i] += 1;
        }
    }
    
    
    
}

/**
 * @param gene
 * @param dev
 * @return
 */
Dna * Dna::mutateGene(int gene, float dev) {
    dna[gene] += randFloat(-dev, dev);
    
    return this;
}

/**
 * @param dnatarget
 * @return
 */
float Dna::difference(Dna & dnatarget) {
    float val = 0.f;
    for (int i = 0; i < num; i++) {
        val += abs(dnatarget.dna[i] - dna[i]);
    }
    return val;
}

/**
 * @param dnatarget
 * @return
 */
vector<float> Dna::differenceDNA(Dna & dnatarget) {
    vector <float> dif; dif.assign(dnatarget.num, 0);
    for (int i = 0; i < num; i++) {
        dif[i] = abs(dnatarget.dna[i] - dna[i]);
    }
    return dif;
}

/**
 * @param gene
 * @param dnatarget
 * @return
 */
float Dna::differenceGene(int gene, Dna & dnatarget) {
    return abs(dnatarget.dna[gene] - dna[gene]);
}

/**
 * @param dnatarget
 * @return
 */
float Dna::fitness(Dna & dnatarget) {
    return 1.0f - (difference(dnatarget) / num);
}

void Dna::print() {
    printDna();
}

void Dna::printDna() {
    cout << "dna num: " + cinder::toString(num) + "\n";
    for (int i = 0; i < num; i++) {
        cout << cinder::toString(dna[i]) + " ";
    }
    cout << endl;
}

string Dna::toString() {
    string data = "";
    for (int i = 0; i < num; i++) {
        data += cinder::toString(dna[i]) + " ";
    }
    return data;
}

Dna * Dna::setMateMode(int mateMode) {
    this->mateMode = mateMode;
    return this;
}

int Dna::getMateMode() {
    return mateMode;
}

Dna * Dna::setBoundsMode(int boundsMode) {
    this->boundsMode = boundsMode;
    return this;
}

int Dna::getBoundsMode() {
    return boundsMode;
}

