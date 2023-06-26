#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <stack>
#include <set>

// Autor: Santiago Gomez Alzate
using namespace std;
vector<vector<string>> gramatica;
vector<vector<string>> first;
vector<vector<string>> follow;
map<vector<int>,bool> calcu; //Mapa para saber si el first de un simbolo ya fue calculado o se esta calculando
void calcutofalse(){
    for(auto it=calcu.begin();it!=calcu.end();it++){
        it->second=false;
    }
}
void imprimirfirst(){
    cout<<"First:\n";
    for(int i=0;i<first.size();i++){
        if(isupper(first[i][0][0])){
            for(int j=0;j<first[i].size();j++){
                if(j!=1){
                    cout<<first[i][j];
                    if(j==0){
                        cout<<" ==> ";
                    }
                    cout<<" | ";
                }
            }
            cout<<endl;
        }
    }
    cout<<endl;
}
void imprimirfollow(){
    cout<<"Follow:\n";
    for(int i=0;i<follow.size();i++){
        if(isupper(follow[i][0][0])) {
            for (int j = 0; j < follow[i].size(); j++) {
                if (j != 1) {
                    cout << follow[i][j];
                    if (j == 0) {
                        cout << " ==> ";
                    }
                    cout << " | ";
                }
            }
            cout << endl;

        }
    }
    cout<<endl;
}
int buscarenvector(string x, vector<vector<string>> g){
    for(int i=0;i<g.size();i++){
        if(x==g[i][0]){
            return i;
        }
    }
    return -1;
}
int buscarengram(char x){
    for(int i=0;i<gramatica.size();i++){
        if(x==gramatica[i][0][0]){
            return i;
        }
    }
    return -1;
}
// Se agrega terminal al first de un simbolo en caso de que no este
void llevoafirst(int posenf, vector<string>r){
    for(int i=0;i<r.size();i++){
        bool exist=false;
        for(int j=2;j<first[posenf].size();j++){
            if(r[i]==first[posenf][j]){
                exist=true;
            }
        }
        if(!exist){
            first[posenf].push_back(r[i]);
        }
    }
}
// Se agrega terminal al follow de un simbolo en caso de que no este
void llevoafollow(int posenf, vector<string>r){
    for(int i=0;i<r.size();i++){
        bool exist=false;
        for(int j=2;j<follow[posenf].size();j++){
            if(r[i]==follow[posenf][j]){
                exist=true;
            }
        }
        if(!exist) {
            follow[posenf].push_back(r[i]);
        }
    }

}
// Retorna en un vector los simbolos actuales de first o follow para un noterminal, dada su posicion
vector<string> traerdevector(int posenf, vector<vector<string>> f){
    vector<string> res;
    for(int i=2;i<f[posenf].size();i++){
        res.push_back(f[posenf][i]);
    }
    return res;
}
bool tienepsilon(vector<string>r){
    bool b=false;
    for(string s:r){
        if(s=="^"){
            b=true;
        }
    }
    return b;
}
// Datos por defecto para first y follow dado vector de simbolos de la gramatica
void tofirstandfollow(vector<string> f){
    for(int i=0;i<f.size();i++){
        for(int j=0;j<f[i].length();j++){
            string carac=string(1,f[i][j]);
            if(buscarenvector(carac, first) < 0 && carac != "^"){
                string es=":";
                if(isupper(f[i][j])){
                    vector<string> t;
                    t.push_back(carac);
                    t.push_back("::");
                    first.push_back(t);
                    follow.push_back(t);
                }
                else{
                    vector<string> t;
                    t.push_back(carac);
                    vector<string> t2=t;
                    t.push_back("::");
                    follow.push_back(t);
                    t2.push_back(es);
                    t2.push_back(carac);
                    first.push_back(t2);
                }

            }
        }
    }
}
vector<char> simbolosterminales(){
    vector<char> s;
    for(vector<string> p:first){
        if(!isupper(p[0][0])){
            s.push_back(p[0][0]);
        }
    }
    return s;
}
vector<char> simbolosnoterminales(){
    vector<char> s;
    for(vector<string> p:first){
        if(isupper(p[0][0])){
            s.push_back(p[0][0]);
        }
    }
    return s;
}
// Obtiene el first para cualquier cadena de terminales y/o no terminales de la gramatica
vector<string> obternerfirstdecadena(string x){
    vector<string> r;
    for(int i=0;i<x.length();i++){
        bool stop=false;
        if(!isupper(x[i])){
            r.push_back(string(1,x[i]));
            break;
        }
        vector<string>temp= traerdevector(buscarenvector(string(1, x[i]), first), first);
        if(!tienepsilon(temp)){
            stop=true;
        }
        for(string x:temp){
            if(x!="^"){
                bool isthere=false;
                for(int j=0;j<r.size();j++){
                    if(r[j]==x){
                        isthere=true;
                    }
                }
                if(!isthere) {
                    r.push_back(x);
                }
            }
        }
        if(stop){
            break;
        }
        if(i==x.length()-1 && !stop){
            r.push_back("^");
        }
    }
    return r;
}
bool firstgotten=false;
vector<string> firstde(vector<vector<string>> v){
    vector<string> p; // Simbolos first de los nidos superiores
    string x=v[0][0]; // Simbolo a calcular first
    string pila=v[1][0]; // "pila" del nido superior para evitar recalculos"
    bool repetidos=false;
    char noterminalrepetido;
    for(int i=0;i<pila.length();i++){
        for(int j=i+1;j<pila.length();j++){
            if(pila[i]==pila[j]){
                repetidos=true;
                noterminalrepetido=pila[i];
            }
        }
    }
    if(repetidos){
        int posengrammar= buscarengram(noterminalrepetido);
        for(int i=1;i<gramatica[posengrammar].size();i++){
            if(calcu[{posengrammar,i}]==false){
                calcu[{posengrammar,i}]=true;
                vector<string> temp= firstde({{gramatica[posengrammar][i]},
                                              {pila.substr(0, pila.length() - 1)}});
                for(string x:temp){
                    p.push_back(x);
                }
            }
            else{
                if(gramatica[posengrammar][i].length()==1 && isupper(gramatica[posengrammar][i][0])){
                    vector<string> temp= traerdevector(buscarenvector(gramatica[posengrammar][i], first), first);
                    for(string x:temp){
                        p.push_back(x);
                    }
                }

            }
        }
        string pola = string(1, pila[pila.length() - 1]);
        for (char s: pola) {
            int posdenuevoenfirst = buscarenvector(string(1, s), first);
            llevoafirst(posdenuevoenfirst, p);
        }
        pila = pila.substr(0, pila.length() - 1);
        for (char s: pila) {
            int posdenuevoenfirst = buscarenvector(string(1, s), first);
            vector<string> sinepsilon;
            for (string t: p) {
                if (t != "^") {
                    sinepsilon.push_back(string(1, t[0]));
                }
            }
            llevoafirst(posdenuevoenfirst, sinepsilon);
        }
    }
    else {
        if (x.length() > 1) {
            bool epsil = false;
            vector<string> firstofword;
            for (int i = 0; i < x.length(); i++) {
                if (!isupper(x[i]) && x[i] != '^') {
                    firstofword.push_back(string(1, x[i]));
                    break;
                }
                else {
                    vector<string> temp;
                    int posengram = buscarengram(x[i]);
                    int posenf = buscarenvector(string(1, x[i]), first);
                    vector<int> posencalcu = {posengram, 0};
                    if (isupper(x[i]) && calcu[posencalcu] == false) {
                        temp = firstde({{string(1, x[i])},
                                        {pila + x[i]}});
                        calcu[posencalcu] = true;
                    }
                    else if (isupper(x[i]) && calcu[posencalcu] == true) {
                        temp = traerdevector(buscarenvector(string(1, x[i]), first), first);
                    } else {
                        temp = firstde({{string(1, x[i])},
                                        {pila}});
                    }
                    for (string s: temp) {
                        if (s != "^") {
                            firstofword.push_back(s);
                        } else {
                            epsil = true;
                        }

                    }
                    if(epsil && i==x.length()-1){
                        firstofword.push_back("^");
                    }
                    if (!epsil) {
                        break;
                    } else {
                        epsil= false;
                    }
                }
            }
            for(string c:firstofword){
                p.push_back(c);
            }
        }
        else {
            int poseng = buscarenvector(x, gramatica);
            int posenf = buscarenvector(x, first);
            if (!isupper(x[0]) && string(1, x[0]) != "^") {
                p.push_back(string(1, x[0]));
            } else if (isupper(x[0])) {
                for (int i = 1; i < gramatica[poseng].size(); i++) {
                    if(x==string(pila[pila.length()-1],1)){
                        for(int r=0;i<gramatica[poseng].size();i++){
                            if(!calcu[{poseng,r}]){
                                if(!isupper(gramatica[poseng][r][0]) || gramatica[poseng][r]=="^"){
                                    p.push_back(string(1,gramatica[poseng][r][0]));
                                    calcu[{poseng,r}]=true;
                                }
                                else{
                                    vector<string>temp= firstde({{gramatica[poseng][r]},{pila + gramatica[poseng][r]}});
                                    for(string s:temp){
                                        p.push_back(s);
                                    }
                                }
                            }
                        }
                    }
                    if(gramatica[poseng][i].length()==1 && isupper(gramatica[poseng][i][0]) && calcu[{poseng,i}]){
                        for(i;i<gramatica[poseng].size();i++){
                            if(!isupper(gramatica[poseng][i][0]) || gramatica[poseng][i]=="^"){
                                p.push_back(string(1,gramatica[poseng][i][0]));
                            }
                            else if(calcu[{poseng,i}]==false){
                                vector<string>temp= firstde({{gramatica[poseng][i]},
                                                             {pila + gramatica[poseng][i]}});
                                for(string s:temp){
                                    p.push_back(s);
                                }
                            }
                            else{
                            }
                        }
                        vector<string> temp = traerdevector(posenf, first);
                        for (string s: temp) {
                            p.push_back(s);
                        }
                    }
                    else {
                        if(gramatica[poseng][i]=="^"){
                            p.push_back("^");
                        }
                        else if ((gramatica[poseng][i]) != "^") {
                            bool epsil = false;
                            vector<int> ubicprod = {poseng, i};
                            vector<string> primeritodeprod;
                            if (calcu[ubicprod] == false) {
                                calcu[ubicprod] = true;
                                if (gramatica[poseng][i].length() > 1) {
                                    primeritodeprod = firstde({{gramatica[poseng][i]},
                                                               {pila}});
                                } else if (isupper(gramatica[poseng][i][0])) {
                                    int nuevaposdeprod = buscarengram(gramatica[poseng][i][0]);
                                    primeritodeprod = firstde({{gramatica[nuevaposdeprod][0]},
                                                               {pila + gramatica[poseng][i]}});
                                } else {
                                    p.push_back(string(1, gramatica[poseng][i][0]));
                                    calcu[ubicprod] = true;
                                }
                                string resultantepila = pila;
                                for (int l = 0; l < resultantepila.length(); l++) {
                                    char s = resultantepila[l];
                                    int posdenuevoenfirst = buscarenvector(string(1, s), first);
                                    if (l == resultantepila.length() - 1) {
                                        vector<string> temporal;
                                        for (string t: primeritodeprod) {
                                            temporal.push_back(t);
                                        }
                                        llevoafirst(posdenuevoenfirst, temporal);
                                    } else {
                                        vector<string> sinepsilon;
                                        for (string t: primeritodeprod) {
                                            if (t != "^") {
                                                sinepsilon.push_back(string(1, t[0]));
                                            } else { p.push_back("^"); }
                                        }
                                        llevoafirst(posdenuevoenfirst, sinepsilon);
                                    }
                                }
                            }else {
                                if (gramatica[poseng][i].length() == 1 && isupper(gramatica[poseng][i][0])) {
                                    bool s = false;
                                    int nuevaposdeprod = buscarengram(gramatica[poseng][i][0]);
                                    while (!s) {
                                        bool inter = true;
                                        for (int k = 1; k < gramatica[nuevaposdeprod].size(); k++) {
                                            vector<int> ubicprod = {nuevaposdeprod, k};
                                            if (calcu[ubicprod] == false) {
                                                calcu[ubicprod] = true;
                                                inter = false;
                                                vector<string> t = firstde({{gramatica[poseng][i]},
                                                                            {pila}});
                                            }
                                        }
                                        if (inter) {
                                            s = true;
                                        }
                                    }
                                }
                                else{
                                    vector<string>temp= firstde({{gramatica[poseng][i]},
                                                                 {pila + gramatica[poseng][0]}});
                                }
                            }
                        }
                        else {
                            p.push_back("^");
                        }
                    }
                }
            }
            else {
                p.push_back("^");
            }
            for (char s: pila) {
                int posdenuevoenfirst = buscarenvector(string(1, s), first);
                vector<string> sinepsilon;
                for (string t: p) {
                    if (t != "^") {
                        sinepsilon.push_back(string(1, t[0]));
                    }
                }
                llevoafirst(posdenuevoenfirst, sinepsilon);
            }
            if(x==string(1,pila[pila.length()-1])){
                llevoafirst(buscarenvector(string(1, pila[pila.length() - 1]), first), p);
            }
        }
    }
    return p;
}
// Lectura de archivo
// Se lee gramatica con sintaxis: NoTerminal[P1,P2,...Pn,]
// NoTerminal deriva en las Pn producciones. El simbolo epsilon se representa con ^
// Ejemplo:
// A[B,C,^,]
// B[C,^,]
// C[1,2,]
vector<string> procesarlinea(string linea){
    vector<string> result;
    string t="";
    t.push_back(linea[0]);
    result.push_back(t);
    int pos=2;
    while(linea[pos]!=']'){
        string produccion="";
        while(linea[pos]!=','){
            produccion.push_back(linea[pos]);
            pos++;
        }
        result.push_back(produccion);
        pos++;
    }
    return result;
}
vector<string> palabrasleidas;
void inicializarentorno(string archivo){
    string line;
    ifstream myfile(archivo);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            if(isupper(line[0])){
                vector<string> prod = procesarlinea(line);
                tofirstandfollow(prod);
                gramatica.push_back(prod);
                vector<string> temp;
                temp.push_back(prod[0]);
                temp.emplace_back("0");
            }
            else{
                palabrasleidas.push_back(line);
            }
        }
        myfile.close();
    } else {
        cout << "No se pudo abrir el archivo";
        exit(1);
    }
    for (int i = 0; i < gramatica.size(); i++) {
        for (int j = 0; j < gramatica[i].size(); j++) {
            vector<int> temp = {i, j};
            calcu[temp] = false;
        }
    }
}
// Busca las apariciones de un caracter dentro de la gramatica
vector<vector<int>> apariciones(char x){
    vector<vector<int>> r;
    for(int i=0;i<gramatica.size();i++){
        for(int j=1;j<gramatica[i].size();j++){
            for(int k=0;k<gramatica[i][j].length();k++){
                if(gramatica[i][j][k]==x){
                    vector<int> s;
                    s.push_back(i);
                    s.push_back(j);
                    s.push_back(k);
                    r.push_back(s);
                }
            }
        }
    }
    return r;
}
// Mapa que verifica si se ha visitado la produccion en la que aparece un noterminal
map<vector<int>,bool>aparicionvisitada;
bool followgotten=false;

