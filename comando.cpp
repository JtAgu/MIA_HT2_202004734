
#include "comando.h"
#include "MBRactions.cpp"

#include <iostream>
#include <string.h>
#include <algorithm>
#include <cstdlib>
#include <math.h>
#include <time.h>
#include <unistd.h>

const string fecha(time_t now) {
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X %d-%m-%Y ", &tstruct);

    return buf;
}


void Comando::identificacionCMD(Parametros p)
{
    if (p.Comando == "mkdisk")
    { // Se identifica el tipo de comando
        if (p.s != " " && p.path != " ")
        { // Se validan los parametros para el comando

            crearArchivo(p.s, p.path, p.u);
        }
        else
        {
            cout << "Error creando Disco: Parametros obligatorios no definidos." << endl;
        }
    }else if (p.Comando=="fdisk"){
        if ( p.path != " "&&p.name!=" ")
        { // Se validan los parametros para el comando
            fdisk(p.u,p.s,p.path,p.name);
        }
        else
        {
            cout << "Error eliminando Disco: Parametros obligatorios no definidos." << endl;
        }
    }
    else if (p.Comando == "rep")
    { 
        rep(p.path);
    }
}

void Comando::crearArchivo(string tam, string path, string dim)
{
    int size_file = 0, tamano = atoi(tam.c_str());

    char dimen = dim.at(0);

    if (dimen == 'k' || dimen == 'K')
    {
        size_file = tamano*1024;
    }
    else if (dimen == 'm' || dimen == 'M' || dim == " ")
    {
        size_file = tamano * 1024*1024;
    }
    else
    {
        cout << "Error creando Disco: Dimensional no reconocible." << endl;
    }

    if (tamano > 0)
    {
        // Preparacion Bloque

        string direct = "";

        string name_p = "";

        int start = 0;
        int end = path.find("/");
        string del = "/";
        while (end != -1)
        {
            //cout << path.substr(start, end - start) << endl;
            direct += path.substr(start, end - start);
            direct += "/";
            start = end + del.size();
            end = path.find("/", start);
        }
        name_p = path.substr(start, end - start);

        string ncomando = name_p.substr(0, name_p.find("."));
        char solo_nombre[64];
        strcpy(solo_nombre, name_p.c_str());
        for (char c : ncomando)
        {
            if (!isalnum(c) && !isalpha(c) && c != '_')
            {
                cout << "Error: El nombre debe solo puede contener numeros, letras y '_'." << endl;
                return;
            }
        }
        int pos = name_p.find(".");
        name_p.erase(0, 1 + pos);

        transform(name_p.begin(), name_p.end(), name_p.begin(), ::tolower);

        if (name_p != "dsk")
        {
            cout << "Error: El nombre debe contener la extensión \".dsk\" al final" << endl;
            return;
        }

        char bloque[1024];
        for (int i = 0; i < 1024; i++)
        {
            bloque[i] = '\0';
        }

        // Escritura de Bloque en Archivo
        int limite = 0;

        FILE *archivo;
        //direct += solo_nombre;
        
        //cout<<direct<<endl;
        
        string crear = "sudo -S mkdir " + direct ;
        system(crear.c_str());
        // cout<<aux<<endl;
        string compermiso = "sudo chmod 777 " + direct ;
        system(compermiso.c_str());
        
        if ((archivo = fopen(path.c_str(), "w+b")) == NULL)
        {
            printf("[ERROR]error al crear el disco!\n");
        }
        else
        {
            srand(time(NULL));
            MBR p;

            p.mbr_fecha_creacion=time(0);
            p.mbr_tamano = size_file;
            p.mbr_dsk_signature = (rand() % 99);

            p.mbr_partition_1.part_status = '0';
            p.mbr_partition_2.part_status = '0';
            p.mbr_partition_3.part_status = '0';
            p.mbr_partition_4.part_status = '0';

            p.mbr_partition_1.part_type = '\0';
            p.mbr_partition_2.part_type = '\0';
            p.mbr_partition_3.part_type = '\0';
            p.mbr_partition_4.part_type = '\0';

            p.mbr_partition_1.part_fit = '\0';
            p.mbr_partition_2.part_fit = '\0';
            p.mbr_partition_3.part_fit = '\0';
            p.mbr_partition_4.part_fit = '\0';

            p.mbr_partition_1.part_start = 0;
            p.mbr_partition_2.part_start = 0;
            p.mbr_partition_3.part_start = 0;
            p.mbr_partition_4.part_start = 0;

            p.mbr_partition_1.part_s = 0;
            p.mbr_partition_2.part_s = 0;
            p.mbr_partition_3.part_s = 0;
            p.mbr_partition_4.part_s = 0;

            strcpy(p.mbr_partition_1.part_name, "");
            strcpy(p.mbr_partition_2.part_name, "");
            strcpy(p.mbr_partition_3.part_name, "");
            strcpy(p.mbr_partition_4.part_name, "");

            // escribir el mbr
            fwrite(&p, sizeof(MBR), 1, archivo);

            // llenando los espacios en blanco
            char vacio = '\0';
            int i = 0;

            for (i = sizeof(MBR); i < size_file; i++)
            {
                fwrite(&vacio, 1, 1, archivo);
            }

            fclose(archivo);

            
            cout << "\tDISCO CREADO CORRECTAMENTE..." << endl;
        }
    }
    else
    {
        cout << "Error creando Disco: tamaño invalido a ejecución." << endl;
    }
}

