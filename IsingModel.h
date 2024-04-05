#include <vector>
#include <cmath>
#include <random>
#include <fstream>
#include <string>
#include <iostream>


class IsingSpin;

class IsingModel {
protected:
    std::vector<IsingSpin> _spins;
    // SquareGrid _grid;
    double _J;
    double _H;
    int _SystemSize;
    std::mt19937 _gen;
    std::uniform_int_distribution<> distr;

    int boundaryCheck(int index);
public:
    IsingModel(int L,double H,double J);

    IsingSpin& get_random_spin();
    IsingSpin& get_spin(int i) {return _spins[i];};
    
    double get_energy(IsingSpin& spin);
    

    void set_seed(double seed) {_gen = std::mt19937(seed);}
    void set_H(double H){_H = H;}
    int get_system_size(){return _spins.size();}

    std::vector<IsingSpin> get_spins(){return _spins;}



};

class IsingSpin {
protected:
    bool _state = false;
    int _index;
public:
    int get_state(){if (_state) return 1; else return -1;}
    int get_index(){return _index;}

    void flip(){_state = !_state;}
    void set_index(int i){_index = i;}
};

class MetropolisHasting {
protected:
    double _T;
    std::mt19937 _gen;
    std::uniform_real_distribution<> _distr;
    
public:
    MetropolisHasting(double T): _T{T}{ set_seed(1); _distr=std::uniform_real_distribution<>(0,1);}
    IsingModel run (int MCSteps,IsingModel model);
    void step (IsingModel &model);
    void set_T(double T){_T=T;}
    bool accept(double deltaE);
    void set_seed(double seed) {_gen = std::mt19937(seed);}

};
class IsingObserver{
protected:
    std::fstream outFile;
    int _numMeasurements = 100;
public:
    IsingObserver(std::string fileName){
        outFile.open(fileName,std::fstream::out);
    }
    ~IsingObserver(){
        outFile.close();
    }
    virtual void takeMeasurement(std::vector<IsingSpin> subject,MetropolisHasting* MC, double value){

    }

};

class MagnetismIsingObserver: public IsingObserver{
public:
    using IsingObserver::IsingObserver;
    void takeMeasurement(std::vector<IsingSpin> subject, MetropolisHasting* MC, double value);
};
class MovieIsingObserver: public IsingObserver{
public:
    using IsingObserver::IsingObserver;
    void takeMeasurement(std::vector<IsingSpin> subject, MetropolisHasting* MC, double value);
};



class IsingExperiment {
public:
    static void movie(int L);

};