vector<string> historialpilafollow;
void siguientede(string x, string pilafollow){
    vector<string>res;
    int posenf= buscarenvector(x, follow);
    historialpilafollow.push_back(pilafollow);
    string repetido="";
    bool hayrepetido=false;

    for(int i=0;i<pilafollow.length()-1;i++){
        for(int j=i+1;j<pilafollow.length();j++){
            if(pilafollow[i]==pilafollow[j]){
                repetido=string(1,pilafollow[i]);
                hayrepetido=true;
                break;
            }
        }
        if(hayrepetido){
            break;
        }
    }
    if(x==" "){
        return;
    }
    else if(hayrepetido){
        vector<vector<int>> apariciones_repetido= apariciones(repetido[0]);
        for(int i=0;i<apariciones_repetido.size();i++){
            int l=apariciones_repetido[i][0],m=apariciones_repetido[i][1],n=apariciones_repetido[i][2];
            string caracter=string(1,gramatica[l][m][n]);
            if(aparicionvisitada[apariciones_repetido[i]]){
                continue;
            }
            else{
                string subpilafollow=pilafollow.substr(0,pilafollow.length()-1);
                aparicionvisitada[apariciones_repetido[i]]=true;
                siguientede(caracter, subpilafollow);

            }
        }
    }
    else{
        vector<vector<int>>s=apariciones(x[0]);
        for(int i=0;i<s.size();i++){
            int l=s[i][0],m=s[i][1],n=s[i][2];
            int sizeprod=(gramatica[l][m]).length();
            if(aparicionvisitada[s[i]]){
                continue;
            }
            else if(sizeprod-1==n){
                if(gramatica[l][0]!=x){
                    pilafollow+=gramatica[l][0];
                    siguientede(gramatica[l][0], pilafollow);
                    int posprodenf= buscarenvector(gramatica[l][0], follow);
                    vector<string>sA= traerdevector(posprodenf, follow);
                    for(string a: sA){
                        res.push_back(a);
                    }
                }
            }
            else{
                string sub=gramatica[l][m].substr(n+1,sizeprod-(n+1));
                vector<string>firstdeprod= obternerfirstdecadena(sub);
                if(!tienepsilon(firstdeprod)){
                    for(string t:firstdeprod){
                        res.push_back(t);
                    }
                }
                else{
                    for(string t:firstdeprod){
                        if(t!="^"){
                            res.push_back(t);
                        }
                    }
                    siguientede(gramatica[l][0], pilafollow + x[0]);
                    int posprodenfollow= buscarenvector(gramatica[l][0], follow);
                    vector<string>u= traerdevector(posprodenfollow, follow);
                    for(string t:u){
                        res.push_back(t);
                    }
                }
            }
        }
    }
    llevoafollow(posenf, res);
    pilafollow=pilafollow.substr(0,pilafollow.length()-1);
}
void obtenerfirstgeneral(){
    for(int i=0;i<gramatica.size();i++){
        for(int j=i;j<gramatica.size();j++){
            int pos= buscarenvector(gramatica[j][0], first);
            vector<vector<string>> v={{gramatica[j][0]},{gramatica[j][0]}};
            calcu[{j,0}]=true;
            vector<string> r= firstde(v);
        }
        calcutofalse();
    }
    firstgotten=true;
}

