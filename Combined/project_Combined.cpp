-6#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<bits/stdc++.h>
#include<map>
#include<chrono>
#include<omp.h>
using namespace std;
using namespace std::chrono;

vector<vector<string>> Read_File(string file_name)
{
    ifstream file;
    file.open(file_name);
    string line;
    vector<vector<string>> Data;
    int i = 0 ;

    while(!file.eof())
    {
        getline(file, line);
        string word;
        stringstream stream_line(line); 
        vector<string> Single_Row;
        while(getline(stream_line, word, ','))
        {
            Single_Row.push_back(word);       
        }
        Data.push_back(Single_Row);
        sort(Data[i].begin(), Data[i].end());
        i++;
    }    

    return(Data);
}

map<string, int> Frequency_Single_Parallel(vector<vector<string>> Data)
{
    map<string, int> Frequency;
    
   #pragma omp parallel  num_threads(4)
    {      
        #pragma omp for nowait
        for (int i = 0; i < Data.size(); i++)
        {
            for (int j = 0; j < Data[i].size(); j++)
            {
                
                if (Frequency[Data[i][j]] == 0)
                {
                    
                    Frequency[Data[i][j]] = 1;
                }
                else
                {
                    Frequency[Data[i][j]] += 1;
                }
            
            }
            
        }
    }

    return (Frequency);

}


map<vector<string>, int> Frequency_Combination_Parallel(vector<vector<string>> Data, vector<vector<string>> Combinations)
{
    map<vector<string>, int> Frequency;
    int i;
    #pragma omp parallel for collapse(2) num_threads(4)
    for (i = 0; i < Data.size(); i++)
    {   
     
        for (int j = 0; j < Combinations.size(); j++)
        {
            if(includes(Data[i].begin(), Data[i].end(), Combinations[j].begin(), Combinations[j].end()))
            {
                if (Frequency[Combinations[j]] == 0)
                {
                    Frequency[Combinations[j]] = 1;
                }
                else
                {
                    Frequency[Combinations[j]] += 1;
                }
            }
            
        }
        
    }


    return (Frequency);

}



map<string, int> Frequency_Single(vector<vector<string>> Data)
{
    map<string, int> Frequency;
    for (int i = 0; i < Data.size(); i++)
    {
        for (int j = 0; j < Data[i].size(); j++)
        {
            
            if (Frequency[Data[i][j]] == 0)
            {
                Frequency[Data[i][j]] = 1;
            }
            else
            {
                Frequency[Data[i][j]] += 1;
            }
        }      
        
    }

    return (Frequency);

}


map<vector<string>, int> Frequency_Combination(vector<vector<string>> Data, vector<vector<string>> Combinations)
{
    map<vector<string>, int> Frequency;

    for (int i = 0; i < Data.size(); i++)
    {
     
        for (int j = 0; j < Combinations.size(); j++)
        {
            
            if(includes(Data[i].begin(), Data[i].end(), Combinations[j].begin(), Combinations[j].end()))
            {
                if (Frequency[Combinations[j]] == 0)
                {
                    Frequency[Combinations[j]] = 1;
                }
                else
                {
                    Frequency[Combinations[j]] += 1;
                }
            }
                
        }
        
    }


    return (Frequency);

}













