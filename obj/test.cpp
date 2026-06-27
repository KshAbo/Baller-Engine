#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <vector>
using namespace std;

struct vertex{
    float x, y, z;
};

int main (int argc, char *argv[]) {
    vector<vertex> Vertex;
    vector<vertex> VertexNormal;
    fstream ifstream;
    ifstream.open("cube.obj");
    stringstream sstream;
    sstream << ifstream.rdbuf();
    ifstream.close();
    string str;
    while(getline(sstream, str)){
        stringstream streamline(str);
        string word;
        while(getline(streamline, word, ' ')){
            if(word == "v"){
                cout << word;
                GLfloat a, b, c;
                streamline >> a >> b >> c;

                cout<< a << " " << b << " "<< c;
                Vertex.push_back({a,b,c});
            }
            else if(word == "vn"){
                GLfloat a, b, c;
                streamline >> a >> b >> c;
                cout << word;
                cout << a << " " << b << " "<< c;
                VertexNormal.push_back({a,b,c});
            }
            else if(word == "f"){
                cout << word;
                for(int i = 0; i < 3; i++){
                    getline(streamline, word, ' ');
                    cout << word << endl;
                    stringstream indexstream(word);
                    while(getline(indexstream, word, '/')){
                        cout << word << " ";
                    }
                    cout << "\t";
                }
            }
        }
        cout << endl;
    }
    return 0;
}