void obtenerfollowgeneral(){
    for(int j=0;j<gramatica.size();j++) {
        for (int i = j; i < gramatica.size(); i++) {
            if (i == 0) {
                int posenfolllow = buscarenvector(gramatica[i][0], follow);
                llevoafollow(posenfolllow, {"$"});
            }
            string pilafollow = gramatica[i][0];
            siguientede(gramatica[i][0], pilafollow);
        }
    }
    for(string pila:historialpilafollow){
        string copiapila=pila;
        while(copiapila.length()){
            string tope=string(1,copiapila[copiapila.length()-1]);
            copiapila=copiapila.substr(0,copiapila.length()-1);
            for(char c:copiapila){
                int possolicitanteenfollow= buscarenvector(string(1,c),follow);
                int postopeenfollow= buscarenvector(tope,follow);
                vector<string> vt= traerdevector(postopeenfollow,follow);
                llevoafollow(possolicitanteenfollow,vt);
            }
        }
    }
    followgotten=true;
}
struct parser {
    vector<char> terminals;
    vector<char> nonterminals;
    map<char, map<char, vector<int>>> a;
    parser(){
        this->terminals=simbolosterminales();
        this->terminals.push_back('$');
        this->nonterminals=simbolosnoterminales();
        for(int i=0;i<nonterminals.size();i++){
            for(int j=0;j<terminals.size();j++){
                a[nonterminals[i]][terminals[j]]={-1,-1};
            }
        }
        if(!firstgotten){
            obtenerfirstgeneral();
        }
        if(!followgotten){
            obtenerfollowgeneral();
        }
    }
    parser(vector<char> simbols,vector<char>nonterminals){
        this->terminals=simbols;
        this->terminals.push_back('$');
        this->nonterminals=nonterminals;
        for(int i=0;i<nonterminals.size();i++){
            for(int j=0;j<terminals.size();j++){
                a[nonterminals[i]][terminals[j]]={-1,-1};
            }
        }
        if(!firstgotten){
            obtenerfirstgeneral();
        }
        if(!followgotten){
            obtenerfollowgeneral();
        }
    }
    bool estaenterminales(char c){
        for(int i=0;i<terminals.size();i++){
            if(terminals[i]==c){
                return true;
            }
        }
        return false;
    }
    bool procesarcadena(string x){
        int puntero=0;
        string stack=gramatica[0][0]+"$";
        string input=x+"$";
        while(stack[0]!='$'){
            if(stack[0]==input[0]){
                stack=stack.substr(1,stack.length());
                input=input.substr(1,input.length());
            }
            else if(!isupper(stack[0])){
                return false;
            }
            else if(!estaenterminales(input[0])){
                return false;
            }
            else if(a[stack[0]][input[0]][0]==-1){
                return false;
            }
            else{
                int pos=a[stack[0]][input[0]][0];
                int prod=a[stack[0]][input[0]][1];
                stack=stack.substr(1,stack.length());
                string prodstring=gramatica[pos][prod];
                if(prodstring!="^"){
                    stack=prodstring+stack;
                }
            }

        }
        return true;
    }
    void validarcadena(string x){
        if(procesarcadena(x)){
            cout<<"La cadena es valida"<<endl;
        }
        else{
            cout<<"La cadena no es valida"<<endl;
        }
    }

};

