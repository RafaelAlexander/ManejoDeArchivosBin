/*
 ============================================================================
 Name        : ManejoDeArchivosBin.c
 Author      : Rafael
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


void crearBitMapFS(char path[]);
void crearTables(char path[]);
void crearBloquesDatos(char path[],u_int particiones);
void crearUnaTabla(char path[],char nombreDeLaTabla[],uint8_t tipoDeConsistencia,u_int tiempoDeCompactacion,u_int particiones);
void crearMetaDataTabla(char path[],uint8_t tipoDeConsistencia,u_int tiempoDeCompactacion,u_int particiones);
void completarDatosDeMetaData(char path[],uint8_t tipoDeConsistencia,u_int tiempoDeCompactacion,u_int particiones);
char* lecturaDatosDeMetaData(char path[],char[]);

enum  TDeCons{SH=0,SHC=1,EC=2};

typedef struct{
	double timeStamp;
	uint16_t key;
	char* value;
}Registro;

int main(void) {
	char path[100];
		//char *path_aux = config_get_string_value(config,"PUNTOMONTAJE");
		strcpy(path,"/home/utnso/Laureano/");
		strcat(path,"LISSANDRA_FS/");
		uint8_t codDeConsistencia = SH;
		u_int tiempoDeCompactacion=60000;
		u_int particiones=4;
		u_int16_t key=3;
		char* nombreDeLaTabla="TablaA";
		if(mkdir(path, 0777) != 0){
			printf("No se pudo crear Carpeta\n");
		}

		crearBitMapFS(path);
		crearTables(path);
		crearUnaTabla("/home/utnso/Laureano/LISSANDRA_FS/Table/",nombreDeLaTabla,codDeConsistencia,tiempoDeCompactacion,particiones);//se podria settear el config.log con un campo DIRTABLAS
		crearBloquesDatos(path,particiones);
		printf(lecturaDatosDeMetaData("/home/utnso/Laureano/LISSANDRA_FS/Table/",nombreDeLaTabla));
	return EXIT_SUCCESS;
}

void crearBitMapFS(char path[]){
	char path_aux[100];
	strcpy(path_aux,path);
	strcat(path_aux, "Metadata/");
	if(mkdir(path_aux, 0777) != 0){
			printf("No se pudo crear la carpeta /Metadata (Puede que ya este creada)\n");
	}
	strcat(path_aux,"Bitmap.bin");
	FILE *f = fopen(path_aux, "wb+");
	fclose(f);
	//<commons/bitarray.h>
}
void crearTables(char path[]){
	char* path_aux[100];
	strcpy(path_aux,path);
	strcat(path_aux, "Table/");
	if(mkdir(path_aux, 0777) != 0){
		printf("No se pudo crear la carpeta /Tables.\n");
	}
}

void crearUnaTabla(char path[],char nombreDeLaTabla[],uint8_t tipoDeConsistencia,u_int tiempoDeCompactacion,u_int particiones){
	char path_aux[100];
	char nombre_aux[50];
	strcpy(nombre_aux,nombreDeLaTabla);
	strcpy(path_aux,path);
	strcat(nombre_aux,"/");
	strcat(path_aux,nombre_aux);
	if(mkdir(path_aux, 0777) != 0){
			printf("No se pudo crear Tabla");
	}
	//crearMetaDataTabla(path_aux,data);
	crearMetaDataTabla(path_aux,tipoDeConsistencia,tiempoDeCompactacion,particiones);
}
void crearMetaDataTabla(char path[],uint8_t tipoDeConsistencia,u_int tiempoDeCompactacion,u_int particiones){
	char path_aux[100];
	strcpy(path_aux,path);
	strcat(path_aux,"Metadata.bin");
	crearParticionesYTemporales(path,particiones);
	//completarDatosDeMetaData(path_aux,data);
	completarDatosDeMetaData(path_aux,tipoDeConsistencia,tiempoDeCompactacion,particiones);
}
void completarDatosDeMetaData(char path[],uint8_t tipoDeConsistencia,u_int tiempoDeCompactacion,u_int particiones){
	char buffer[100];
	int nbytes;
	//buffer="CONSISTENCY=SC\nPARTITIONS=4\nCOMPACTION_TIME=60000";
	//sprintf(buffer,"PARTITIONS=%d\nCOMPACTION_TIME=%d\n",particiones,tiempoDeCompactacion);
	switch(tipoDeConsistencia){
	case SH:
		//Hice este sprintf para probar que no habia error
		sprintf(buffer,"CONSISTENCY=SC\nPARTITIONS=%d\nCOMPACTION_TIME=%d",particiones,tiempoDeCompactacion);
		break;
	case SHC:
		sprintf(buffer,"CONSISTENCY=SHC\nPARTITIONS=%d\nCOMPACTION_TIME=%d",particiones,tiempoDeCompactacion);
		break;
	case EC:
		sprintf(buffer,"CONSISTENCY=SC\nPARTITIONS=%d\nCOMPACTION_TIME=%d\n",particiones,tiempoDeCompactacion);
		break;
	default:
		printf("\nRecibi tipo de consistencia invalida");
	}
	nbytes=strlen(buffer);
	FILE *f = fopen(path, "wb+");
	if(f){
	//Mando tamaño de string
	fwrite(&nbytes,sizeof nbytes,1,f);
	//Mando String
	fwrite(buffer,nbytes,1,f);
	}
	fclose(f);
}
char* lecturaDatosDeMetaData(char path[],char nombreDeLaTabla[]){
		FILE* f;
		char path_aux[100];
		char aux[50];
		char valoresQueNecesito[50]="";
		strcpy(aux,nombreDeLaTabla);
		strcpy(path_aux,path);
		strcat(aux,"/");
		strcat(path_aux,aux);
		if(opendir(path_aux)==NULL){
					printf("No existe tabla correspondiente\n");
		}
		strcat(path_aux,"Metadata.bin");
		int nbytes;
	    char *contenido;
	    char *delimitador="=\n";
	    f=fopen(path_aux,"rb");

	    if(f){

	    fread (&nbytes, sizeof nbytes, 1, f);
	    contenido = (char *)malloc(nbytes+1);
	    fread (contenido, nbytes, 1, f);
	    contenido[nbytes] = 0;
	   // char* token=strtok_r(contenido,delimitador,guardado);
	    char* token=strtok(contenido,delimitador);
	    if(token!=NULL){
	    	while(token!=NULL){
	    	if(strstr(token,"CONSISTENCY")==NULL&&strstr(token,"PARTITIONS")==NULL&&strstr(token,"COMPACTION_TIME")==NULL){
	    		sprintf(aux,"%s\n",token);
	    		printf(aux);
	    		strcat(valoresQueNecesito,aux);
	    	}
	    		//token=strtok_r(NULL,delimitador,guardado);
	    		token=strtok(NULL,delimitador);
	    	}
	    }
	    }
	    else{
	    	printf("Error en lectura de archivo");
	    	fclose(f);
	    }
	    fclose(f);
	    return valoresQueNecesito;
}
void crearParticionesYTemporales(char path[],u_int particiones){
	char aux[100];
	char temporal[100];
	char particion[100];
	int contador=1;
	FILE* f;
	while(contador<=particiones){
	//DirectorioDeParticion
	strcpy(particion,path);
	sprintf(aux,"%d.bin",contador);
	strcat(particion,aux);
	//DirectorioDeTemporal
	strcpy(temporal,path);
	sprintf(aux,"%d.tmp",contador);
	strcat(temporal,aux);
	//Sube Contador
	contador++;
	//CreacionDeParticion
	f = fopen(particion, "wb+");
	fclose(f);
	//CreacionDeTemporal
	f=fopen(temporal,"wb+");
	fclose(f);
	}
}

void crearBloquesDatos(char path[],u_int particiones){
	char path_aux[100];
	strcpy(path_aux,path);
	strcat(path_aux, "Bloques/");
	if(mkdir(path_aux, 0777) != 0){
		printf("No se pudo crear /Bloques\n");
	}
	crearBloquesDeDatosDeCadaParticion(path_aux,particiones);
}

void crearBloquesDeDatosDeCadaParticion(char path[],u_int particiones){
	char path_aux[100];
	char bloquesDeDatos[100];
	char* contenido="SIZE=0\nBLOCKS=[]";
	FILE* f;
	int nbytes;
	int contador=1;
	while(contador<=particiones){
		strcpy(path_aux,path);
		sprintf(bloquesDeDatos,"%d.bin",contador);
		strcat(path_aux,bloquesDeDatos);
		nbytes=strlen(contenido);
		f = fopen(path_aux, "wb+");
		if(f){
		//Mando tamaño de string
		fwrite(&nbytes,sizeof nbytes,1,f);
		//Mando String
		fwrite(contenido,nbytes,1,f);
		}
		fclose(f);
		contador++;
	}
}
