#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <vector>
#include <cstdio>
#include "mergesort.h"
#include "priorityQ.h"

#define ll long long int

using namespace std;

#define MAXBUFFSIZE 1024*1024*2

class FileElement {
public:
    ll data;
    ifstream *iFile;
    FileElement() {

    }
    FileElement(ll num, ifstream *file) {
        data = num;
        iFile = file;
    }
};

class myComparator {
public:
    int operator()(const FileElement a, const FileElement b) const{
        if(a.data > b.data)
            return 1;
        else if(a.data < b.data)
            return -1;
        else
            return 0;
    }
};

class KwayMergeSort {
private:

public:
    string inputFile;
    string outputFile;
    vector<string> tempFilesVec;
    vector<ifstream*> openTempFilesVec;
    KwayMergeSort(string inFile, string outFile) {
        inputFile = inFile;
        outputFile = outFile;
    }

    void sortFile() {
        divide_sort();
        if(!tempFilesVec.empty())
            conquer_merge();
    }

    void divide_sort() {
        struct stat filestatus;
        stat(inputFile.c_str(), &filestatus);
        ifstream file(inputFile,ifstream::binary);
        ll chunk_size = MAXBUFFSIZE;
        ll total_size = filestatus.st_size;
        ll total_chunks = total_size / chunk_size;
        ll last_chunk_size = total_size % chunk_size;
        if (last_chunk_size != 0) {
            ++total_chunks; /* add an unfilled final chunk */
        }
        else {
            last_chunk_size = chunk_size;
        }
        string prevnum;
        bool intbroken = false;
        for (ll chunk = 0; chunk < total_chunks; ++chunk) {
            vector<ll> num_vec;
            ll this_chunk_size = chunk == total_chunks - 1 ? last_chunk_size : chunk_size;
            char chunk_data[this_chunk_size];
            file.read(&chunk_data[0], this_chunk_size);
            string chunk_str = string(chunk_data);
            stringstream ss(chunk_str);
            while (ss.good()) {
                string substr;
                getline(ss, substr, ',');
                if(intbroken) {
                    prevnum += substr;
                    substr = prevnum;
                    intbroken = false;
                    prevnum = "";
                }
                stringstream toInt(substr);
                ll num = 0;
                toInt >> num;
                num_vec.push_back(num);
                prevnum = substr;
            }
            if(chunk < total_chunks-1 && chunk_data[this_chunk_size-1]!=',') {
                intbroken = true;
                prevnum = prevnum.substr(0,prevnum.length()-1);
                num_vec.pop_back();
            }
            else if(chunk<=total_chunks-1 && chunk_data[this_chunk_size-1]==',') {
                num_vec.pop_back();
            }
            mergeSort(num_vec,0,num_vec.size()-1);
            if(total_chunks>1) {
                write_temp_file(num_vec, chunk);
            }
            else {
                write_single_output_file(num_vec);
            }
        }
    }

    void write_single_output_file(vector<ll> &num_vec) {
        ofstream *tempop;
        tempop = new ofstream(outputFile.c_str(), ios::out);
        for (ll i = 0; i < num_vec.size(); ++i) {
            if(i==num_vec.size()-1)
                *tempop << num_vec[i];
            else
                *tempop << to_string(num_vec[i]) << ",";
        }
        tempop->close();
        delete tempop;
    }

    void write_temp_file(vector<ll> &num_vec,ll chunk_no) {
        int pos = inputFile.find_last_of('.');
        string tempfile;
        if (pos != string::npos)
            tempfile = inputFile.substr(0,pos);
        tempfile += "."+to_string(chunk_no)+".tmp";
        ofstream *tempop;
        tempop = new ofstream(tempfile.c_str(), ios::out);
        for (ll i = 0; i < num_vec.size(); ++i) {
            if(i==num_vec.size()-1)
                *tempop << num_vec[i];
            else
                *tempop << num_vec[i] << "\n";
        }
        tempop->close();
        delete tempop;
        tempFilesVec.push_back(tempfile);
    }

    void conquer_merge() {
        open_temp_files();
        int pos = inputFile.find_last_of('/');
        string path;
        if (pos != string::npos )
            path = inputFile.substr(0,pos);
        string outPath = path+"/output.txt";
        cout << outPath << endl;
        ofstream *outputFile = new ofstream(outPath.c_str(), ios::out);
        PriorityQ<FileElement,myComparator> minHeap;
        ll line;
        for (ll i = 0; i < openTempFilesVec.size(); ++i) {
            *openTempFilesVec[i] >> line;
            minHeap.push(FileElement(line,openTempFilesVec[i]));
        }
        while (minHeap.empty() == false) {
            FileElement minElement = minHeap.top();
            if(minHeap.size()==1) {
                *outputFile << minElement.data;
            }
            else {
                *outputFile << minElement.data << ",";
            }
            minHeap.pop();
            *(minElement.iFile) >> line;
            if(*(minElement.iFile)) {
                minHeap.push(FileElement(line,minElement.iFile));
            }
        }
        close_temp_files();
    }

    void open_temp_files() {
        for (ll i = 0; i < tempFilesVec.size(); ++i) {
            ifstream *file;
            file = new ifstream(tempFilesVec[i].c_str(), ios::in);
            openTempFilesVec.push_back(file);
        }
    }

    void close_temp_files() {
        for (ll i=0; i < openTempFilesVec.size(); ++i) {
            openTempFilesVec[i]->close();
            remove(tempFilesVec[i].c_str());
            delete openTempFilesVec[i];
        }
    }
};

int main(int argc,char ** argv) {
    if(argc<3) {
        cout << "Usage : main.cpp <input_file> <output_file>" << endl;
        return 0;
    }
    KwayMergeSort *kwms = new KwayMergeSort(argv[1],argv[2]);
    kwms->sortFile();
    return 0;
}