void imprimirparsergrafico(parser p){
    cout<<"  |     ";
    for(int i=0;i<p.terminals.size();i++){
        cout<<p.terminals[i]<<"     |    ";
    }
    cout<<endl;
    for(int i=0;i<p.nonterminals.size();i++){
        cout<<p.nonterminals[i]<<"   ";
        for(int j=0;j<p.terminals.size();j++){
            // imprimo la produccion
            if(p.a[p.nonterminals[i]][p.terminals[j]][0]!=-1){
                cout<<" "<<p.nonterminals[i]<<"-> "<<gramatica[p.a[p.nonterminals[i]][p.terminals[j]][0]][p.a[p.nonterminals[i]][p.terminals[j]][1]]<<"  | ";
            }
            else{
                cout<<"error() | ";
            }
        }
        cout<<endl;
        for(int k=0;k<(p.terminals.size())*11;k++){
            cout<<"-";
        }
        cout<<endl;
    }
}
bool parsertopdowngotten=false;
parser sacarparser(){
    vector<char>term=simbolosterminales();
    vector<char>noterm=simbolosnoterminales();
    parser resultado(term,noterm);
    for(char head:noterm){
        int pos= buscarengram(head);
        for(int i=1;i<gramatica[pos].size();i++){
            vector<string> firstprod= obternerfirstdecadena(gramatica[pos][i]);
            for(string primerito:firstprod){
                vector<int>temp={pos,i};
                if(primerito!="^"){
                    vector<int>examino=resultado.a[head][primerito[0]];
                    if(examino[0]==-1){
                        resultado.a[head][primerito[0]]=temp;
                    }
                    else {
                        if(examino[0]!=pos || examino[1]!=i){
                            cout << "Doble entrada en misma casilla dentro de parser"<< endl;
                            cout<<"Cuando se intenta poner esta produccion: "<<gramatica[pos][0]<<"->"<<gramatica[pos][i]<<" en la casilla "<<head<<","<<primerito[0]<<endl;
                            cout<<"Se encuentra que ya esta la produccion "<<gramatica[examino[0]][0]<<"->"<<gramatica[examino[0]][examino[1]]<<endl;
                            cout<<"No es LL1"<<endl;
                            exit(2);
                        }
                    }
                }
                else{
                    int posenfollow= buscarenvector(gramatica[pos][0], follow);
                    vector<string> followofhead= traerdevector(posenfollow, follow);
                    for(string minifollow:followofhead){
                        vector<int>examino=resultado.a[head][minifollow[0]];
                        if(examino[0]==-1){
                            resultado.a[head][minifollow[0]]=temp;
                        }
                        else{
                            if(examino[0]!=pos || examino[1]!=i){
                                cout<<"Doble entrada en misma casilla dentro de parser"<<endl;
                                cout<<"Cuando se intenta poner esta produccion: "<<gramatica[pos][0]<<"->"<<gramatica[pos][i]<<endl;
                                cout<<"Se encuentra que ya esta la produccion "<<gramatica[examino[0]][0]<<"->"<<gramatica[examino[0]][examino[1]]<<endl;
                                cout<<"No es LL1"<<endl;
                                exit(3);
                            }
                        }
                    }
                }
            }
        }
    }
    parsertopdowngotten;
    return resultado;
}

