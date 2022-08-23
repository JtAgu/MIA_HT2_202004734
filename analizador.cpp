
#include "analizador.h"

vector<string> Analizador::split_txt(string text){ // Split para separar tipo de comando y parametros
    stringstream text_to_split(text);
    string segment;
    vector<string> splited;

    while(std::getline(text_to_split, segment, ' ')){
       splited.push_back(segment);
    }
    return splited;
}

string Analizador::replace_txt(string str, const string& from, const string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

void Analizador::analizar(string entrada){
    string entradacmd = entrada;
    // Transformacion de toda la entrada a minusculas
    transform(entradacmd.begin(), entradacmd.end(), entradacmd.begin(), ::tolower);
    vector<string> cmdentrada = split_txt(entradacmd);
    vector<string> parametros;
    string comando = "";
    for(int i = 0; i< cmdentrada.size(); i++){
        if(i == 0){
            comando = cmdentrada.at(i);
        }else{
            parametros.push_back(cmdentrada.at(i));
        }
    }
    // Identificacion de comando, parametros y ejecucion
    identificarParametros(comando, parametros);
}

void Analizador::identificarParametros(string comando, vector<string> parametros){
    //PASANDO POR EL COMANDO A RECONOCER CADA PARAMETRO PERMITIDO
    string param = "";
    if(comando == "mkdisk"){
        cmd.param.Comando = "mkdisk";
        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-s->") == 0){
                param = replace_txt(param, "-s->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.s = param;
            }else if(param.find("-u->") == 0){
                param = replace_txt(param, "-u->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.u = param;
            }else if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            } else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        if(cmd.param.s!=" " && cmd.param.path!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
        
    } else if(comando == "fdisk"){
        cmd.param.Comando = "fdisk";

        for(int i=0; i<parametros.size(); i++){
            param = parametros.at(i);
            if(param.find("-s->") == 0){
                if(cmd.param.add_==" "){
                param = replace_txt(param, "-s->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.s = param;
                }
            }else if(param.find("-u->") == 0){
                param = replace_txt(param, "-u->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.u = param;
            }else if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            } else if(param.find("-name->") == 0){
                param = replace_txt(param, "-name->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.name = param;
            }  else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.path!=" "&& cmd.param.name!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }


    } 
    else if (comando == "exec")
    {
        cmd.param.Comando = "exec";

        for (int i = 0; i < parametros.size(); i++)
        {
            param = parametros.at(i);
            if (param.find("-path=") == 0)
            {
                param = replace_txt(param, "-path=", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;

                ifstream archivo(param);
                string linea = "";
                // Obtener línea de archivo, y almacenar contenido en "linea"
                while (getline(archivo, linea))
                {
                    // Lo vamos imprimiendo
                    cout<<"EJECUTANDO:\t"<<linea<<endl;
                    analizar(linea);
                    
                }
                
            }
            else
            {

            }
        }
        // Ejecucion de metodo
        //cmd.identificacionCMD(cmd.param);
    }else if(comando == "rep"){

        for(int i=0; i<parametros.size(); i++){
        param = parametros.at(i);
            if(param.find("-path->") == 0){
                param = replace_txt(param, "-path->", "");
                param = replace_txt(param, "\"", "");
                cmd.param.path = param;
            }  else {
                cout << "*PARAMETRO INCORRECTO*" << endl;
            }
        }
        // Ejecucion de metodo
        if(cmd.param.path!=" "&& cmd.param.name!=" "){
            cmd.identificacionCMD(cmd.param);
        }else{
            cout << "*INFORMACION INSUFICIENTE PARA EJECUCION*" << endl;
        }
            
        
    }else{
        cout << "*COMANDO IRRECONOCIBLE*" << endl;
    }
}