void Comando::eliminarArchivo (string path){
        string direct = "";

        string name_p = "";

        int start = 0;
        int end = path.find("/");
        string del = "/";
        while (end != -1)
        {
            //cout << path.substr(start, end - start) << endl;
            direct += path.substr(start, end - start);
            direct += "/";
            start = end + del.size();
            end = path.find("/", start);
        }
        name_p = path.substr(start, end - start);

        string ncomando = name_p.substr(0, name_p.find("."));
        char solo_nombre[64];
        strcpy(solo_nombre, name_p.c_str());
        for (char c : ncomando)
        {
            if (!isalnum(c) && !isalpha(c) && c != '_')
            {
                cout << "Error: El nombre debe solo puede contener numeros, letras y '_'." << endl;
                return;
            }
        }
        int pos = name_p.find(".");
        name_p.erase(0, 1 + pos);

        transform(name_p.begin(), name_p.end(), name_p.begin(), ::tolower);

        if (name_p != "dsk")
        {
            cout << "Error: El nombre debe contener la extensión \".dsk\" al final" << endl;
            return;
        }else{
            remove(path.c_str());
        }
}

void Comando :: fdisk(string dimension,string tamano,string path,string name){

    if(tamano!=" "){
        
        MBR disco=leerMBR(path);

        int prtActiva[] = {0,0,0,0};

        partitiond particiones[4];
        particiones[0] = disco.mbr_partition_1;
        particiones[1] = disco.mbr_partition_2;
        particiones[2] = disco.mbr_partition_3;
        particiones[3] = disco.mbr_partition_4;
        int tam=atoi(tamano.c_str());
        if(dimension!=" "){
            if(dimension=="k"){
                tam=tam*1024;
            }else if (dimension=="m"){
                tam=tam*1024*1024;
            }else if (dimension=="b"){
                
            }else{
                cout<<"Error: el parametro -unit solo reconoce: B, K y M. "<<endl;
                return;
            }
        }

        
            for (int i = 0; i < 4; i++) {
                if(particiones[i].part_s!=0){
                    prtActiva[i]=1;
                }
            }

            string nombre = "";
            
            for (int i = 0; i < 4; i++) {
                nombre=particiones[i].part_name;
                if(nombre==name){
                    cout<<"Error: El nombre "<<name<<" ya está ocupado. "<<endl;
                    return;
                }
            }

            

            int tamanoDisponible=0;
            int tDisponible[4]={0,0,0,0};
            int particionSeleccionada=0;
            

            
            if(prtActiva[0]==0 && prtActiva[1]==0 && prtActiva[2]==0 && prtActiva[3]==0){ 
                tamanoDisponible = disco.mbr_tamano - (sizeof (disco) + 1);
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 0;
                }
            }else if(prtActiva[0]==0 && prtActiva[1]==0 && prtActiva[2]==0 && prtActiva[3]==1) { 
                tamanoDisponible = particiones[3].part_start - (sizeof (disco) + 1);
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 0;
                }
            }else if(prtActiva[0]==0 && prtActiva[1]==0 && prtActiva[2]==1 && prtActiva[3]==0) { 
                
                tDisponible[0] = particiones[2].part_start - (sizeof (disco) + 1);
                tDisponible[3] = disco.mbr_tamano - (particiones[2].part_start+particiones[2].part_s+1);
                if (tam > tDisponible[0]) {
                    if (tam > tDisponible[3]) {
                        cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                        return;
                    } else {
                        tamanoDisponible=tDisponible[3];
                        particionSeleccionada = 3;
                    }
                } else {
                    tamanoDisponible=tDisponible[0];
                    particionSeleccionada = 0;
                }
            }else if(prtActiva[0]==0 && prtActiva[1]==0 && prtActiva[2]==1 && prtActiva[3]==1) { 
                tamanoDisponible = particiones[2].part_start - (sizeof (disco) + 1);
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 0;
                }
            }else if(prtActiva[0]==0 && prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==0) { 
                
                tDisponible[0] = particiones[1].part_start - (sizeof (disco) + 1);
                tDisponible[2] = disco.mbr_tamano - (particiones[1].part_start+particiones[1].part_s+1);
                if (tam > tDisponible[0]) {
                    if (tam > tDisponible[2]) {
                        cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                        return;
                    } else {
                        tamanoDisponible=tDisponible[2];
                        particionSeleccionada = 2;
                    }
                } else {
                    tamanoDisponible=tDisponible[0];
                    particionSeleccionada = 0;
                }
            }else if(prtActiva[0]==0 && prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==1) { 
                
                tDisponible[0] = particiones[1].part_start - (sizeof (disco) + 1);
                tDisponible[2] = particiones[3].part_start - (particiones[1].part_start+particiones[1].part_s+1);
                if (tam > tDisponible[0]) {
                    if (tam > tDisponible[2]) {
                        cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                        return;
                    } else {
                        tamanoDisponible=tDisponible[2];
                        particionSeleccionada = 2;
                    }
                } else {
                    tamanoDisponible=tDisponible[0];
                    particionSeleccionada = 0;
                }
            }else if(prtActiva[0]==0 && prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==0) { 
                
                tDisponible[0] = particiones[1].part_start - (sizeof (disco) + 1);
                tDisponible[3] = disco.mbr_tamano - (particiones[2].part_start+particiones[2].part_s+1);
                if (tam > tDisponible[0]) {
                    if (tam > tDisponible[3]) {
                        cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                        return;
                    } else {
                        tamanoDisponible=tDisponible[3];
                        particionSeleccionada = 3;
                    }
                } else {
                    tamanoDisponible=tDisponible[0];
                    particionSeleccionada = 0;
                }
            }else if(prtActiva[0]==0 && prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==1) { 
                tamanoDisponible = particiones[1].part_start - (sizeof (disco) + 1);
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 0;
                }
            }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==0 && prtActiva[3]==0) { 
                tamanoDisponible = disco.mbr_tamano - (particiones[0].part_start+particiones[0].part_s + 1);
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 1;
                }
            }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==0 && prtActiva[3]==1) { 
                tamanoDisponible = particiones[3].part_start - (particiones[0].part_start+particiones[0].part_s + 1);
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 1;
                }
            }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==1 && prtActiva[3]==0) { 
                
                tDisponible[1] = particiones[2].part_start - (particiones[0].part_start+particiones[0].part_s + 1);
                tDisponible[3] = disco.mbr_tamano - (particiones[2].part_start+particiones[2].part_s+1);
                if (tam > tDisponible[1]) {
                    if (tam > tDisponible[3]) {
                        cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                        return;
                    } else {
                        tamanoDisponible=tDisponible[3];
                        particionSeleccionada = 3;
                    }
                } else {
                    tamanoDisponible=tDisponible[1];
                    particionSeleccionada = 1;
                }
            }else if(prtActiva[0]==1 && prtActiva[1]==0 && prtActiva[2]==1 && prtActiva[3]==1) {
                tamanoDisponible = particiones[2].part_start - (particiones[0].part_start+particiones[0].part_s + 1);
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 1;
                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==0) {
                tamanoDisponible = disco.mbr_tamano - (particiones[1].part_start+particiones[1].part_s + 1);
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 2;
                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==0 && prtActiva[3]==0) {
                tamanoDisponible = particiones[3].part_start - (particiones[1].part_start+particiones[1].part_s + 1);
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 2;
                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==0) {
                tamanoDisponible = disco.mbr_tamano - (particiones[2].part_start+particiones[2].part_s + 1);
                if (tam > tamanoDisponible) {
                    cout<<"Error: el tamaño de la partición supera al espacio disponible"<<endl;
                    return;
                } else {
                    particionSeleccionada = 3;
                }
            }else if(prtActiva[0]==1 && prtActiva[1]==1 && prtActiva[2]==1 && prtActiva[3]==1) {
                cout<<"Error: ya se han ocupado las 4 particiones en este disco "<<endl;
                return;
            }

            cout<<"El tamaño disponible en el disco es de "<<tamanoDisponible<<" bytes"<<endl;
            cout<<"Se creará una partición con "<<tam<<" bytes"<<endl;
            cout<<"en la partición No."<<(particionSeleccionada+1)<<" del disco"<<endl;
            particiones[particionSeleccionada].part_status='1';
            particiones[particionSeleccionada].part_type='P';
            particiones[particionSeleccionada].part_s=atoi(tamano.c_str());
            strcpy(particiones[particionSeleccionada].part_name,name.c_str());

            switch (particionSeleccionada) {
                case 0:
                    particiones[particionSeleccionada].part_start=(sizeof (disco)+1);
                    break;
                case 1:
                    particiones[particionSeleccionada].part_start=(particiones[0].part_start+particiones[0].part_s+1);
                    break;
                case 2:
                    particiones[particionSeleccionada].part_start=(particiones[1].part_start+particiones[1].part_s+1);
                    break;
                case 3:
                    particiones[particionSeleccionada].part_start=(particiones[2].part_start+particiones[2].part_s+1);
                    break;
            }

            cout<<endl;
            cout<<"Start: "<<particiones[particionSeleccionada].part_start<<endl;
            cout<<"Size: "<<tam<<endl;
            cout<<"Name: "<<name<<endl;
            cout<<endl;

            disco.mbr_partition_1=particiones[0];
            disco.mbr_partition_2=particiones[1];
            disco.mbr_partition_3=particiones[2];
            disco.mbr_partition_4=particiones[3];
            actualizarMBR(disco,path);
        
    }
}