vector<vector<string>> grammartolist(){
    vector<vector<string>> listgrammar;
    for(int i=0;i<gramatica.size();i++){
        for(int j=1;j<gramatica[i].size();j++){
            vector<string>temp={gramatica[i][0],gramatica[i][j]};
            listgrammar.push_back(temp);
        }
    }
    return listgrammar;
}
vector<vector<string>> listagramglobal;
// Estructura item, un item es en esencia 2 enteros, el numero de regla y la posicion del punto
struct item{
    bool closure=false;
    bool goneto=false;
    int regla;
    int punto;
    item(int r,int p,bool f){
        this->closure=f;
        this->regla=r;
        this->punto=p;
    }
    item(int r,int p){
        this->regla=r;
        this->punto=p;
    }
    void imprimir(){
        cout<<listagramglobal[regla][0]<<" -> ";
        for(int i=0;i<punto;i++){
            cout<<listagramglobal[regla][1][i];
        }
        cout<<".";
        for(int i=punto;i<listagramglobal[regla][1].length();i++){
            cout<<listagramglobal[regla][1][i];
        }
        cout<<endl;
    }
};
// Un estado basicamente es un conjunto de items, y un mapa de transiciones
struct estado{
    int numestado;
    vector<item> items;
    bool gone_to=false;
    bool closured=false;
    map<char,int>map_shift_action;
    map<char,int>map_reduce;
    estado(int n,item i){
        this->numestado=n;
        this->items.push_back(i);
    }
    estado(int n,vector<item> i){
        this->numestado=n;
        for(item local:i){
            items.push_back(local);
        }
    }
    void additem(item i){
        bool esta=false;
        for(item local:items){
            if(local.regla==i.regla){
                if(local.punto==i.punto){
                    esta=true;
                }
            }
        }
        if(!esta){
            items.push_back(i);
        }
    }
    void additems(vector<item> v){
        for(item i:v){
            additem(i);
        }
    }
    bool en_estado(vector<int> it){
        bool esta=false;
        for(item i:items){
            if(i.regla==it[0] && i.punto==it[1]){
                esta=true;
                break;
            }
        }
        return esta;
    }
    int pos_shift_map(char c){
        int res=-1;
        for(auto i:map_shift_action){
            if(i.first==c){
                res=i.second;
                break;
            }
        }
        return res;
    }
    int pos_reduce_map(char c){
        int res=-1;
        for(auto i:map_reduce){
            if(i.first==c){
                res=i.second;
                break;
            }
        }
        return res;
    }
    void imprimir(){
        cout<<"Estado "<<numestado<<endl;
        for(item i:items){
            i.imprimir();
        }
        cout<<endl;
    }
    void imprimirmapa_st_a(){
        if(map_shift_action.size()>0){
            cout<<"Shifts y Gotos de estado "<<numestado<<endl;
            for(auto i:map_shift_action){
                cout<<i.first<<" -> ";
                if(i.second==-2){
                    cout<<"Aceptacion"<<endl;
                }
                else{
                    cout<<i.second<<endl;
                }
            }
            cout<<endl;
        }
    }
    void imprimir_reduce(){
        if(map_reduce.size()>0){
            cout<<"Reducciones del estado "<<numestado<<endl;
            for(auto i:map_reduce){
                cout<<i.first<<" -> "<<i.second<<endl;
            }
            cout<<endl;
        }
    }
};

