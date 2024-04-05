#include "IsingModel.h"

IsingModel MetropolisHasting::run(int MCSteps, IsingModel model) {
    for (int i = 0; i<MCSteps; i++){

        for (int j = 0; j <model.get_system_size(); j++){
            // claculate the energy
            step(model);
        }
    }
    return model;
}
void MetropolisHasting::step(IsingModel &model) {
        double deltaE;
        IsingSpin *testSpin;

        testSpin = &(model.get_random_spin());
        // here we use that E(sigma) = -E(-sigma); I.E our hamiltion for E_i is linear in sigma_i
        // delta E is E_i(-sigma_i)-E_I(sigma_i) = -2E_i(sigma_i)
        deltaE = -2*model.get_energy(*testSpin);
        if (accept(deltaE)) {
            testSpin->flip();
        }

}
bool MetropolisHasting::accept(double deltaE) { 

    // if this reduces energy accept the change
    if (deltaE<0) {
        return true; 
    } else {
        // otherwise generate a random number and compare exp(-delta /T to it)
        double r = _distr(_gen);
        if (r<exp(-deltaE/_T)) {
            return true;
        }
    }
    return false;
}

IsingSpin& IsingModel::get_random_spin() {
    int i = distr(_gen);
    return _spins[i];
}

double IsingModel::get_energy(IsingSpin& spin) {
    double E = 0;
    int neighborDist[4] = {-1, 1, -_SystemSize, +_SystemSize};
    int index = spin.get_index();
    for (int i = 0; i< 4; i++){
        E-= _J*spin.get_state()*_spins[boundaryCheck(index+neighborDist[i])].get_state();
    }
    E-= spin.get_state()*_H;
    return E;
}
int IsingModel::boundaryCheck(int index){
    if (index<0) {
         return index + _SystemSize*_SystemSize;
    }
    if (index>=_SystemSize*_SystemSize) {
        return index - _SystemSize*_SystemSize;
    } 
    return index;
}

// void IsingModel::initialize(double ratio) {
//     for (int i = 0; i<_SystemSize*_SystemSize; i++){
//         if (distr(gen)<ratio){
//             _spins[i].flip();
//         }
//     }
// }

IsingModel::IsingModel(int L, double H, double J): _SystemSize{L},_H{H},_J{J} {
    _spins.resize(_SystemSize*_SystemSize);
    for (int i = 0; i< _SystemSize*_SystemSize; i++){
        _spins[i].set_index(i);
    }
    distr=std::uniform_int_distribution<>(0,_SystemSize*_SystemSize);
}


void MagnetismIsingObserver::takeMeasurement(std::vector<IsingSpin> subject, MetropolisHasting *MCAlgorithm ,double value){
    double m = 0;
    #pragma parallel for reduction(+:m)
    for (int i = 0; i<subject.size(); i++){
        m+=subject[i].get_state();
    }
    m=m/subject.size();
    outFile<<value<<","<<m<<std::endl;
}

void MovieIsingObserver::takeMeasurement(std::vector<IsingSpin> subject, MetropolisHasting *MCAlgorithm ,double value){
    outFile<<value<<',';

    for (int i = 0; i<subject.size(); i++){
        outFile<<subject[i].get_state()<<',';
    }
    
    outFile<<std::endl;
}

void IsingExperiment::movie(int L) {
    IsingModel model(L,0,1);
    MetropolisHasting MCAlgorithm(10.0);
    MovieIsingObserver obs("IsingMovie.out");
    // 101 points from 10 to 0
    for (double T = 5; T>=0; T-=0.1){
        MCAlgorithm.set_T(T);
        for (int i = 0; i<100;i++){
            model = MCAlgorithm.run(1,model);
        }
        obs.takeMeasurement(model.get_spins(),&MCAlgorithm,T);
        std::cout<<(5-T)*20<<'%'<<std::endl;
    }
}