void rep(string path){
    string dot = "digraph D {\n"
                 "\n"
                 "    node [fontname=\"Arial\"];\n"
                 "\n"
                 "    node_A [shape=record label=\"";
    MBR retorno = leerMBR(path);
    

    partitiond particiones[4];
    particiones[0] = retorno.mbr_partition_1;
    particiones[1] = retorno.mbr_partition_2;
    particiones[2] = retorno.mbr_partition_3;
    particiones[3] = retorno.mbr_partition_4;
    dot+="a0 [label=<\n <TABLE cellspacing=\"0\" cellpadding=\"10\">\n";
    dot+= "<TR><TD>MBR</TD><TD>PARTICION 1</TD><TD>PARTICION 2</TD><TD>PARTICION 3</TD><TD>PARTICION 4</TD></TR>\n";
    dot+="<TR>";
    dot+="<TD>";
    
    dot += "SIZE: ";
    dot+= to_string(retorno.mbr_tamano);
    dot += "\\n";
    dot+="DATE: ";
    string fechaa= rtfecha(retorno.mbr_fecha_creacion);
    dot+= fechaa;
    dot += "\\n";
    dot+="SIGNATURE: ";
    dot+= to_string(retorno.mbr_dsk_signature);
    dot += "\\n";
    dot+="FIT: ";
    dot+= to_string(retorno.dsk_fit);
    dot+="</TD>\n";

    for (int i = 0; i < 4; ++i) {
        dot+="<TD>";
        string status="";
        status+=particiones[i].part_status;
        string tipo="";
        status+=particiones[i].part_type;
        if(status=="1"){
            dot+="";
            if(tipo=="P"){
                dot+="PRIMARY\nSIZE:  ";
            }else{
                dot+="EXTEND\nSIZE:  ";
            }
            dot+=particiones[i].part_s;
        }else{
            dot+="ESPACIO LIBRE";
        }
        dot+="</TD>\n";
    }
    dot+="</TR>\n";
    dot += "\"    </TABLE>>];\n}\";";
    cout<<dot<<endl;
    //escribirDOT(dot);
}


void escribirDOT(string dot){
    ofstream myfile;
    myfile.open ("reporte.dot");
    myfile << dot;
    myfile.close();

    char comando[1024];
    strcpy(comando,"dot");
    strcat(comando," \'");
    strcat(comando,"reporte.dot");
    strcat(comando,"\' -T");
    strcat(comando," -o \'");
    strcat(comando,"reporte.pdf");
    strcat(comando,"\'");

    system(comando);
    
    cout<<"\t...................Se ha creado el reporte................"<<endl;

}


string rtfecha(time_t now) {
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), " %d-%m-%Y %X", &tstruct);
    string fech = buf;

    return fech;
}