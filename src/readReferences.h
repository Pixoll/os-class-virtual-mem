#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;
//Esto es para leer las references el .txt parte 2.4)
vector<int> readReferences(const string &filename) {
  ifstream ifs(filename);
  vector<int> refs;
  int num;

  if (!ifs.is_open()) {
    cerr << "Error al abrir el archivo: " << filename << endl;
    exit(1);
  }

  cout << "Leyendo archivo: " << filename << endl;

  while (ifs >> num) {
    refs.push_back(num);
  }

  return refs;
}