void Generate_Combinations(vector<string> Frequent_List, int pairs)
{
    ofstream combination_file;
    combination_file.open("combination.txt");
    combination_file << pairs << endl;
    for(auto i : Frequent_List)
        combination_file << i << endl;
    
    combination_file.close();
    system("python3 combination.py");
   
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
       std::cout << "Please give proper arguement " << endl;
       exit(0);

    }

    int pairs = 2;
    string file_name;


    vector<vector<string>>Data;
    
    if (argv[1][0] == '1')
    file_name = "groceries.txt";
    else if (argv[1][0] == '2')
    file_name = "groceries_big.txt";
    else if (argv[1][0] == '3')
    file_name = "groceries_big0.txt";
    else
    {
        std::cout << "Invalid Input" << endl;
        exit(0);
    }
    

    map<vector<string>, int> Combination_Frequencies;	//declaring a map type variable named combination frequencies
    vector<vector<string>> Combinations;			//declaring a string type dynamic 2d array named combinations

    int line_count = 0;
    string line;
    ifstream file(file_name);
    while(getline(file, line)) line_count ++;
    int Count;
    std::cout << endl << "Total Records : " << line_count << endl;
    std::cout << endl << "Minimum Frequency Count : ";
    cin >> Count;
    
    file.close();
    Data = Read_File(file_name);
    
    std::cout << endl << endl << "================================================== Parallel Exection Started =======================================" << endl << endl ;
    // ================================================== Parallel Code ====================================================================

    auto start = high_resolution_clock::now(); 
    // int Total_Records = Data.size();
    bool Flag = 0;
    vector<string> Frequent_List;
    
    map<string, int> Frequency = Frequency_Single_Parallel(Data);
    
    #pragma openmp parallell for num_threads(4) shared(Flag)
    for(auto i : Frequency)
    {
        if(i.second >= Count)
        {
            if(!Flag) {std::cout << endl << "Frequent Items according Minimum Frequency Criteria are : " << endl; std::cout << "Frequent Combinations are : " << endl; std::cout << "================================================" << endl << endl; Flag = 1;} 
            Frequent_List.push_back(i.first);
            std::cout << i.first << " : " << i.second << endl;
        }
    
            
    }
    Frequency.clear();

    if(Frequent_List.size() == 1)
    {   
        std::cout << "Only 1 item fulfill the Minimum Frequency Criteria : " << endl << endl;
        std::cout << "Result : " << Frequent_List[0] << endl;
    }
    else if(Frequent_List.size() == 0)
    {
        std::cout << "No item can fulfill the Minimum Frequency Criteria" << endl;
    }
    else if(Frequent_List.size() == pairs)
    {
        std::cout << endl << "Above 2 items are frequent based on Minimum Frequency Criteria " << endl; 
    }
    else
    {
        
            
        while (true)
        {
            Generate_Combinations(Frequent_List, pairs);
            // Combinations.clear();
            Combinations = Read_File("Results.txt");
            
            Combination_Frequencies = Frequency_Combination_Parallel(Data, Combinations);

            Frequent_List.clear();
            Frequent_List.shrink_to_fit();
            Flag = 0;
            Combinations.clear();
            
            #pragma openmp parallel for num_threads(4) shared(Flag)
            {
            for(auto i : Combination_Frequencies)
            {
        
                if(i.second >= Count)
                {
                    if(!Flag){ std::cout << endl << "Frequent Combinations are : " << endl; std::cout << "===========================" << endl << endl; Flag = 1;}
        
                    for(auto j : i.first)
                    {
                            std::cout << j <<  endl;
                            if(find(Frequent_List.begin(), Frequent_List.end(),j) == Frequent_List.end())
                                Frequent_List.push_back(j);
                   
                    }
                    std::cout << endl << "Count : " << i.second << endl << endl;
                    
                }
            }
            }
            
            
            Combination_Frequencies.clear();
            // Combination_Frequencies.shrink_to_fit();

            if(Flag == 0)
            {
                std::cout << "No Further Combinations can fulfill the Minimum Frequency Criteria" << endl << endl; 
                break;
            }
            else if (Frequent_List.size() == pairs)
            {
                break;
            }
            
            pairs++;
            
            if (Frequent_List.size() == pairs)
            {
                std::cout << endl << "Above Combinations are the Final Results" << endl;
                break;
            }
            
            

        }



    }
    
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 
    float time_taken_Parallel = duration.count()/ 1000; 



    std::cout << endl << endl << "================================================== Parallel Exection Ended =======================================" << endl << endl ;
    
    
    
    std::cout << endl << endl << "================================================== Serial Exection Started =======================================" << endl << endl ;
    

    Frequent_List.clear();
    Frequent_List.shrink_to_fit();



    start = high_resolution_clock::now(); 
    // Data = Read_File(file_name);
    // int Total_Records = Data.size();
    Flag = 0;
    // vector<string> Frequent_List;
    pairs  = 2;

    Frequency = Frequency_Single(Data);


    for(auto i : Frequency)
    {
        if(i.second >= Count)
        {
            if(!Flag) {std::cout << endl << "Frequent Items according Minimum Frequency Criteria are : " << endl; std::cout << "Frequent Combinations are : " << endl; std::cout << "================================================" << endl << endl; Flag = 1;} 
            Frequent_List.push_back(i.first);
            std::cout << i.first << " : " << i.second << endl;
        }
            
    }
    Frequency.clear();
    if(Frequent_List.size() == 1)
    {   
        std::cout << "Only 1 item fulfill the Minimum Frequency Criteria : " << endl << endl;
        std::cout << "Result : " << Frequent_List[0] << endl;
    }
    else if(Frequent_List.size() == 0)
    {
        std::cout << "No item can fulfill the Minimum Frequency Criteria" << endl;
    }
    else if(Frequent_List.size() == pairs)
    {
        std::cout << endl << "Above 2 items are frequent based on Minimum Frequency Criteria " << endl; 
    }
    else
    {
        
            
        while (true)
        {
            Generate_Combinations(Frequent_List, pairs);
            Combinations = Read_File("Results.txt");
            
            Combination_Frequencies = Frequency_Combination(Data, Combinations);
            Frequent_List.clear();
            Frequent_List.shrink_to_fit();
            Combinations.clear();

            Flag = 0;
            for(auto i : Combination_Frequencies)
            {
                if(i.second >= Count)
                {
                    if(!Flag){ std::cout << endl << "Frequent Combinations are : " << endl; std::cout << "===========================" << endl << endl; Flag = 1;}
                    for(auto j : i.first)
                    {
                        std::cout << j <<  endl;
                        if(find(Frequent_List.begin(), Frequent_List.end(),j) == Frequent_List.end())
                            Frequent_List.push_back(j);
                    }
                    std::cout << endl << "Count : " << i.second << endl << endl;
                    
                }
            }
            
            
            if(Flag == 0)
            {
                std::cout << "No Further Combinations can fulfill the Minimum Frequency Criteria" << endl << endl; 
                break;
            }
            else if (Frequent_List.size() == pairs)
            {
                break;
            }
            
            pairs++;
            
            if (Frequent_List.size() == pairs)
            {
                std::cout << endl << "Above Combinations are the Final Results" << endl;
                break;
            }
            
            

        }
        
    }

    std::cout << endl << endl << "================================================== Serial Exection Ended =======================================" << endl << endl ;
    

    stop = high_resolution_clock::now(); 
    duration = duration_cast<microseconds>(stop - start);
    float time_taken_Sequential = duration.count()/ 1000;








    std::cout << "Time Taken by the Serial Execution : " << (time_taken_Sequential/1000) << " seconds "<< endl; 
    std::cout << "Time Taken by the Parallel Execution : " << (time_taken_Parallel/1000) << " seconds "<< endl; 

    ofstream result;
    result.open("../Graph_Results.txt");
    result << "Sequential" << endl << time_taken_Sequential << endl << "Parallel" << endl << time_taken_Parallel;
    result.close();

    system("python3 ../Graph.py");

}
