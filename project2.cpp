#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <limits>
#include <cmath>
using namespace std;

const int R = 2000; //rows
const int C = 51; //cols
double data[R][C]; //data set

void get_data() //get data from file into data set
{
    fstream file;
    file.open("large.txt", ios::in);
    double val;
    
    for(int i = 0; i < R; i++)
    {
        for( int j = 0; j < C; j++)
        {
            file >> val;
            data[i][j] = val;
        }
    }
    file.close();
}

double loocv(vector<int> set, int ft) //leave one out cross validation
{
    double nn_dist; //nearest neighbor distance
    int nn_col; //location of nearest neighbor;
    double dist, sum, nn_class;
    int num_cc = 0; //number of correctly classified
    
   for(int i = 0; i < R; i++)
   {
       nn_dist = std::numeric_limits<float>::max();
       nn_col =  std::numeric_limits<int>::max();
       
       for(int j = 0; j < R; j++)
       {
           if(j != i) //don't compare to self
           {
                sum = 0;
                for(int k = 0; k < set.size(); k++) //loop through all features in current set
                    sum += (data[i][set[k]] - data[j][set[k]]) * (data[i][set[k]] - data[j][set[k]]);
                    
                sum += (data[i][ft] - data[j][ft]) * (data[i][ft] - data[j][ft]); //add feature to add to sum
                
                dist = sqrt(sum);
                if(dist < nn_dist)
                {
                    nn_dist = dist;
                    nn_col = j;
                    nn_class = data[j][0];
                }
           }
       }
       
       if(data[i][0] == nn_class)
            num_cc++;
   }
   double accuracy = 1.0*num_cc / R;
    
    return accuracy;
}

void print_set(vector<int> set)
{
    cout << "{";
    for(int i = 0; i < set.size(); i++)
    {
        cout << set[i];
        if(i != set.size() - 1) //don't put comma at the end of set
            cout << ",";
    }
        
    cout << "}";
}

int main()
{
    get_data();
    vector<int> csf; //current set of features
    double accuracy, best_accuracy;
    int feature_to_add;
    
    cout << "Beginning search" << endl;
    for(int i = 1; i < C; i++)
    {
        best_accuracy = 0;
        
        for(int j = 1; j < C; j++)
        {
            if(!(find(csf.begin(), csf.end(), j) != csf.end())) //only add if not already added
            {
                //cout<<"\tAddign feature " << j;
                accuracy = 100*loocv(csf, j);
                //cout << " accurracy: " << accuracy << "%" << endl;
                
                if(accuracy > best_accuracy)
                {
                    best_accuracy = accuracy;
                    feature_to_add = j;
                }
                
            }
            
        }
        
        csf.push_back(feature_to_add); //add this feature to current set
        cout << "Set "; 
        print_set(csf);
        cout << " was best, accuracy is " << best_accuracy << "%" << endl << endl;
    }
    

    return 0;
}

