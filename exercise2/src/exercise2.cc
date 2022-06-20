#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>


using namespace std;

void printPath(float adjgraph[11][11], int n);

int main(int argc, char** argv){

    string fname = argv[1];
    //string fname = "C:/Users/User/CLionProjects/untitled2/input2.csv";

    int source = atoi(argv[2]);
    vector<vector<string>> content;
    vector<string> row;
    string line, word;


    fstream file (fname, ios::in);
    if(file.is_open())
    {
        while(getline(file, line))
        {
            row.clear();

            stringstream str(line);

            while(getline(str, word, ','))
                row.push_back(word);
            content.push_back(row);
        }
    }
    else
        cout<<"Could not open the file\n";

    //printing adjmatrix
    for(int i=0;i<content.size();i++)
    {
        for(int j=0;j<content[i].size();j++)
        {
            if(j < content[i].size()-1)
                cout<<content[i][j]<<",";
            else
                cout<<content[i][j]<<"";
        }
        cout<<"\n";
    }

    float adjgraph[11][11];
    for(int i = 0; i< content.size();i++){
        for(int j = 0; j < content[i].size();j++){
            adjgraph[i][j] = stof(content[i][j]);}
    }

    printPath(adjgraph,source);
}

void printPath(float adjgraph[11][11], int n){
    string path = "";
    bool arr[] = {false,false,false,false,false,false,false,false,false,false,false};  // array to check whether vertive is chosen before
    bool condition = true;
    while(condition){
        if(adjgraph[n][n] != 0 ){
            path = path + to_string(n);
            condition = false;
            cout<< path;}
        else{
            path = path + to_string(n)+ " ";
            arr[n] = true;
            float min = 1;
            int index;
            for(int i =0; i<11;i++){
                if(adjgraph[n][i] < min && adjgraph[n][i] > 0 && arr[i] == false){
                    min = adjgraph[n][i];
                    index = i;
                }
            }
            n = index;
        }
    }
}