bool parsingtablegotten=false;
// El canonical es un conjunto de estados
struct canonical{
    vector<estado> estados;
    vector<vector<string>> gram;
    string nuevosimbolo;
    map<int,map<char,vector<int>>> parsingtable;
    bool doble_entry=false;
    vector<char> simbolosgramaticales;
    canonical(){}
    canonical(vector<vector<string>> g,string ns){
        this->gram=g;
        this->nuevosimbolo=ns;
        item primeritem(0,0);
        estado primerestado(0,primeritem);
        this->estados.push_back(primerestado);
    }
    vector<item> traeritems(string noterminal){
        vector<item> bolsita;
        for(int i=1;i<gram.size();i++){
            if(gram[i][0]==noterminal){
                item nuevo(i,0);
                bolsita.push_back(nuevo);
            }
        }
        return bolsita;
    }
    void closure(estado &e){
        bool cambios=true;
        while(cambios){
            bool seagrego=false;
            for(int j=0;j<e.items.size();j++){
                if(!e.items[j].closure){
                    int item_regla=e.items[j].regla;
                    int item_punto=e.items[j].punto;
                    int longitud=gram[item_regla][1].length();
                    if(item_punto<longitud){
                        char simbolo=gram[item_regla][1][item_punto];
                        if(isupper(simbolo)){
                            e.additems(traeritems(string(1,simbolo)));
                            seagrego=true;
                        }
                    }
                }
                e.items[j].closure=true;
            }
            if(!seagrego){
                cambios=false;
            }
        }
        e.closured=true;
    }
    void closure_estados(){
        for(estado &e:estados){
            if(!e.closured){
                closure(e);
            }
        }
    }
    vector<vector<int>> recolectaritems(int pos_de_estado, char x){
        vector<vector<int>> r;
        for(item &i: estados[pos_de_estado].items){
            if(gram[i.regla][1][i.punto]==x){
                r.push_back({i.regla,i.punto+1});
                i.goneto=true;
            }
        }
        return r;
    }
    int posestado(vector<vector<int>> _items){
        int pos=-1;
        for(estado e:estados){
            bool items_estan=true;
            for(vector<int> _item: _items){
                if(!e.en_estado(_item)){
                    items_estan=false;
                }
            }
            if(items_estan){
                pos=e.numestado;
                break;
            }
        }
        return pos;
    }
    bool go_to(estado e){
        bool seingresaron=false;
        for(item i: e.items){
            if(!i.goneto){
                string produccion=this->gram[i.regla][1];
                int num_estado;
                if((i.punto>=produccion.length()) && gram[i.regla][0]==nuevosimbolo){
                    i.goneto=true;
                    num_estado=-2;
                    this->estados[e.numestado].map_shift_action['$']=num_estado;
                }
                else if((i.punto>=produccion.length()) || produccion=="^"){
                    i.goneto=true;
                    int posenfollow= buscarenvector(gram[i.regla][0], follow);
                    vector<string> follow_head_item= traerdevector(posenfollow, follow);
                    for(string s: follow_head_item){
                        if(this->estados[e.numestado].pos_reduce_map(s[0]) == -1){
                            this->estados[e.numestado].map_reduce[s[0]]=i.regla;
                        }
                        else if(this->estados[e.numestado].pos_reduce_map(s[0]) == i.regla){
                            continue;
                        }
                        else{
                            cout<<"Error en reduce"<<endl;
                            cout<<"Al intentar reducir "<<s[0]<<" en el estado "<<e.numestado<<" con la regla "<<i.regla<<endl;
                            cout<<"Ya se habia reducido con la regla "<<this->estados[e.numestado].map_reduce[s[0]]<<endl;
                            this->doble_entry=true;
                            exit(4);
                        }
                    }
                }
                else{
                    vector<vector<int>> proximos=recolectaritems(e.numestado,produccion[i.punto]);
                    num_estado= posestado(proximos);
                    if(num_estado==-1){
                        vector<item> saliente;
                        for(vector<int> v: proximos){
                            item newitem(v[0],v[1]);
                            saliente.push_back(newitem);
                        }
                        estado nuevo(this->estados.size(),saliente);
                        num_estado=nuevo.numestado;
                        estados.push_back(nuevo);
                        seingresaron=true;
                    }
                    if(this->estados[e.numestado].pos_shift_map(produccion[i.punto] == -1)){
                        this->estados[e.numestado].map_shift_action[produccion[i.punto]]=num_estado;
                    }
                    else{
                        cout<<"Error en goto_action"<<endl;
                        cout<<"Cuando se intenta introducir "<<produccion[i.punto]<<" para ir con el estado "<<num_estado<<endl;
                        cout << "Se encontro que ya esta asociado a la letra el estado " << this->estados[e.numestado].pos_shift_map(produccion[i.punto]) << endl;
                        this->doble_entry=true;
                    }
                }
            }
        }
        e.gone_to=true;
        return seingresaron;
    }
    void states_go_to(){
        bool cambios=true;
        while(cambios){
            bool estadosagregados=false;
            for(estado e: estados){
                if(!e.gone_to){
                    bool intern= go_to(e);
                    if(intern){
                        estadosagregados=true;
                        closure_estados();
                    }
                }
            }
            if(!estadosagregados){
                cambios=false;
                break;
            }
        }
    }
    void imprimircanonical(){
        for(estado e:estados){
            e.imprimir();
            e.imprimirmapa_st_a();
            e.imprimir_reduce();
            cout<<"==========================="<<endl;
        }
    }
    vector<int> in_parsingtable(int numero, char simbolo){
        vector<int> res={-1,-1};
        for(auto i: parsingtable){
            if(i.first==numero){
                for(auto j: i.second){
                    if(j.first==simbolo){
                        res=j.second;
                    }
                }
            }
        }
        return res;
    }
    void llenar_parsingTable(){
        if(doble_entry){
            cout<<"Error: Conflicto por doble entrada llenando la Parsing Table"<<endl;
            exit(7);
        }
        int n=estados.size();
        vector<char> st=simbolosterminales();
        st.push_back('$');
        vector<char> snt=simbolosnoterminales();
        st.insert(st.end(),snt.begin(),snt.end());
        vector<char> simbolosgramatica=st;

        this->simbolosgramaticales=st;
        for(int i=0;i<estados.size();i++){
            for(int j=0;j<simbolosgramatica.size();j++){
                int posenshift=estados[i].pos_shift_map(simbolosgramatica[j]);
                if(posenshift!=-1){
                    if(simbolosgramatica[j]=='$'){
                        vector<int> res={3,posenshift};
                        parsingtable[i][simbolosgramatica[j]]=res;
                    }
                    else{
                        vector<int> res={1,posenshift};
                        parsingtable[i][simbolosgramatica[j]]=res;
                    }
                }
                int posenreduce=estados[i].pos_reduce_map(simbolosgramatica[j]);
                if(posenreduce!=-1){
                    if(in_parsingtable(i, simbolosgramatica[j])[0] == -1){
                        if(posenreduce!=-1){
                            vector<int> res={2,posenreduce};
                            parsingtable[i][simbolosgramatica[j]]=res;
                        }
                        else{
                            vector<int> res={-1,-1};
                            parsingtable[i][simbolosgramatica[j]]=res;
                        }
                    }
                    else{
                        cout<<"Error: Conflicto por doble entrada llenando la Parsing Table"<<endl;
                        cout<<"En el estado "<<i<<" con el simbolo "<<simbolosgramatica[j]<<endl;
                        cout<<"No es LR(0)";
                        exit(6);
                    }
                }
            }
        }
        parsingtablegotten=true;
    }
    bool procesarCadena(string x){
        stack<int> pilaestados;
        pilaestados.push(0);
        string simbolos;
        string input=x+'$';
        while(true){
            vector<int> action= in_parsingtable(pilaestados.top(), input[0]);
            if(action[0]==1){
                simbolos+=input[0];
                pilaestados.push(action[1]);
                input=input.substr(1,input.length());
            }
            else if(action[0]==2){
                int n=gram[action[1]][1].length();
                char head=gram[action[1]][0][0];
                if(gram[action[1]][1]=="^"){n=0;}
                simbolos=simbolos.substr(0,simbolos.length()-n);
                while(n--){
                    pilaestados.pop();
                }
                int s= in_parsingtable(pilaestados.top(), head)[1];
                pilaestados.push(s);
                simbolos+=head;
            }
            else if(action[0]==3){
                return true;
            }
            else{
                return false;
            }
        }
    }
    void validarcadena(string x){
        if(procesarCadena(x)){
            cout<<"Cadena Valida"<<endl;
        }
        else{
            cout<<"Cadena Invalida"<<endl;
        }
    }
    void validarcadenas(vector<string> v){
        for(string s:v){
            validarcadena(s);
        }
    }
    void imprimir_parsing_table(){
        for(int i=0;i<simbolosgramaticales.size()*3;i++){
            cout<<" ";
        }
        cout<<"Parsing Table"<<endl;
        cout<<endl;
        cout<<"  |   ";
        for(char s:simbolosgramaticales){
            cout<<s<<"   |   ";
        }
        cout<<endl;
        for(auto i: parsingtable){
            cout<<i.first<<" |";
            for(char s:simbolosgramaticales){
                vector<int> action= in_parsingtable(i.first, s);
                if(action[0]==-1){
                    cout<<"       |";
                }
                else if(action[0]==1){
                    if(isupper(s)){
                        cout<<"   "<<action[1]<<"   |";
                    }
                    else{
                        cout<<"  s"<<action[1]<<"   |";
                    }
                }
                else if(action[0]==2){
                    cout<<"  r"<<action[1]<<"   |";
                }
                else if(action[0]==3){
                    cout<<"  acc  |";
                }
            }
            cout<<endl;
        }
        cout<<endl;
    }
};
bool canonicalgotten=false;
string abecedario="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
canonical sacarcanonical(){
    string simboloinicial=gramatica[0][0];
    string nuevosimbolo;
    vector<char>noterm=simbolosnoterminales();
    for(int i=0;i<abecedario.length();i++){
        bool esta=false;
        for(int j=0;j<noterm.size();j++){
            if(abecedario[i]==noterm[j]){
                esta=true;
                break;
            }
        }
        if(!esta){
            nuevosimbolo=abecedario[i];
            break;
        }
    }
    vector<vector<string>> nuevagram={{nuevosimbolo,simboloinicial}};
    vector<vector<string>> viejagram=grammartolist();
    for(int i=0;i<viejagram.size();i++){
        nuevagram.push_back(viejagram[i]);
    }
    listagramglobal=nuevagram;
    canonical resultado(nuevagram,nuevosimbolo);
    resultado.closure_estados();
    resultado.states_go_to();
    canonicalgotten=true;
    return resultado;
}

