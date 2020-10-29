//#include <iostream>
//#include <fstream>
//#include <algorithm>
//#include <sstream>
#include <sys/stat.h>
#include <bits/stdc++.h>
//#include <string>
#include "mergesort.h"
#include "priorityQ.h"

using namespace std;

#define MAXBUFFSIZE 128

class FileElement {
public:
    int data;
    ifstream *iFile;
    FileElement() {

    }
    FileElement(int num, ifstream *file) {
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
    vector<string> tempFilesVec;
    vector<ifstream*> openTempFilesVec;
    KwayMergeSort(const string &inFile) {
        inputFile = inFile;
    }

    void sortFile() {
        divide_sort();
        conquer_merge();
    }

    void divide_sort() {
        struct stat filestatus;
        stat(inputFile.c_str(), &filestatus);
        ifstream file(inputFile,ifstream::binary);
        size_t chunk_size = MAXBUFFSIZE;
        size_t total_size = filestatus.st_size;
        size_t total_chunks = total_size / chunk_size;
        size_t last_chunk_size = total_size % chunk_size;
        if (last_chunk_size != 0) {
            ++total_chunks; /* add an unfilled final chunk */
        }
        else {
            last_chunk_size = chunk_size;
        }
//        cout << filestatus.st_size << endl;
//        cout << total_chunks << endl;
        string prevnum;
        bool intbroken = false;
        for (size_t chunk = 0; chunk < total_chunks; ++chunk) {
            vector<int> num_vec;
//            cout << "initvec" << num_vec.size() << endl;
            size_t this_chunk_size = chunk == total_chunks - 1 ? last_chunk_size : chunk_size;
            char chunk_data[this_chunk_size];
            file.read(&chunk_data[0], this_chunk_size);
            string chunk_str = string(chunk_data);
//            cout << "###" << this_chunk_size  << endl;
            stringstream ss(chunk_str);
            while (ss.good()) {
                string substr;
                getline(ss, substr, ',');
                if(intbroken) {
//                    cout << "he" << prevnum << "+" << substr << endl;
                    prevnum += substr;
                    substr = prevnum;
                    intbroken = false;
                    prevnum = "";
                }
                stringstream toInt(substr);
                int num = 0;
                toInt >> num;
//                cout << num << ",";
                num_vec.push_back(num);
                prevnum = substr;
            }
            if(chunk < total_chunks-1 && chunk_data[this_chunk_size-1]!=',') {
                intbroken = true;
                prevnum = prevnum.substr(0,prevnum.length()-1);
                num_vec.pop_back();
//                cout << chunk_data[this_chunk_size-1] << "here";
            }
            else if(chunk<=total_chunks-1 && chunk_data[this_chunk_size-1]==',') {
                num_vec.pop_back();
            }
//            cout << endl << "______________" << endl;
//            for(auto i=0;i<num_vec.size();i++) {
//                cout << num_vec[i] << " ";
//            }
//            cout << endl;
//            sort(num_vec.begin(),num_vec.end());
            mergeSort(num_vec,0,num_vec.size()-1);
            write_temp_file(num_vec,chunk);
        }
    }

    void write_temp_file(vector<int> &num_vec,int chunk_no) {
        int pos = inputFile.find_last_of('.');
        string tempfile;
        if (pos != string::npos)
            tempfile = inputFile.substr(0,pos);
        tempfile += "."+to_string(chunk_no)+".tmp";
//        cout << pos << " " << tempfile << endl;
        ofstream *tempop;
        tempop = new ofstream(tempfile.c_str(), ios::out);
        for (size_t i = 0; i < num_vec.size(); ++i) {
            if(i==num_vec.size()-1)
                *tempop << to_string(num_vec[i]);
            else
                *tempop << to_string(num_vec[i]) << "\n";
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
//        priority_queue<FileElement, vector<FileElement>, myComparator> minHeap;
        PriorityQ<FileElement,myComparator> minHeap;
        int line;
        for (size_t i = 0; i < openTempFilesVec.size(); ++i) {
            *openTempFilesVec[i] >> line;
            minHeap.push(FileElement(line,openTempFilesVec[i]));
        }
        while (minHeap.empty() == false) {
            FileElement minElement = minHeap.top();
            *outputFile << minElement.data << endl;
            minHeap.pop();
            *(minElement.iFile) >> line;
            if(*(minElement.iFile)) {
                minHeap.push(FileElement(line,minElement.iFile));
            }
        }
        close_temp_files();
    }

    void open_temp_files() {
        for (size_t i = 0; i < tempFilesVec.size(); ++i) {
            ifstream *file;
            file = new ifstream(tempFilesVec[i].c_str(), ios::in);
            openTempFilesVec.push_back(file);
        }
    }

    void close_temp_files() {
        for (size_t i=0; i < openTempFilesVec.size(); ++i) {
            openTempFilesVec[i]->close();
            delete openTempFilesVec[i];
        }
//        for (size_t i=0; i < tempFilesVec.size(); ++i) {
//            remove(tempFilesVec[i].c_str());
//        }
    }
};

int main() {
    KwayMergeSort *kwms = new KwayMergeSort("/home/mansi/IIITH/DSA/assgn3/kwaysort/input.txt");
    kwms->sortFile();
//    ifstream bigFile("mybigfile.dat");
//    constexpr size_t bufferSize = 1024;
//    unique_ptr<char[]> buffer(new char[bufferSize]);
//    while(bigFile)
//    {
//        bigFile.read(buffer.get(), bufferSize);
//        // process data in buffer
//    }
    return 0;
}
