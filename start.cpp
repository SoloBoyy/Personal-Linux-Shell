#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <algorithm>
using namespace std;

string remove_whitespaces(string str)
{
 
 const auto strBegin = str.find_first_not_of(" \t");
 if (strBegin == string::npos)
 return ""; // no content

 const auto strEnd = str.find_last_not_of(" \t");
 const auto strRange = strEnd - strBegin + 1;

 str = str.substr(strBegin, strRange); 

 string::iterator new_end = 
 unique(str.begin(), str.end(),
 [=](char lhs, char rhs){ return (lhs == rhs) && (lhs == ' '); }
 );
 str.erase(new_end, str.end()); 

 return str;


}
char** vec_to_char_array(vector<string>& x)
{
 char** arr = new char* [x.size() + 1]; 
 for(int i = 0; i < x.size(); i++)
 {
 arr[i] = (char*) x[i].c_str(); 
 }
 arr [x.size()] = NULL;
 return arr;
}
vector<string> split(string input, char sep)
{ 
 int number = 0;
 string temporary;
 vector<string> vector;
 int i = 0;
 for(int x = 0; x < input.size(); x++)
 {
 char c = input[x];
 bool bs = (c == sep);
 bool bs1 = (number != 1);
 if(bs1 && bs)
 {
 if(c == sep)
 {
 string t = remove_whitespaces(temporary);
 temporary = t;
 vector.push_back(temporary);
 temporary = "";
 if(number == 2)
 number = 0; 
 }
 
 }
 else
 {
 temporary += c;
 bool bs2 = (c == '\"');
 bool bs3 = (c == '\'');
 if(bs2 || bs3)
 number+=1;
 }
 }
 if(temporary != "")
 vector.push_back(temporary);
 
 return vector;
}
void quote_checker (string inputLine)
{
 inputLine = remove_whitespaces(inputLine);
 
 //Checking single and double quotes
 
 string temp = "";
 int location = inputLine.find("'");
 int location2 = inputLine.find("\"");

 if(location != string::npos)
 {
 int first = location + 1;
 int second = inputLine.size() - location - 2;
 temp = inputLine.substr(first, second);
 cout << temp << endl;
 }
 else if(location2 != string::npos)
 {
 int first = location2 + 1;
 int second = inputLine.size() - location2 - 2;
 temp = inputLine.substr(first, second);
 cout << temp << endl;
 } 
}
void directory_handler (vector<string>& previousDirectory, string inputLine)
{
 char* prevPWD = new char [FILENAME_MAX];
 
 //Directory command handler 
 /* if(inputLine.find ("cd")!=string::npos)
 {
 if(inputLine.find ("-")!=string::npos)
 {
 chdir(prevPWD);
 getcwd(prevPWD, FILENAME_MAX);
 }
 else{
 getcwd(prevPWD, FILENAME_MAX);
 chdir("-");
 }
 }
*/
 string direct = remove_whitespaces(split(inputLine, ' ') [1]);
 if(direct.length() > 3){
 direct = direct.substr(2);
 } 
 if(previousDirectory.size() > 0 && direct.find("-") != string::npos)
 {
 string prev = previousDirectory.back();
 char* next = (char*)prev.c_str();
 chdir(next);
 previousDirectory.pop_back(); 
 }
 else{
 char current [256];
 getcwd(current, 256);

 char* next = (char*) direct.c_str();
 chdir(next);

 string curr = current;
 previousDirectory.push_back(curr); 
 } 
 
 
}
void awk_handler(string inputLine){
 
}
void execute (string inputLine){
 vector<int> bgs;
 
 //vector<string> parts;
 bool bg = false;
 inputLine = remove_whitespaces(inputLine);

 //removes & from input line
 if(inputLine[inputLine.size() - 1] == '&') 
 {
 bg = true;
 inputLine = inputLine.substr(0,inputLine.size() - 1);
 }
 

 //splitting command line and removing spaces
 vector<string> c = split(inputLine, '|');
 for(int x = 0; x < c.size(); x++)
 {
 string temp1 = c.at(x);
 temp1 = remove_whitespaces(temp1);
 c.at(x) = temp1;
 }
 
 //piping 
 for(int i = 0; i < c.size(); i++)
 {
 string previous, middle, last;
 int fd0, fd1, fd2;
 int location1 = inputLine.find('<'); 
 int location2 = inputLine.find('>'); 
 int fd[2];
 pipe(fd);
 inputLine = remove_whitespaces(c[i]); //ps
 //child process
 int cid = fork();
 if(cid == 0)
 {
 if(inputLine.find("awk") != string::npos)
 { 
 for(int x = 0; x < inputLine.size(); x++)
 { 
 if(inputLine.find("'") != string::npos || inputLine.find("\"") != string::npos)
 { 
 
 size_t found = inputLine.find("'");
 inputLine.erase(found, 1);
 }

 string str0 = inputLine.substr(0,4);
 string str1 = inputLine.substr(4,inputLine.size());

 if(str1.find(" ") != string::npos)
 {
 size_t found = str1.find(" ");
 str1.erase(found, 1); 
 }
 
 inputLine = str0 + str1;
 
 } 
 }
 
 
 if(inputLine.find('>') != string::npos && inputLine.find('<') != string::npos)
 {
 
 previous = inputLine.substr(0, location1);
 
 middle = inputLine.substr(location1+1,2); 
 
 last = inputLine.substr(location2 + 1);

 inputLine = previous;

 char* filen = new char [location1+1];
 strcpy(filen, middle.c_str());

 char* filen1 = new char [location1+1];
 strcpy(filen1, last.c_str());

 fd0 = open(filen, O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR);
 dup2(fd0, 0);
 close(fd0);
 fd0 = open(filen1, O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
 dup2(fd0, 1);
 close(fd0);

 delete[]filen;
 delete[]filen1;

 }
 
 else if(inputLine.find('>') != string::npos)
 {
 int location2 = inputLine.find('>'); 
 previous = inputLine.substr(0, location2); 
 last = inputLine.substr(location2 + 1);
 inputLine = previous;
 char* filen = new char [location2+1];
 strcpy(filen, last.c_str());
 fd1 = open(filen, O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
 dup2(fd1, 1);
 close(fd1);
 delete[]filen;
 
 }
 else if(inputLine.find('<') != string::npos)
 {
 int location1 = inputLine.find('<'); 
 previous = inputLine.substr(0, location1);
 last = inputLine.substr(location1 + 1);
 inputLine = previous;
 char* filen = new char [location1+1];
 strcpy(filen, last.c_str());
 fd2 = open(filen, O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR);
 dup2(fd2, 0);
 close(fd2);
 delete[]filen;
 
 }

 
 if(i < c.size() - 1)
 {
 dup2(fd[1], 1);
 close(fd[1]);
 }
 
 inputLine = remove_whitespaces(inputLine);
 vector<string> parts = split(inputLine, ' ');
 char** args = vec_to_char_array(parts);
 execvp(args[0], args);
 
 }
 else
 {
 if(!bg)
 {
 if(i == c.size() - 1)
 waitpid(cid, 0, 0);
 }
 else
 {
 bgs.push_back(cid);
 }
 dup2(fd[0], 0);
 close(fd[1]);
 }
 } 
}

int main()
{
 vector <string> previousDirectory;
 vector<int> bgs;
 dup2(0,9); 
 while(true)
 {
 dup2(9,0);
 
 //zombie process 
 for(int i = 0; i < bgs.size(); i++)
 {
 if(waitpid(bgs[i], 0, WNOHANG) == bgs[i])
 {
 cout << "Process: " << bgs[i] << " ended" << endl; 
 bgs.erase(bgs.begin() + i);
 i--;
 }
 
 }

 //Shell User prompt w/ time stamp
 time_t personTime = time(NULL); 
 cout << getenv("USER") << " " << ctime(&personTime) << "Richie$ ";

 //taking in command line
 string inputLine;
 getline(cin, inputLine);

 //exit shell conditional
 if(inputLine == string("exit")) 
 {
 cout << "Exited Shell" << endl;
 break;
 }

 if(inputLine.find("echo") != string::npos)
 { 
 quote_checker(inputLine);
 }
 else if(inputLine.find("cd") != string::npos)
 {
 directory_handler(previousDirectory, inputLine);
 }
 else
 {
 execute(inputLine);
 }
 
 
 } 
 
 return 0;
}