int main() {
    inicializarentorno("..\\caso4.txt");
    int opcion;
    parser parsertopdown;
    canonical c;
    while (true) {
        // Mostrar menú principal
        cout << "Que desea hacer?" << endl;
        cout << "1 - Opciones con top-down" << endl;
        cout << "2 - Opciones con bottom-up" << endl;
        cout << "-1 - Salir" << endl;
        cout << "Digite su opcion: ";
        cin >> opcion;

        if (opcion == -1) {
            break; // Salir del bucle principal
        }

        switch (opcion) {
            case 1: {
                int opcion2;
                bool regresar = false;

                while (!regresar) {
                    // Mostrar menú de opciones para top-down
                    cout << "Eliga una opcion" << endl;
                    cout << "1 - First y Follow de la gramatica" << endl;
                    cout << "2 - First de una cadena" << endl;
                    cout << "3 - Tabla Top Down" << endl;
                    cout << "4 - Procesar las cadenas leidas del documento" << endl;
                    cout << "0 - Regresar" << endl;
                    cout << "Digite su opcion: ";
                    cin >> opcion2;

                    switch (opcion2) {
                        case 1:
                            if (!firstgotten) {
                                obtenerfirstgeneral();
                            }
                            if (!followgotten) {
                                obtenerfollowgeneral();
                            }
                            imprimirfirst();
                            imprimirfollow();
                            break;
                        case 2: {
                            string cad;
                            cout << "Cero (0) para regresar" << endl;
                            cout << "Escriba la cadena: ";
                            cin >> cad;

                            while (cad != "0") {
                                vector<string> o = obternerfirstdecadena(cad);
                                cout << "First de la cadena: ";
                                for (string s : o) {
                                    cout << s << " ";
                                }
                                cout << endl;

                                cout << "Cero (0) para regresar" << endl;
                                cout << "Escriba la cadena: ";
                                cin >> cad;
                            }
                            break;
                        }
                        case 3: {
                            if (!parsertopdowngotten) {
                                parsertopdown = sacarparser();
                            }
                            imprimirparsergrafico(parsertopdown);
                            break;
                        }
                        case 4: {
                            if (!parsertopdowngotten) {
                                parsertopdown = sacarparser();
                            }
                            for (string s : palabrasleidas) {
                                parsertopdown.validarcadena(s);
                            }
                            break;
                        }
                        case 0:
                            regresar = true;
                            break;
                        default:
                            cout << "Opcion invalida" << endl;
                            break;
                    }
                }

                break;
            }
            case 2: {
                int opcion3;

                while (true) {
                    // Mostrar menú de opciones para bottom-up
                    cout << "Eliga una opcion" << endl;
                    cout << "1 - Crear canonical con items" << endl;
                    cout << "2 - Imprimir tabla de analisis sintactico" << endl;
                    cout << "3 - Procesar las cadenas leidas del documento" << endl;
                    cout << "0 - Regresar" << endl;
                    cout << "Digite su opcion: ";
                    cin >> opcion3;
                    if (opcion3 == 0) {
                        break; // Regresar al menú principal
                    }
                    switch (opcion3) {
                        case 1:
                            if (!canonicalgotten) {
                                c=sacarcanonical();
                            }
                            c.imprimircanonical();
                            break;
                        case 2:
                            if (!canonicalgotten) {
                                c=sacarcanonical();
                            }
                            if (!parsingtablegotten) {
                                c.llenar_parsingTable();
                            }

                            c.imprimir_parsing_table();
                            break;
                        case 3:
                            if (!canonicalgotten) {
                                c=sacarcanonical();
                            }
                            if (!parsingtablegotten) {
                                c.llenar_parsingTable();
                            }
                            for (string s : palabrasleidas) {
                                c.validarcadena(s);
                            }
                            break;
                        case 0:
                            break;
                        default:
                            cout << "Opcion invalida" << endl;
                            break;
                    }
                }

                break;
            }
            default:
                cout << "Opcion invalida" << endl;
                break;
        }
    }
    return 0;

}
