#include <stdio.h>

#include <predengine.hpp>
#include <dbAPI.hpp>

using namespace predengine;

char grades[] = {'A', 'B', 'C'};

class LoanPrediction {
    public:
    dbAPI* db;
    InputLayer* il;
    HiddenLayer* hl,* hll;
    OutputLayer* ol;
    LoanPrediction(const char* db_file_path, const char* db_table_name ) 
    {
        db = new dbAPI(db_file_path, db_table_name);
        il = new InputLayer(5, 10);
        hll = new HiddenLayer(10, 10);
        hl = new HiddenLayer(10, 3);
        ol = new OutputLayer(3, 3);
    }
    LoanPrediction() {
        il = new InputLayer(5, 10);
        hll = new HiddenLayer(10, 10);
        hl = new HiddenLayer(10, 3);
        ol = new OutputLayer(3, 3);
    }
    double normalizeData(double data, double min, double max) {
        return ( ( data - min ) / ( max - min ) );
    }

    unsigned int getInfoFromDB(unsigned int current_count) {
        il->neurons[0]->neuron_value = normalizeData(db->selectById__annual_inc(current_count),0,950000);
        il->neurons[1]->neuron_value = normalizeData(db->selectById__funded_amnt(current_count), 1000, 40001);
        il->neurons[2]->neuron_value = normalizeData(db->selectById__funded_amnt_inv(current_count),725,40001);
        il->neurons[3]->neuron_value = normalizeData(db->selectById__installment(current_count),30.64,1424.32);
        il->neurons[4]->neuron_value = normalizeData(db->selectById__int_rate(current_count),6.11,16.91);
        int grade = db->selectById__grade(current_count);
        if(grade == 'C') return 2;
        else if(grade == 'B') return 1;
        return 0;
    }
    char predictLoan(double annual_inc, double funded_amnt, double funded_amnt_inv, double installment, double int_rate) {
        il->neurons[0]->neuron_value = annual_inc;
        il->neurons[1]->neuron_value = funded_amnt;
        il->neurons[2]->neuron_value = funded_amnt_inv;
        il->neurons[3]->neuron_value = installment;
        il->neurons[4]->neuron_value = int_rate;
        hll->forward_propagation(il->neurons, il->nr_of_neurons);
        hl->forward_propagation(hll->neurons, hll->nr_of_neurons);
        ol->forward_propagation(hl->neurons, hl->nr_of_neurons);
        unsigned int result = ol->get_result_position();
        if(result == 0) return 'A';
        else if(result == 1) return 'B';
        return 'C';
    }
    void makeTrain(unsigned int train_count = 2000) {
        unsigned int correct_response = 0, current_response = 0;
        int correct_count = 0;
        int tmp = 0;
        double *out_errors = (double*)malloc(sizeof(double) * hl->nr_of_neurons);
        double *hl_errors = (double*)malloc(sizeof(double) * hll->nr_of_neurons);
        double *hll_errors = (double*)malloc(sizeof(double) * il->nr_of_neurons);
        for(unsigned int i = 1; i<= train_count; i++) {
            correct_response = getInfoFromDB(i);
            hll->forward_propagation(il->neurons, il->nr_of_neurons);
            hl->forward_propagation(hll->neurons, hll->nr_of_neurons);
            ol->forward_propagation(hl->neurons, hl->nr_of_neurons);
            current_response = ol->get_result_position();
            if( current_response == correct_response ) {
                correct_count++;
            }
            else {
                tmp = ol->back_propagation(hl->neurons, hl->nr_of_neurons ,correct_response, out_errors);
                if(tmp == 0) {
                hl->back_propagation(hll->neurons, hll->nr_of_neurons, out_errors, hl_errors);
                hll->back_propagation(il->neurons, il->nr_of_neurons, hl_errors, hll_errors);
                }
            }
            memset(out_errors, 0, sizeof(double) * hl->nr_of_neurons);
            memset(hl_errors, 0, sizeof(double) * hll->nr_of_neurons);
            memset(hll_errors, 0, sizeof(double) * il->nr_of_neurons);
        }
        //printf("Correct: %2.f%%\n", (double)correct_count / (double)train_count * 100.0 );
        free(out_errors);
        free(hl_errors);
        free(hll_errors);
    }
    void saveWeights(const char* filename) {
        FILE* f = fopen(filename, "w+");
        for(size_t i = 0; i < il->nr_of_neurons; i++) {
            for(size_t j = 0; j < il->neurons[0]->nr_of_weights; j++) {
                fprintf(f, "%lf ", il->neurons[i]->weights[j]);
            }
        }
        for(size_t i = 0; i < hll->nr_of_neurons; i++) {
            for(size_t j = 0; j < hll->neurons[0]->nr_of_weights; j++) {
                fprintf(f, "%lf ", hll->neurons[i]->weights[j]);
            }
            fprintf(f, "%lf ", hll->bias_weigths[i]);
        }
        for(size_t i = 0; i < hl->nr_of_neurons; i++) {
            for(size_t j = 0; j < hl->neurons[0]->nr_of_weights; j++) {
                fprintf(f, "%lf ", hl->neurons[i]->weights[j]);
            }
            fprintf(f, "%lf ", hl->bias_weigths[i]);
        }
        for(size_t j = 0; j < ol->neurons[0]->nr_of_weights; j++) {
            fprintf(f, "%lf ", ol->bias_weigths[j]);
        }
        printf("Weights saved!\n");
        fclose(f);
    }
    void loadWeights(const char* filename) {
        FILE* f = fopen(filename, "r");
        for(size_t i = 0; i < il->nr_of_neurons; i++) {
            for(size_t j = 0; j < il->neurons[0]->nr_of_weights; j++) {
                fscanf(f, "%lf", &il->neurons[i]->weights[j]);
                //fscanf(f, " ");
            }
        }
        for(size_t i = 0; i < hll->nr_of_neurons; i++) {
            for(size_t j = 0; j < hll->neurons[0]->nr_of_weights; j++) {
                fscanf(f, "%lf", &hll->neurons[i]->weights[j]);
                //fscanf(f, " ");
            }
            fscanf(f, "%lf", &hll->bias_weigths[i]);
        }
        for(size_t i = 0; i < hl->nr_of_neurons; i++) {
            for(size_t j = 0; j < hl->neurons[0]->nr_of_weights; j++) {
                fscanf(f, "%lf", &hl->neurons[i]->weights[j]);
                //fscanf(f, " ");
            }
            fscanf(f, "%lf", &hl->bias_weigths[i]);
        }
        for(size_t j = 0; j < ol->neurons[0]->nr_of_weights; j++) {
            fscanf(f, "%lf", &ol->bias_weigths[j]);
            //fscanf(f, " ");
        }
        printf("Weights loadet!\n");
        fclose(f);
    }
    char getCharbyPosition(unsigned int position) {
        return grades[position];
    }
    void makeTestPrediction(int id) {
        unsigned int result_from_db = getInfoFromDB(id), current_response = 0;

        printf("Correct result: %c\n", getCharbyPosition(result_from_db));
        hl->forward_propagation(il->neurons, il->nr_of_neurons);
        ol->forward_propagation(hl->neurons, hl->nr_of_neurons);
        current_response = ol->get_result_position();
        printf("Result after prediction: %c\n", getCharbyPosition(current_response));
    }
    ~LoanPrediction() {
        delete db;
        delete il;
        delete hll;
        delete hl;
        delete ol;
    }
};

void test(LoanPrediction* lp){
        int id = 0;
    do {
        scanf("%d", &id);
        lp->makeTestPrediction(id);
    }
    while(id != -1);
}

extern "C" {
    LoanPrediction* LoanPrediction_new() { return new LoanPrediction();}
    void LoanPrediction_loadWeights(LoanPrediction* lp) { lp->loadWeights("weights.dima"); }
    char LoanPrediction_predict(LoanPrediction* lp, double annual_inc, double funded_amnt, double funded_amnt_inv, double installment, double int_rate) {
        return lp->predictLoan( annual_inc,  funded_amnt,  funded_amnt_inv,  installment,  int_rate);
    }
    //void LoanPrediction_delete(LoanPrediction* lp) { delete lp; }
}



int main() {
    srand(time(nullptr));
    LoanPrediction* lp = new LoanPrediction("db/prediction_data.db","train_data");
    //lp->makeTrain();
    //lp->saveWeights("weights.dima");
    lp->loadWeights("weights.dima");
    test(lp);
    //printf("before error\n");
    delete lp;
    //printf("after error\n");
    return 0;
}

