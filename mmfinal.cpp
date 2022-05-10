// v 3.1 (without debugging)
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <cmath>
#include <ctime>
 
void set_random_seed();
int randn(int n);
void next_num (std::vector<int>& result, int& occ_num, int& test_num, std::vector<int> poss, int size);
void new_black (std::vector<int>& end, int test_num, int& index1, int& occ_num, int&tblack);
 
struct mm_code_maker{
 
    void init(int i_length, int i_num){
        length = i_length;
        num = i_num;
    }
 
    void generate_sequence(){
        for(int i = 0; i < length; i++){
            sequence.push_back(randn(num));
        }
    }
 

    void give_feedback(const std::vector<int>& attempt, int& black_hits, int& white_hits){
        
    black_hits = 0;
    white_hits = 0;
    std::vector<int> occ_att;
    std::vector<int> occ_seq;

    for(int i = 0; i < num; i++){
      occ_att.push_back(0);
      occ_seq.push_back(0);
    }

    for(int j = 0; j < length; j++){
        occ_att[attempt[j]] ++;
        occ_seq[sequence[j]] ++;
      
      if(attempt[j] == sequence[j]){
        black_hits++;
      }
    }
    
    int hits = 0;
    
    for(int k = 0; k < num; k++) {
        
      if(occ_att[k]> occ_seq[k]){
        hits += occ_seq[k];
      }
      else{
        hits += occ_att[k];
      }
    }
    white_hits = hits - black_hits;
    }
 
    std::vector<int> sequence;
 
    int length;
    int num; 
};
 
struct mm_solver{
 
    void init(int i_length, int i_num){
        length = i_length;
        num = i_num;
        first_learn = true;
        first_try = true;
        total_num = pow(num, length);
        num_poss = 0;
    }
 
    void create_attempt(std::vector<int>& attempt){
        
       int max_num = pow(8, 7) +1;
       if(total_num < max_num) {
            if(first_try == true) {
                for(int i = 0; i < (length/2); i++){
                attempt.push_back(1);
                tried.push_back(1);
                }
                for(int j = 0; j <= (length - attempt.size()); j++) {
                    attempt.push_back(2);
                    tried.push_back(2);
                }
            first_try = false;
            }
            
            else {
                attempt.clear();
                for(int j = 0; j < length; j++) {
                    attempt.push_back(choices[j]);
                    tried.push_back(choices[j]);
                }
            }
        }
    
        else {
            if(first_try == true) {
                num_test = 0;
                num_occ = 0;
                first_time = true;
                for(int i =0; i <length; i++) {
                    attempt.push_back(0);
                    final.push_back(0);
                }
            first_try = false;
            }
            
            else {
                attempt.clear();
                for(int j = 0; j < length; j++) {
                    attempt.push_back(choices[j]);
                }
            } 
        }
    }

 
   
