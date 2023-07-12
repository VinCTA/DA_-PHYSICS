#include <iostream>
#include <fstream>
#include <string>
int main ()
{const char* filename = "File.txt"; // dichiaro il caarattere  File.txt che si chiama filename 
 std::ofstream OutFile(filename);//dichiara una variabile di tipo file e apre il file di nome filename
 
 OutFile << parola << std::endl;
 OutFile.close(); // Chiude il file
}