    void learn(std::vector<int>& attempt, int black_hits, int white_hits){
      
    int max_num = pow(8, 7) +1;
    if(total_num < max_num) {
        if(first_learn == true) {
            choices.clear();
            // creates set of every possible outcome 
        for(int i =0; i < total_num; i++) {
            possibilities.clear();
            int power = i;
            
            // push back every number in the set
            while(power!=0) {
                possibilities.push_back(power%num);
                power /= num;
            }
            
            // add leading zeroes
            int zeroes = length - possibilities.size();
            while (zeroes != 0) {
                possibilities.push_back(0);
                zeroes --;
            }
            
            int test_black = 0;
            int test_white = 0;
            std::vector<int> freq_poss;
            std::vector<int> freq_att;

            for(int i = 0; i < num; i++){
            freq_att.push_back(0);
            freq_poss.push_back(0);
            }

            for(int j = 0; j < length; j++){
                freq_poss[possibilities[j]] ++;
                freq_att[attempt[j]] ++;
            
            if(possibilities[j] == attempt[j]){
                test_black++;
            }
            }
            
            int test_hits = 0;
            
            for(int k = 0; k < num; k++) {
                
            if(freq_poss[k]> freq_att[k]){
                test_hits += freq_att[k];
            }
            else{
                test_hits += freq_poss[k];
            }
            }
            test_white = test_hits - test_black;
            
            if(test_black == black_hits && test_white == white_hits) {
                    for(int m=0; m < length; m++) {
                        choices.push_back(possibilities[m]);
                    }
            }
        }
        first_learn = false;
        }
    
        else {
            possibilities.clear();
            // creates set of every possible outcome 
        for(int i =0; i < choices.size(); i++) {
            possibilities.push_back(choices[i]);
            }
            choices.clear();
            
            for(int w = 0; w < possibilities.size() ; w+= length) {
                int test_black = 0;
                int test_white = 0;
                std::vector<int> freq_poss;
                std::vector<int> freq_att;

                for(int i = 0; i < num; i++){
                freq_att.push_back(0);
                freq_poss.push_back(0);
                }

                for(int j = 0; j < length; j++){
                    freq_poss[possibilities[w+j]] ++;
                    freq_att[attempt[j]] ++;
                
                if(possibilities[w+j] == attempt[j]){
                    test_black++;
                }
                }
                
                int test_hits = 0;
                
                for(int k = 0; k < num; k++) {
                    
                if(freq_poss[k]> freq_att[k]){
                    test_hits += freq_att[k];
                }
                else{
                    test_hits += freq_poss[k];
                }
                }
                test_white = test_hits - test_black;
            
            if(test_black == black_hits && test_white == white_hits) {
                bool used = true;
                    for(int a=0; a < length; a++) {
                        for(int b=0; b <= (tried.size() / length); b++) {
                            if(possibilities[w+a] != tried[b*length]) {
                                used = false;
                            }
                        }
                    }
                    if(used == false) {
                        for(int m=0; m < length; m++) {
                            choices.push_back(possibilities[w+m]);
                            
                        }
                    }
                }
            }
        }
    }
    
    else{
        if(first_time == true) {
            blackh = black_hits;
            first_time = false;
            total_black = blackh;
            for(int i=0; i < length; i++) {
                final.push_back(0);
            }
//             record number of black pegs for 0
        }
        
//         test a number full of the digit for every digit possible (0 to num-1)
        if(num_poss < (num-1)) {
            // num_poss initialized at 0
            for(int j=0; j < black_hits; j++) {
                possibilities.push_back(num_poss);
//                 put in vector possibilities the digit the number of times it is in the actual code (num_poss is inserted black_hits times)
            }
            num_poss ++;
            choices.clear();
            for(int i=0; i < length; i++) {
                choices.push_back(num_poss);
            }
        }
       
//        after we have tested all the full number we have in possibilities all the numbers that appear in the actual but in random positions.
        else {
        int avance = 0 ;    
        
        if(num_poss == (num-1)) {
            for(int j=0; j < black_hits; j++) {
                possibilities.push_back(num_poss);
            }
            black_hits = blackh;
            num_poss ++;
            for(int i=0; i < length; i++) {
                std::cout << possibilities[i] << " ";
            }
            std::cout << std::endl;
        }
        
//             if occurancy is 0 we increase the number to test
            if(num_occ == 0) {
                first_learn = true;
            }
            
            if(total_black == length) {
                first_learn = false;
                choices.clear();
                for(int k=0; k < length; k++) {
                    choices.push_back(final[k]);
                }
            }  
            
//             count the number of times a number appears in the code and create choices full of 0
            if(first_learn == true) {
                next_num (choices, num_occ, num_test, possibilities, length);
                first_learn = false;
                index = length;
            }
            
            
//             if we have some occurancy 
            if(num_occ > 0) {
//                 if the number of black pegs increased that means the number is in the correct postion so we put it in the final vector, we reduce the index so it is gonna be tested somewhere else, decrease number occurancy as we found one and increase the total number of black pegs by one.
                if(black_hits > blackh) {
                    new_black (final, num_test, index, num_occ, total_black);
                    
                }
                
                else {
                    index --;
                }
            }
          
          
//           we want to test only on indexes that are empty in final so while it is not the case we increase the value of index
            if(index != 0) {
                while(final[index] != 0) {
                    index --;
                    avance ++;
                }
                
            }
        
            
//             we apply the number to test in choices and put a 0 back where the number was previously.
            if(total_black != length) {
                if(num_occ == 0) {
                    next_num (choices, num_occ, num_test, possibilities, length);
                    first_learn = false;
                    index = (length-1);
                } 
                
                choices[index] = num_test;
                choices[index+ 1 + avance] = 0;
            }
         
//          calling it more time here remove a dummy attempt at the end before giving the actual solution
            if(total_black == length) {
                first_learn = false;
                choices.clear();
                for(int k=0; k < length; k++) {
                    choices.push_back(final[k]);
                }
            }
            
        }
    }
    }
        
    bool first_try;
    bool first_learn;
    bool first_time;
    
    int blackh;
    int index;
    int num_poss;
    int num_occ;
    int num_test;
    int total_black;
    
    std::vector <int> possibilities;
    std::vector <int> choices;
    std::vector<int> tried;
    std::vector<int> final;
 
    int length;
    int num;
    double total_num;
 
};
 
/// the main is the same as in the previous example
 
int main(){
 
    clock_t start = clock();
    
    set_random_seed();
 
    int length, num;
    std::cout << "enter length of sequence and number of possible values:" << std::endl;
    std::cin >> length >> num;
 
    mm_solver solver;
    solver.init(length, num);
 
    mm_code_maker maker;
    maker.init(length, num);
    maker.generate_sequence();
 
    int black_hits=0, white_hits=0;
    int attempts_limit = 5000;
    int attempts = 0;
    while((black_hits < length) && (attempts < attempts_limit)){
        std::vector<int> attempt;
        solver.create_attempt(attempt);
        maker.give_feedback(attempt, black_hits, white_hits);
        std::cout << "attempt: " << std::endl;
        for(int i = 0; i < attempt.size(); i++){
            std::cout << attempt[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "black pegs: " << black_hits << " " << " white pegs: " << white_hits << std::endl;
        solver.learn(attempt, black_hits, white_hits);
        attempts++;
    }
 
    if(black_hits == length){
        std::cout << "the solver has found the sequence in " << attempts << " attempts" << std::endl;        
    }
    else{
        std::cout << "after " << attempts << " attempts still no solution" << std::endl;
    }
    std::cout << "the sequence generated by the code maker was:" << std::endl;
    for(int i = 0; i < maker.sequence.size(); i++){
        std::cout << maker.sequence[i] << " ";
    }
    std::cout << std::endl;
    clock_t elapsed = clock() - start;
    double time = elapsed / (double) CLOCKS_PER_SEC;
    std::cout << "Time to run: " << time << " seconds" << std::endl;
    return 0;
}
 
void set_random_seed(){
    std::srand(std::time(0));
}
 
int randn(int n){
    return std::rand() % n;
}

void next_num (std::vector<int>& result, int& occ_num, int& test_num, std::vector<int> poss, int size) {
    result.clear();
    occ_num = 0;
    while(occ_num == 0) {
        test_num ++;
        if(test_num != 0) {
            for(int i = 0; i < size; i++) {
                result.push_back(0);
                if(poss[i] == test_num) {
                    occ_num ++;
                }
            }
        }
    }
}

void new_black (std::vector<int>& end, int test_num, int& index1, int& occ_num, int&tblack) {
    end[index1] = test_num;
    index1 --;
    occ_num --;
    tblack ++;
}
